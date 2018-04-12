#include "mytcp.h"


/*******************************************************************
* 名称：                TCP_Init()
* 功能：                socket初始化
* 入口参数：
*               fd              :   文件描述符
*               sockaddr_in     :   ip地址结构体
*
* 出口参数：      正确返回为0，错误返回为-1
*******************************************************************/
int TCP_Init(int *fd,struct sockaddr_in *seraddr)
{
//    char ch[]="hello\r\n";
    printf("create socket\n");
    if((*fd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("create socket error!\n");
        return -1;
    }
    //如果是AF_INET 那么地址由结构体sockaddr_in所决定
    memset(seraddr,0,sizeof(*seraddr));

    seraddr->sin_family=AF_INET;//选择ipv4通信
    seraddr->sin_addr.s_addr=inet_addr("172.24.41.10");//赋值ip
    seraddr->sin_port=htons(1234);//设置端口号

//    printf("inet_pton\n");
//    if(inet_pton(AF_INET,"172.24.41.10",&(seraddr->sin_addr))<=0)
//    {
//        printf("inet_pton error \n");
//        return -1;
//    }

    printf("connect \n");
    if(connect(*fd,(struct sockaddr *)seraddr,sizeof(*seraddr))<0)
    {
        printf("connect error : %s \n",strerror(errno));
        return -1;
    }
    printf("connect success!\n");

//    send(*fd,ch,strlen(ch),0);

    return 0;
}
