#include "mycommon.h"
#include "myi2c-dev.h"

int setI2CSlave(int fd, int slave)
{
	if (ioctl(fd, I2C_SLAVE, slave) < 0)
	{
		errMsg("Fail ioctl I2C_SLAVE");
		return -1;
	}
	return 0;
}

int setI2CTimeout(int fd, int timeout)
{
	if (ioctl(fd, I2C_TIMEOUT, timeout) < 0)
	{
		errMsg("Fail ioctl I2C_TIMEOUT");
		return -1;
	}
	return 0;
}

int setI2CRetries(int fd, int retries)
{
	if (ioctl(fd, I2C_RETRIES, retries) < 0)
	{
		errMsg("Fail ioctl I2C_RETRIES");
		return -1;
	}
	return 0;
}

int I2CWriteByteTo(int fd, int pos, unsigned char byteData, int wait_ms)
{
	if (i2c_smbus_write_byte_data(fd, (unsigned char)pos, (unsigned char)byteData) < 0)
	{
		errMsg("Fail to I2CWriteByteTo");
		return -1;
	}
	usleep(1000 * wait_ms);
	return 0;
}

int I2CReadByteFrom(int fd, int pos, int wait_ms)
{
	ioctl(fd, BLKFLSBUF); // clear kernel read buffer
	if (i2c_smbus_write_byte(fd, (unsigned char)pos) < 0)
	{
		errMsg("Fail to I2CReadByteFrom");
		return -1;
	}
	usleep(1000 * wait_ms);
	return i2c_smbus_read_byte(fd);
}

int I2CWriteByte(int fd, unsigned char byteData, int wait_ms)
{

	if (i2c_smbus_write_byte(fd, (unsigned char)byteData) < 0)
	{
		errMsg("Fail to I2CWriteByte");
		return -1;
	}
	usleep(1000 * wait_ms);
	return 0;
}

int I2CReadByte(int fd, int wait_ms)
{
	int ret;
	ioctl(fd, BLKFLSBUF); // clear kernel read buffer
	usleep(1000 * wait_ms);
	if ((ret = i2c_smbus_read_byte(fd)) == -1)
	{
		errMsg("Fail to I2CReadByte");
	}
	return ret;
}
