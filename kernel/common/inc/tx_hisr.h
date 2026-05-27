#ifdef PLAT_USE_THREADX

#ifndef HISR_H_
#define HISR_H_
#include<tx_api.h>

typedef struct TX_HISR_STRUCT{

	TX_THREAD  			tx_hisr_thread;					//note: should not be a pointer
	TX_SEMAPHORE		tx_hisr_semaphore;		////note: should not be a pointer
	VOID                (*tx_hisr_entry)(ULONG);
	ULONG               tx_hisr_entry_parameter;
	char				*tx_hisr_stack_start; 
	UINT				reserved;

}TX_HISR;


UINT _tx_hisr_create(TX_HISR *hisr_ptr, CHAR * name,VOID(* entry_function)(ULONG),
				ULONG entry_input,UINT priority);
UINT _tx_hisr_delete(TX_HISR *hisr_ptr);
UINT _tx_hisr_activate(TX_HISR *hisr_ptr);
void hisr_thread(ULONG entry_input);


#endif /*HISR_H_*/

#endif
