#ifndef PSM_WRAPPER_H
#define PSM_WRAPPER_H

#include <psm.h>
#include <string.h>
#include <errno.h>

#include "psm_module_def.h"
#include "duster_applets.h"
#include "FDI_TYPE.h"
#include "FDI_FILE.h"
//#include "csw_mrv_mem.h"

#include "fatwk_psm.h"

typedef enum handle_Type{
	handle_duster = 0,
#if PSM_PLATFORM_NEZHA_DEFINED
	handle_SMS1, //used, storage in DDR
	handle_phonebook, //used, storage in DDR
#ifdef SMS_ME_SUPPORT
	handle_SMS_ME, //used, storage in DDR
#endif
	handle_SMS5, //used
#endif
	total_handle_num,
}handle_Type;

typedef struct{
	char code;
	char *html;
}psm_iso8859_15;


extern psm_handle_t *g_handle[TOTAL_PSMFILE_NUM];
extern char * psmfile[TOTAL_PSMFILE_NUM];

extern psm_iso8859_15 psm_he[];

extern int psm_ncodes;

#ifdef MRVL_USE_POOL_MEM
#define duster_malloc(Size) DusterMalloc((Size))
#define duster_mem_check(ptr) mrv_memory_check(ptr)
#define duster_free(p) DusterFree(p)
#else
#ifndef duster_malloc
#define duster_malloc(Size) malloc((Size))
#endif
#ifndef duster_mem_check
#define duster_mem_check(ptr) 1
#endif
#ifndef duster_free
#define duster_free(p) free(p)
#endif
#endif

psm_handle_t * psm_open(char *module_name, char handleIndex);

int psm_eraseall__(char handleIndex);

void psm_set_module_name(psm_handle_t *handle, char *module_name);

#if PSM_PLATFORM_NEZHA_DEFINED

/* The set_version/get_version calls will be used by the individual
 * modules after upgrades. The modules will check the version of their
 * variables in the flash, and they will have routines to handle the
 * upgrades of variables based on their current version.
 */
int psm_set_version(psm_handle_t *handle, int version);

int psm_get_version(psm_handle_t *handle);

//for other psm file
int psmfile_set_wrapper(char *mod_name, char *subspace, char *variable, char *value, char handleIndex);

char *psmfile_get_wrapper(char *mod_name, char *subspace, char *variable, char handleIndex);
#endif

void psm_open__(char *mod_name, char *subspace);

//for default psm file
int psm_set_wrapper(char *mod_name, char *subspace, char *variable, char *value);

//for default psm file
char *psm_get_wrapper(char *mod_name, char *subspace, char *variable);

//for default psm file
int psm_commit__(void);

#if PSM_PLATFORM_NEZHA_DEFINED
//for default psm file
char *psm_get_wrapper_with_variable_name(char *mod_name,char *variable);

//for other psm file
int psm_commit_other__(char handleIndex);

int psm_match__(char *mod_name, char *subspace, char *variable, char *value);
#endif

static inline void *duster_realloc(void *p, size_t size)
{
	char* buff=NULL;
	if(size < strlen(p))
		size = strlen(p) + 1;
	if((buff = duster_malloc(size)) != NULL)
	{
		memset(buff,	0,	size);
		memcpy(buff,	p,	strlen(p));
		duster_free(p);
	}

	return buff;
}

static  char *psm_htmlentities(const char *str)
{
	char *buf;
	int siz, len, i = 0, j;

	siz = strlen(str) + 1;

	buf = (char *)duster_malloc(siz);
	ASSERT(buf);
	for (; *str; str++, i++)  {
		for (j = 0; j < psm_ncodes; j++) {
			if (*str == psm_he[j].code) {
				len = strlen(psm_he[j].html);
				buf = duster_realloc(buf, siz += len);
				ASSERT(buf);
				strcpy(buf + i, psm_he[j].html);
				i += len - 1;
				break;
			}
		}

		if (j == psm_ncodes)
			buf[i] = *str;
	}

	buf[i] = '\0';
	return buf;
}
static inline char *psm_HtmlToAscii(const char *str)
{
	char *buf;
	int siz, len, i = 0, j;

	siz = strlen(str) + 1;

	buf = (char *)duster_malloc(siz);
	ASSERT(buf);
	memset(buf,	0,	siz);

	for(;	*str;	i++)
		{
			for (j = 0; j < psm_ncodes; j++)
			{
				if (strncmp(str,	psm_he[j].html,	strlen(psm_he[j].html)) == 0)
				{
					CPUartLogPrintf("%s psm_he[%d] is %s",__FUNCTION__,j,psm_he[j].html);
					buf[i] = psm_he[j].code;
					CPUartLogPrintf("%s buf[%d] is 0x%x",__FUNCTION__,i,buf[i]);
					str += strlen(psm_he[j].html);
					break;
				}
			}
			if (j == psm_ncodes)
				{
					buf[i] = *str;
					str++;
				}
		}
	return buf;
}

static inline char* psm_strdup(char* str)
{
    int len;
    char* newstring=NULL;

    len = strlen( str ) + 1;
    newstring = (char*) duster_malloc(len);

    if( newstring != NULL )
	{
	    memset(newstring,0,len);
	    memcpy(newstring, str, len );
	    return newstring;
	}
    else
	    return NULL;
}

static inline void psm_handle_subspaces__(psm_handle_t *handle, char *subspace)
{
	char *ourstring = psm_strdup(subspace);
	char *cur, *p = ourstring;

	cur = p;
	while (*p != '\0') {
		if (*p == '.') {
			*p = '\0';
			p++;
			psm_push_subspace(handle, cur);
			cur = p;
		}
		p++;
	}
	psm_push_subspace(handle, cur); /* push the final subspace */
	duster_free(ourstring);
}

static inline void psm_open_internal(char *mod_name, char *subspace, char handleIndex)
{
	if (g_handle[handleIndex]==NULL) {  /* psm_get/psm_set is called the very first time*/

		PSM_Debug("%s: g_handle == NULL, handleIndex=0x%x", __FUNCTION__, handleIndex);

		if (mod_name == NULL) /* Use default namespace */
			mod_name = "default";

		g_handle[handleIndex] = psm_open(mod_name, handleIndex);
		if (g_handle[handleIndex] == NULL ){  /* Major hardware failure */
			CPUartLogPrintf("Unrecoverable hardware failure!");
			CPUartLogPrintf("%s, erase here",__FUNCTION__);
			psm_eraseall__(handleIndex);
			psm_commit(g_handle[handleIndex], handleIndex);
			return;
		}

	}  else {
		//fill env, just read content from flash for SMS5 file
	#if PSM_PLATFORM_NEZHA_DEFINED
		if(handleIndex == handle_SMS5)
			fill_env(NULL,g_handle[handleIndex]->psmh_backend_info, handleIndex);
    #endif
    
		if(subspace == NULL)
			PSM_Printf("%s 1: subspace == NULL",__FUNCTION__);


		if (mod_name == NULL) /* Use default namespace */
			mod_name = "default";
		psm_set_module_name(g_handle[handleIndex], mod_name);
	}

	g_handle[handleIndex]->psmh_subspacelen = 0; /* Reset subspace everytime psm_open_internal is called */
	if (subspace != NULL)
		psm_handle_subspaces__(g_handle[handleIndex], subspace);
	else
		PSM_Printf("%s 2: subspace == NULL",__FUNCTION__);
}

#endif
