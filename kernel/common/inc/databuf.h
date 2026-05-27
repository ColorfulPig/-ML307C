#ifndef __DATA_BUF_H__
#define __DATA_BUF_H__

#include <inttypes.h>
#include "osa.h"
#include "csw_mem.h"
#include "global_types.h"

struct data_node
{
	struct data_node* next;
	struct data_node* prev;

	uint8_t *ptr;
	uint16_t length;

	uint16_t allocated_counter;
};

struct data_buffer
{
	uint16_t block_size;
	uint16_t block_num;

	uint8_t *mem_pool;

	struct data_node** data_nodes;

	struct data_node* current_node;
	struct data_node* free_list;
	
	struct data_node* data_list;

	OSASemaRef wait;
	OS_HISR ack_hisr;
};

uint8_t *data_buffer_get(struct data_buffer *buf, uint16_t size);
void data_buffer_put(struct data_buffer *buf, uint8_t *ptr);
struct data_node* data_buffer_get_block(struct data_buffer *buf);
void data_buffer_put_block(struct data_buffer *buf, struct data_node* node);
void data_buffer_init(struct data_buffer *buf, uint8_t *ptr, uint16_t block_size, uint16_t block_num);

#endif
