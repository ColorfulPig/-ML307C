/****************************************************************************
 * Included Files
 ****************************************************************************/
 
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cm_ssl.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* 本源文件内代码用于版本兼容性，请勿进行修改 */

int cm_ssl_conn(void **cm_ssl_ctx_p, int ssl_id, int socket, int timeout)
{
    return cm_ssl_conn_with_host(cm_ssl_ctx_p, ssl_id, socket, timeout, NULL);
}
