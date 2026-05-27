#ifndef __ACI_H__
#define __ACI_H__

#define ACI_CHANNEL_MAX     	32
#define ACI_CHANNEL_NAME_MAX    32

enum 
{
	ACI_EVENT_BLOCKED,
	ACI_EVENT_RESUMED,
	ACI_EVENT_LINKUP,
	ACI_EVENT_LINKDOWN,
};

enum 
{
	ACI_RC_OK, 
	ACI_RC_ERROR,
	ACI_RC_FULL,
	ACI_RC_HWM,
	ACI_RC_CLOSE_ERROR
};

struct ACIChannel;

/* description of ACI channel */
struct ACIChannelDesc
{
    int HWM, LWM;
	int Priority;

    void (*RxIndication)(struct ACIChannel *channel, void *ptr, int size);
    void (*TxComplete)  (struct ACIChannel *channel, void *ptr);

	void (*LinkStatusIndication)(struct ACIChannel *channel, int event);
};

struct ACIChannelOps
{
	int  (*Open) (struct ACIChannel *channel);
    int  (*TxReq)(struct ACIChannel *channel, void* ptr, int size);
	void (*RxAck)(struct ACIChannel *channel, void* ptr);
	int  (*Close)(struct ACIChannel *channel);
};

struct ACIChannelId
{
    char* name;
    int   id;
};

int ACIRegisterChannels(const struct ACIChannelId* ids, int id_num, const struct ACIChannelOps* ops);

/* ACI interface for upper layer application */
struct ACIChannel *ACIOpen(const char *name, struct ACIChannelDesc *desc);
int ACIWrite(struct ACIChannel *channel, void* ptr, int size);
void ACIClose(struct ACIChannel *channel);
void ACIRxAck(struct ACIChannel *channel, void* ptr);
struct ACIChannel *ACIGetChannel(const char *name);
struct ACIChannel *ACIChannelGetById(int id);

#endif
