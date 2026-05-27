/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _POSIX_TIMER_H_
#define _POSIX_TIMER_H_

#include <stdint.h>
#include "signal.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POSIX_TIMER_TICKS_PER_SECOND        (1000/OsaGetClockRate(NULL))



#define NANOSECONDS_PER_SECOND       1000000000LL
#define MICROSECONDS_PER_SECOND      1000000LL
#define MILLISECONDS_PER_SECOND      1000LL
#define NANOSECONDS_PER_MICROSECONDS 1000000LL


#define  TIMER_ABSTIME  1


/**
 * System-wide realtime clock, can jump forwards and backwards as the system
 * time-of-day clock is changed
 */
#define CLOCK_REALTIME  (clockid_t)1

/**
 * The absolute elapsed wall-clock time since some arbitrary, fixed point in the past.
 * It isn't affected by changes in the system time-of-day clock
 */
#define CLOCK_MONOTONIC (clockid_t)2

#define POSIX_TIMER_ID_MIN 1



typedef void * timer_t;
typedef uint32_t clockid_t;
typedef uint64_t tick_t;
typedef uint32_t useconds_t;


struct timespec {
   time_t tv_sec;                /* Seconds */
   long   tv_nsec;               /* Nanoseconds */
};

struct itimerspec {
   struct timespec it_interval;  /* Timer interval */
   struct timespec it_value;     /* Initial expiration */
};

struct timezone {
  int tz_minuteswest;   /* minutes west of Greenwich */
  int tz_dsttime;       /* type of dst correction */
};

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
    long    tv_sec;     /* seconds */
    long    tv_usec;    /* and microseconds */
};

int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue);
int timer_gettime(timer_t timerid, struct itimerspec *value);
int timer_getoverrun(timer_t timerid);

int clock_getres(clockid_t clock_id, struct timespec *res);
int clock_gettime(clockid_t clock_id, struct timespec *tp);
int clock_settime(clockid_t clock_id, const struct timespec *tp);
int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp);

unsigned int sleep(unsigned int seconds);
int          usleep(useconds_t us);
int          nanosleep(const struct timespec *rqtp, struct timespec *rmtp);

/* utils */
int timespec_abs_to_ticks(clock_t clock, const struct timespec *abstime, tick_t *ticks_relate);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _POSIX_TIMER_H_ */

