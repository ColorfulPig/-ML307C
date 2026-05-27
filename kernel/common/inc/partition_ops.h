#ifndef    _PARTITION_OPS_H_
#define    _PARTITION_OPS_H_

#include "qspi_flash.h"
#include "qspi_nor.h"
#include "spi_nor.h"
#include "Typedef.h"

/**********************************************
 * @ func   :  partition_ops_read
 * @ arg    :  pname    - the name of aboot rw/image partition
 *             offset   - the offset bytes from the partition base vaddr
 *             length   - length to read from partition
 *             buf_addr - target buffer pointer
 * @ desc   :  to load raw data from partition with <pname> index
 ***********************************************/
int partition_ops_read(const char * pname, 
                                        		uint32_t offset , 
                                        		uint32_t length , 
                                        		char * buf_addr);
		
		
int partition_ops_write(const char * pname, 
                                                    uint32_t offset , 
                                                    uint32_t length , 
                                                    char * buf_addr);

int partition_ops_erase(const char * pname, 
                                    		uint32_t offset , 
                                    		uint32_t length);

#endif


