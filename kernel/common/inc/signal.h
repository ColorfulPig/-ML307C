/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _POSIX_SIGNAL_H_
#define _POSIX_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#define SIGEV_NONE   0
#define SIGEV_SIGNAL 1
#define SIGEV_THREAD 2

union sigval {
    int   sival_int;
    void *sival_ptr;
};

struct sigevent {
    int             sigev_notify;
    int             sigev_signo;
    union sigval    sigev_value;
    void           *sigev_notify_attributes;
    void (*sigev_notify_function)(union sigval);
};


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _POSIX_SIGNAL_H_ */

