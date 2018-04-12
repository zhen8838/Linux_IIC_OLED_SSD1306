#include "gps.h" 


void Gps_Msg_Show(nmea_msg *gps,char *dtbuf)
{
    double tp;
    tp=gps->longitude;
    sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gps->ewhemi);//经度
    printf("%s\n",dtbuf);
    tp=gps->latitude;
    sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gps->nshemi);//纬度
    printf("%s\n",dtbuf);
}

void Gps_Msg_Socket(int fd,nmea_msg * gps,char * dtbuf)
{
    double tp;
    tp=gps->longitude;
    sprintf((char *)dtbuf,"Longitude:%.5f %1c \r\n",tp/=100000,gps->ewhemi);//经度
    send(fd,dtbuf,strlen(dtbuf),0);
    tp=gps->latitude;
    sprintf((char *)dtbuf,"Latitude:%.5f %1c \r\n",tp/=100000,gps->nshemi);//纬度
    send(fd,dtbuf,strlen(dtbuf),0);
}

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号							  
uint8_t NMEA_Comma_Pos( uint8_t *buf,uint8_t cx)//出错代码段
{	 		    
    uint8_t *p=buf;
    while(cx)
    {
        if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
        if(*buf==',')cx--;
        buf++;
    }
    return buf-p;
}
//m^n函数
//返回值:m^n次方.
uint32_t NMEA_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int NMEA_Str2num(uint8_t *buf,uint8_t*dx)
{
    uint8_t *p=buf;
    uint32_t ires=0,fres=0;
    uint8_t ilen=0,flen=0,i;
    uint8_t mask=0;
    int res;
    while(1) //得到整数和小数的长度
    {
        if(*p=='-'){mask|=0X02;p++;}//是负数
        if(*p==','||(*p=='*'))break;//遇到结束了
        if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
        else if(*p>'9'||(*p<'0'))	//有非法字符
        {
            ilen=0;
            flen=0;
            break;
        }
        if(mask&0X01)flen++;
        else ilen++;
        p++;
    }
    if(mask&0X02)buf++;	//去掉负号
    for(i=0;i<ilen;i++)	//得到整数部分数据
    {
        ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen>5)flen=5;	//最多取5位小数
    *dx=flen;	 		//小数点位数
    for(i=0;i<flen;i++)	//得到小数部分数据
    {
        fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
    }
    res=ires*NMEA_Pow(10,flen)+fres;
    if(mask&0X02)res=-res;
    return res;
}	  							 
//分析GPGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    uint8_t *p,*p1,dx;
    uint8_t len,i,j,slx=0;
    uint8_t posx;
    p=buf;
    p1=(uint8_t*)strstr((const char *)p,"$GPGSV");
    len=p1[7]-'0';								//得到GPGSV的条数
    posx=NMEA_Comma_Pos(p1,3); 					//得到可见卫星总数
    if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++)
    {
        p1=(uint8_t*)strstr((const char *)p,"$GPGSV");
        for(j=0;j<4;j++)
        {
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//得到卫星编号
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//得到卫星信噪比
            else break;
            slx++;
        }
        p=p1+1;//切换到下一个GPGSV信息
    }
}
//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx;
    uint8_t posx;
    p1=(uint8_t*)strstr((const char *)buf,"$GPGGA");
    posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
    if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
    if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
    if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);
}
//分析GPGSA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx;
    uint8_t posx;
    uint8_t i;
    p1=(uint8_t*)strstr((const char *)buf,"$GPGSA");
    posx=NMEA_Comma_Pos(p1,2);								//得到定位类型
    if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<12;i++)										//得到定位卫星编号
    {
        posx=NMEA_Comma_Pos(p1,3+i);
        if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
        else break;
    }
    posx=NMEA_Comma_Pos(p1,15);								//得到PDOP位置精度因子
    if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,16);								//得到HDOP位置精度因子
    if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,17);								//得到VDOP位置精度因子
    if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);
}
//分析GPRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx;
    uint8_t posx;
    uint32_t temp;
    float rs;

    p1=(uint8_t*)strstr((const char *)buf,"GPRMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.

    posx=NMEA_Comma_Pos(p1,3);								//得到纬度
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//得到°
        rs=temp%NMEA_Pow(10,dx+2);				//得到'
        gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为°
    }
    posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬
    if(posx!=0XFF)gpsx->nshemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,5);								//得到经度
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
        rs=temp%NMEA_Pow(10,dx+2);				//得到'
        gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为°
    }
    posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
    if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);
}
//分析GPVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx;
    uint8_t posx;
    p1=(uint8_t*)strstr((const char *)buf,"$GPVTG");
    posx=NMEA_Comma_Pos(p1,7);								//得到地面速率
    if(posx!=0XFF)
    {
        gpsx->speed=NMEA_Str2num(p1+posx,&dx);
        if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//确保扩大1000倍
    }
}  
//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
    //	NMEA_GPGSV_Analysis(gpsx,buf);	//GPGSV解析
    //	NMEA_GPGGA_Analysis(gpsx,buf);	//GPGGA解析
    //	NMEA_GPGSA_Analysis(gpsx,buf);	//GPGSA解析
    if((strstr((const char *)buf,"$GPRMC")))
        NMEA_GPRMC_Analysis(gpsx,buf);	//GPRMC解析
    //	NMEA_GPVTG_Analysis(gpsx,buf);	//GPVTG解析
}






