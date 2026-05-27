#ifndef __IMS_API_PSRAM__
#define __IMS_API_PSRAM__
#include "sockets.h"
#include "diag.h"
#include <teldef.h>

typedef void (*IMSThread_Func) (void *argv);

struct IMSThread
{
    IMSThread_Func Func;
    OSATaskRef  thread;
    void *param;
};

char GetImsEnable(int sim_id);
void ims_set_cid(int sim_id, int cid);
int ims_get_cid(int sim_id);
void ims_thread_entry(void * argv);
void ims_task_suspend(void);
void ims_task_resume(void);
void restartImsTask(char *file, int line);
void DoRestartImsTask(void);
void ims_client_task_entry(void);
void setATSocketValid(int simID, bool isValid);
int imsGetATChannel(int simID);

#endif

