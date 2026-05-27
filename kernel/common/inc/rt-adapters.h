
#ifndef _RT_ADAPTERS_H_
#define _RT_ADAPTERS_H_

#include <stdint.h>
#include <time.h>
#include "osa.h"
#include "dfs_asr_config.h"
#include "posix_log.h"

/* boolean type definitions */
#define RT_TRUE                         1               /**< boolean true  */
#define RT_FALSE                        0               /**< boolean fails */


/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */
#define RT_EINTR                        9               /**< Interrupted system call */
#define RT_EINVAL                       10              /**< Invalid argument */


#define RT_NULL                         (0)


/**
 * IPC flags and control command definitions
 */
#define RT_IPC_FLAG_FIFO                0x00            /**< FIFOed IPC. @ref IPC. */
#define RT_IPC_FLAG_PRIO                0x01            /**< PRIOed IPC. @ref IPC. */

#define RT_WAITING_FOREVER              -1              /**< Block forever until get resource. */
#define RT_WAITING_NO                   0               /**< Non-block. */


#define RTM_EXPORT(x)
#define FINSH_FUNCTION_EXPORT(name, desc)



#define S_IFMT               00170000
#define S_IFSOCK             0140000
#define S_IFLNK              0120000
#define S_IFREG              0100000
#define S_IFBLK              0060000
#define S_IFDIR              0040000
#define S_IFCHR              0020000
#define S_IFIFO              0010000
#define S_ISUID              0004000
#define S_ISGID              0002000
#define S_ISVTX              0001000

#define S_ISLNK(m)           (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)           (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)           (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)           (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)           (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)          (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)          (((m) & S_IFMT) == S_IFSOCK)

#ifndef S_IRWXU
#define S_IRWXU              00700
#endif

#ifndef S_IRUSR
#define S_IRUSR              00400
#endif

#ifndef S_IWUSR
#define S_IWUSR              00200
#endif

#ifndef S_IXUSR
#define S_IXUSR              00100
#endif

#ifndef S_IRWXG
#define S_IRWXG              00070
#endif

#ifndef S_IRGRP
#define S_IRGRP              00040
#endif

#ifndef S_IWGRP
#define S_IWGRP              00020
#endif

#ifndef S_IXGRP
#define S_IXGRP              00010
#endif

#ifndef S_IRWXO
#define S_IRWXO              00007
#endif

#ifndef S_IROTH
#define S_IROTH              00004
#endif

#ifndef S_IWOTH
#define S_IWOTH              00002
#endif

#ifndef S_IXOTH
#define S_IXOTH              00001
#endif



#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_BINARY    0100000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE 0100000
#define O_NOATIME  01000000
#define O_PATH    010000000
#define O_TMPFILE 020200000
#define O_NDELAY O_NONBLOCK

#define O_SEARCH  O_PATH
#define O_EXEC    O_PATH

#define O_ACCMODE (03|O_SEARCH)

#define F_DUPFD  0
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4

#define F_SETOWN 8
#define F_GETOWN 9
#define F_SETSIG 10
#define F_GETSIG 11

#define F_GETLK 12
#define F_SETLK 13
#define F_SETLKW 14

#define F_SETOWN_EX 15
#define F_GETOWN_EX 16

#define F_GETOWNER_UIDS 17



#define INIT_PREV_EXPORT(fn)        
#define INIT_DEVICE_EXPORT(fn) 
#define INIT_COMPONENT_EXPORT(fn)
#define INIT_ENV_EXPORT(fn)
#define INIT_APP_EXPORT(fn)        



#define ERROR_BASE_NO    200

#define EPERM            (ERROR_BASE_NO + 1)
#define ENOENT           (ERROR_BASE_NO + 2)
#define ESRCH            (ERROR_BASE_NO + 3)
#define EINTR            (ERROR_BASE_NO + 4)
#define EIO              (ERROR_BASE_NO + 5)
#define ENXIO            (ERROR_BASE_NO + 6)
#define E2BIG            (ERROR_BASE_NO + 7)
#define ENOEXEC          (ERROR_BASE_NO + 8)
#define EBADF            (ERROR_BASE_NO + 9)
#define ECHILD          (ERROR_BASE_NO + 10)
#define EAGAIN          (ERROR_BASE_NO + 11)

#ifndef ENOMEM
#define ENOMEM          (ERROR_BASE_NO + 12)
#endif

#define EACCES          (ERROR_BASE_NO + 13)
#define EFAULT          (ERROR_BASE_NO + 14)
#define ENOTBLK         (ERROR_BASE_NO + 15)
#define EBUSY           (ERROR_BASE_NO + 16)
#define EEXIST          (ERROR_BASE_NO + 17)
#define EXDEV           (ERROR_BASE_NO + 18)
#define ENODEV          (ERROR_BASE_NO + 19)
#define ENOTDIR         (ERROR_BASE_NO + 20)
#define EISDIR          (ERROR_BASE_NO + 21)

#ifndef EINVAL
#define EINVAL          (ERROR_BASE_NO + 22)
#endif

#define ENFILE          (ERROR_BASE_NO + 23)
#define EMFILE          (ERROR_BASE_NO + 24)
#define ENOTTY          (ERROR_BASE_NO + 25)
#define ETXTBSY         (ERROR_BASE_NO + 26)
#define EFBIG           (ERROR_BASE_NO + 27)
#define ENOSPC          (ERROR_BASE_NO + 28)
#define ESPIPE          (ERROR_BASE_NO + 29)
#define EROFS           (ERROR_BASE_NO + 30)
#define EMLINK          (ERROR_BASE_NO + 31)
#define EPIPE           (ERROR_BASE_NO + 32)

#ifndef EDOM
#define EDOM            (ERROR_BASE_NO + 33)
#endif

#ifndef ERANGE
#define ERANGE          (ERROR_BASE_NO + 34)
#endif

#define EDEADLK         (ERROR_BASE_NO + 35)
#define ENAMETOOLONG    (ERROR_BASE_NO + 36)
#define ENOLCK          (ERROR_BASE_NO + 37)
#define ENOSYS          (ERROR_BASE_NO + 38)
#define ENOTEMPTY       (ERROR_BASE_NO + 39)
#define ELOOP           (ERROR_BASE_NO + 40)
#define EWOULDBLOCK     EAGAIN
#define ENOMSG          (ERROR_BASE_NO + 42)
#define EIDRM           (ERROR_BASE_NO + 43)
#define ECHRNG          (ERROR_BASE_NO + 44)
#define EL2NSYNC        (ERROR_BASE_NO + 45)
#define EL3HLT          (ERROR_BASE_NO + 46)
#define EL3RST          (ERROR_BASE_NO + 47)
#define ELNRNG          (ERROR_BASE_NO + 48)
#define EUNATCH         (ERROR_BASE_NO + 49)
#define ENOCSI          (ERROR_BASE_NO + 50)
#define EL2HLT          (ERROR_BASE_NO + 51)
#define EBADE           (ERROR_BASE_NO + 52)
#define EBADR           (ERROR_BASE_NO + 53)
#define EXFULL          (ERROR_BASE_NO + 54)
#define ENOANO          (ERROR_BASE_NO + 55)
#define EBADRQC         (ERROR_BASE_NO + 56)
#define EBADSLT         (ERROR_BASE_NO + 57)
#define EDEADLOCK       EDEADLK
#define EBFONT          (ERROR_BASE_NO + 59)
#define ENOSTR          (ERROR_BASE_NO + 60)
#define ENODATA         (ERROR_BASE_NO + 61)
#define ETIME           (ERROR_BASE_NO + 62)
#define ENOSR           (ERROR_BASE_NO + 63)
#define ENONET          (ERROR_BASE_NO + 64)
#define ENOPKG          (ERROR_BASE_NO + 65)
#define EREMOTE         (ERROR_BASE_NO + 66)
#define ENOLINK         (ERROR_BASE_NO + 67)
#define EADV            (ERROR_BASE_NO + 68)
#define ESRMNT          (ERROR_BASE_NO + 69)
#define ECOMM           (ERROR_BASE_NO + 70)
#define EPROTO          (ERROR_BASE_NO + 71)
#define EMULTIHOP       (ERROR_BASE_NO + 72)
#define EDOTDOT         (ERROR_BASE_NO + 73)
#define EBADMSG         (ERROR_BASE_NO + 74)
#define EOVERFLOW       (ERROR_BASE_NO + 75)
#define ENOTUNIQ        (ERROR_BASE_NO + 76)
#define EBADFD          (ERROR_BASE_NO + 77)
#define EREMCHG         (ERROR_BASE_NO + 78)
#define ELIBACC         (ERROR_BASE_NO + 79)
#define ELIBBAD         (ERROR_BASE_NO + 80)
#define ELIBSCN         (ERROR_BASE_NO + 81)
#define ELIBMAX         (ERROR_BASE_NO + 82)
#define ELIBEXEC        (ERROR_BASE_NO + 83)

#ifndef EILSEQ
#define EILSEQ          (ERROR_BASE_NO + 84)
#endif

#define ERESTART        (ERROR_BASE_NO + 85)
#define ESTRPIPE        (ERROR_BASE_NO + 86)
#define EUSERS          (ERROR_BASE_NO + 87)
#define ENOTSOCK        (ERROR_BASE_NO + 88)
#define EDESTADDRREQ    (ERROR_BASE_NO + 89)
#define EMSGSIZE        (ERROR_BASE_NO + 90)
#define EPROTOTYPE      (ERROR_BASE_NO + 91)
#define ENOPROTOOPT     (ERROR_BASE_NO + 92)
#define EPROTONOSUPPORT (ERROR_BASE_NO + 93)
#define ESOCKTNOSUPPORT (ERROR_BASE_NO + 94)
#define EOPNOTSUPP      (ERROR_BASE_NO + 95)
#define ENOTSUP         EOPNOTSUPP         
#define EPFNOSUPPORT    (ERROR_BASE_NO + 96)
#define EAFNOSUPPORT    (ERROR_BASE_NO + 97)
#define EADDRINUSE      (ERROR_BASE_NO + 98)
#define EADDRNOTAVAIL   (ERROR_BASE_NO + 99)
#define ENETDOWN        (ERROR_BASE_NO + 100)
#define ENETUNREACH     (ERROR_BASE_NO + 101)
#define ENETRESET       (ERROR_BASE_NO + 102)
#define ECONNABORTED    (ERROR_BASE_NO + 103)
#define ECONNRESET      (ERROR_BASE_NO + 104)
#define ENOBUFS         (ERROR_BASE_NO + 105)
#define EISCONN         (ERROR_BASE_NO + 106)
#define ENOTCONN        (ERROR_BASE_NO + 107)
#define ESHUTDOWN       (ERROR_BASE_NO + 108)
#define ETOOMANYREFS    (ERROR_BASE_NO + 109)
#define ETIMEDOUT       (ERROR_BASE_NO + 110)
#define ECONNREFUSED    (ERROR_BASE_NO + 111)
#define EHOSTDOWN       (ERROR_BASE_NO + 112)
#define EHOSTUNREACH    (ERROR_BASE_NO + 113)
#define EALREADY        (ERROR_BASE_NO + 114)
#define EINPROGRESS     (ERROR_BASE_NO + 115)
#define ESTALE          (ERROR_BASE_NO + 116)
#define EUCLEAN         (ERROR_BASE_NO + 117)
#define ENOTNAM         (ERROR_BASE_NO + 118)
#define ENAVAIL         (ERROR_BASE_NO + 119)
#define EISNAM          (ERROR_BASE_NO + 120)
#define EREMOTEIO       (ERROR_BASE_NO + 121)
#define EDQUOT          (ERROR_BASE_NO + 122)
#define ENOMEDIUM       (ERROR_BASE_NO + 123)
#define EMEDIUMTYPE     (ERROR_BASE_NO + 124)
#define ECANCELED       (ERROR_BASE_NO + 125)
#define ENOKEY          (ERROR_BASE_NO + 126)
#define EKEYEXPIRED     (ERROR_BASE_NO + 127)
#define EKEYREVOKED     (ERROR_BASE_NO + 128)
#define EKEYREJECTED    (ERROR_BASE_NO + 129)
#define EOWNERDEAD      (ERROR_BASE_NO + 130)
#define ENOTRECOVERABLE (ERROR_BASE_NO + 131)
#define ERFKILL         (ERROR_BASE_NO + 132)
#define EHWPOISON       (ERROR_BASE_NO + 133)


#define RT_DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define RT_DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define RT_DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define RT_DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define RT_DEVICE_OFLAG_OPEN            0x008           /**< device is opened */
#define RT_DEVICE_OFLAG_MASK            0xf0f           /**< mask of open flag */



#define DT_UNKNOWN		0x00
#define DT_REG			0x01
#define DT_DIR			0x02



#define rt_inline                   static __inline
#define ALIGN               __align
#define RT_WEAK


typedef unsigned char 					rt_bool_t;
typedef long          					rt_err_t;
typedef signed   char                   rt_int8_t;      /**<  8bit integer type */
typedef signed   short                  rt_int16_t;     /**< 16bit integer type */
typedef signed   int                    rt_int32_t;     /**< 32bit integer type */
typedef unsigned char                   rt_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  rt_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned int                    rt_uint32_t;    /**< 32bit unsigned integer type */

typedef long                            rt_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU related data type */
typedef rt_uint32_t                     rt_time_t;      /**< Type for time stamp */
typedef rt_uint32_t                     rt_tick_t;      /**< Type for tick count */
typedef rt_ubase_t                      rt_size_t;      /**< Type for size number */
typedef rt_base_t                       rt_off_t;       /**< Type for offset */


typedef signed long off_t;
typedef int mode_t;



struct rt_mutex { 
	OsaRefT ref;
};
typedef struct rt_mutex *rt_mutex_t;


struct rt_semaphore
{
    OsaRefT ref;
};
typedef struct rt_semaphore *rt_sem_t;


/**
 * Device structure
 */
struct rt_device {
	const char *dev_name;
	const char *drive_name;
};
typedef struct rt_device *rt_device_t;


struct stat
{
    struct rt_device *st_dev;
    uint16_t  st_ino;
    uint16_t  st_mode;
    uint16_t  st_nlink;
    uint16_t  st_uid;
    uint16_t  st_gid;
    struct rt_device *st_rdev;
    uint32_t  st_size;
    time_t    st_atime;
    long      st_spare1;
    time_t    st_mtime;
    long      st_spare2;
    time_t    st_ctime;
    long      st_spare3;
    uint32_t  st_blksize;
    uint32_t  st_blocks;
    long      st_spare4[2];
};


extern void *rt_malloc(unsigned long size);
extern void rt_free(void *rmem);
extern void *rt_calloc(rt_size_t count, rt_size_t size);
extern void *rt_realloc(void *rmem, rt_size_t newsize);


extern rt_int32_t rt_snprintf(char *buf, rt_size_t size, const char *fmt, ...);
extern char *rt_strdup(const char *s);
extern rt_err_t rt_mutex_init(rt_mutex_t mutex, const char *name, rt_uint8_t flag);
extern rt_err_t rt_mutex_take(rt_mutex_t mutex, rt_int32_t time);
extern rt_err_t rt_mutex_release(rt_mutex_t mutex);


extern void rt_set_errno(rt_err_t error);
extern rt_err_t rt_get_errno(void);
extern rt_device_t rt_device_find(const char *name);
extern rt_err_t rt_device_open(rt_device_t dev, rt_uint16_t oflag);
extern rt_err_t rt_device_close(rt_device_t dev);

#endif  // _RT_ADAPTERS_H_

