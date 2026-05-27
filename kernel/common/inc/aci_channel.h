#ifndef __ACI_CHANNEL_H__
#define __ACI_CHANNEL_H__

struct ACIChannel
{
	/* channel name and id */
	char name[ACI_CHANNEL_NAME_MAX];
	int id;

    struct ACIChannelDesc desc;
	struct ACIChannelOps* ops;

	UINT32 LastTxTick;
	UINT32 LastRxTick;
	UINT32 LastBlockTick;
	UINT32 LastResumeTick;

	UINT32 UserData;
};

/* callback interface, which invoked by channel interface */
int ACIChannelRxCB(struct ACIChannel *channel, void* ptr, int size);
void ACIChannelTxDoneCB(struct ACIChannel *channel, void* ptr);
void ACIChannelLinkStatusIndCB(struct ACIChannel *channel, int event);
void ACIChannelSetUserData(struct ACIChannel *channel, UINT32 data);
UINT32 ACIChannelGetUserData(struct ACIChannel *channel);

#endif

