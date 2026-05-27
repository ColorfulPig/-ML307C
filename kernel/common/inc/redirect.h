#ifndef __REDIRECT_H__
#define __REDIRECT_H__

#include "cc.h"

#if LWIP_REDIRECT

extern char * redirect_cmd;
extern int redirect_cmd_size;
extern char * redirect_host;
extern char * redirect_target_url;
extern int redirect_host_length;
extern char * redirect_subpath;
extern int redirect_subpath_length;
extern int redirect_enable;
extern int redirect_result_store;
extern int redirect_all_port;
#define MAX_REDIRECT_DNS 6
extern char * redirect_work_buf;
#define WORK_BUF_SIZE 2048

extern unsigned int http_processed_result;
#define HANDLE_GET				(1U<<0)
#define HANDLE_AGENT			(1U<<1)
#define HANDLE_HOST				(1U<<2)
#define HANDLE_REFERER		(1U<<3)
#define HANDLE_ALL				(HANDLE_GET | HANDLE_AGENT | HANDLE_HOST)
#define HANDLE_EXCLUSION	(HANDLE_REFERER)
#define HANDLE_FORWARD_EV	(HANDLE_GET | HANDLE_HOST)

struct redirect_port_map {
  unsigned short int original;
  unsigned short int redirected;
};

extern struct redirect_port_map port_redirect_map;

#define MAX_MIFI_CLIENT 36
#include "etharp.h"

struct client_status {
  unsigned int ip_address;
  struct eth_addr mac_address;
  int state; // Indication client be redirect or not.
  unsigned int time_ticks;//for notion wechat
  unsigned int wechat_scan;//for notion wechat
};

extern struct client_status redirect_status[MAX_MIFI_CLIENT];
extern unsigned short redirect_port[MAX_MIFI_CLIENT];
#define REDIRECT_STATUS_NULL            0
#define REDIRECT_STATUS_REDIRECTED      1
#define REDIRECT_STATUS_TEMP_REDIRECTED      2
#define REDIRECT_STATUS_ALWAYS_REDIRECTED    3



#define REDIRECT_WHITE_LIST_FILE        "rdirect.bin"

struct http_header_request_table {
  const char * tag;
  const char * format;
  const char * full;
  int (*callback) (struct http_header_request_table *, char *);
};

#define ELEMENT(TAG, FORMAT, CALLBACK) {TAG, FORMAT, TAG##FORMAT, CALLBACK}

struct white_mac {
  int last;
  struct eth_addr mac;
  int state;
};

#define WHITE_LIST_MAGIC1	0x48824021
#define WHITE_LIST_MAGIC2	0xacdfbec6

struct white_mac_list {
  unsigned int magic1;
  struct white_mac list[MAX_MIFI_CLIENT];
  int cnt;
  unsigned int magic2;
};

extern struct white_mac_list redirect_white_list;

// Send message to task, write record buffer to file
extern void update_white_list(void);

extern void notion_redirect_set_url(struct eth_addr *mac_address);

extern void reset_client(unsigned int client_ip);

// updtate record buffer
extern int update_white_list_struct(struct eth_addr *mac_address);

extern int parse_http_request(char **ppbuf, int *plen, struct http_header_request_table table[]);

extern struct http_header_request_table header_request_table[];

extern struct http_header_request_table notion_header_request_table[];


typedef int (*customer_redirect_end_func)(unsigned int ip_addr); // return 1 means stop redirect; 0 means keep redirect state

extern char * customer_redirect_reply_msg;

extern int customer_redirect_reply_msg_len;

extern customer_redirect_end_func customer_redirect_end;

extern int set_redirect_url(char * url);

struct dns_redirect {
	const char * name;
	unsigned int addr_list[MAX_REDIRECT_DNS];
};

extern struct dns_redirect * dns_white_list;

#endif
#endif
