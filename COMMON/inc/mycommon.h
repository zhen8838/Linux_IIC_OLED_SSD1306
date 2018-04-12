#ifndef _MYCOMMON_H_
#define _MYCOMMON_H_


#define __DEBUG

#ifdef __DEBUG
#define DEBUG(format, args...) \
        printf("FAHW-Lib: " format, ## args)
#else
#define DEBUG(format, args...)
#endif

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/ip.h>
#include <linux/fs.h>

#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <semaphore.h>
#include <limits.h>

#include <netinet/in.h>

#include "tlpi_hdr.h"
#include "get_num.h"
#include "ename.c.inc"
#include "error_functions.h"
#include "alt_functions.h"





#endif // _COMMON_H_


