/*Prority Queue Implementation for a better packet control in the BLUE-voice sync aplication*/

/******************************************************************************
 * Include Files
******************************************************************************/
#include <hal_types.h>
#include "ble_queue.h"

#ifdef USE_STM32F4XX_NUCLEO
  #include "stm32f4xx_hal.h"
#endif /* USE_STM32F4XX_NUCLEO */

#ifdef USE_STM32L0XX_NUCLEO
#include "stm32l0xx_hal.h"
#endif /* USE_STM32L0XX_NUCLEO */

#ifdef USE_STM32L4XX_NUCLEO
#include "stm32l4xx_hal.h"
#endif /* USE_STM32L4XX_NUCLEO */

// DEBUG Macros
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif



// Helpful Macros
#define LEFT_CHILD(i)   ((i<<1)+1)
#define RIGHT_CHILD(i)  ((i<<1)+2)
#define PARENT_ENTRY(i) ((i-1)>>1)

#define GET_PRIORITY(root,indx) root->nodes[indx].priority 

#ifndef TRUE
#define TRUE 1
#endif
 #ifndef FALSE
#define FALSE 0
#endif

#define MAX_PRIORITY_LEVEL  (8)
#define MIN_PRIORITIY_LEVEL (1)


static int Get_min_priority_index(heap_t *root,int root_index){
	int min_index;
	int left_indx = LEFT_CHILD(root_index);
	int right_indx = RIGHT_CHILD(root_index);	
	if(left_indx > root->size)
		return -1;
	else if(right_indx > root->size)
		return left_indx;
	else if(GET_PRIORITY(root,left_indx)<=GET_PRIORITY(root,right_indx))
		return left_indx;
	else
		return right_indx;
}
/**
 * @brief initialized the priority queue.
 * @note This funtion can be used to initialized the priority queue
 * @return none.
 */

error_msg_t ary_init_queue (heap_t * root,int queue_size){
	if(queue_size > MAX_HEAP_SIZE){
		return NOT_ENOUGH_SPACE;
	}
	root->len = queue_size;
    root->size = 0;	
return OPT_SUCCESS;
}

/**
 * @brief check if the queue is empty.
 * @note This fuction check the queue size if it is diferent to 0 mean's that we have a new entry in queue
 * @return This funtion return TRUE if the queue is empty otherwise FALSE.
 */
uint8_t ary_queue_is_empty(heap_t * root){
	 uint32_t uwPRIMASK_Bit;
	 uwPRIMASK_Bit = __get_PRIMASK();  /**< backup PRIMASK bit */
	  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
	  if(root->size!=0){
		__set_PRIMASK(uwPRIMASK_Bit);     /**< Restore PRIMASK bit*/  
		return FALSE;
	  }
	 __set_PRIMASK(uwPRIMASK_Bit);     /**< Restore PRIMASK bit*/
	 return TRUE;
	  
}
/**
 * @brief check if the queue is FULL.
 * @note This fuction check if thequeue size equal to a HEAP_MAXIMUM
 * @return This funtion return TRUE if the queue is full otherwise FALSE.
 */
uint8_t ary_queue_is_full(heap_t * root){
	 uint32_t uwPRIMASK_Bit;
	 uwPRIMASK_Bit = __get_PRIMASK();  /**< backup PRIMASK bit */
	  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
	
	if(root->size = root->len){ /**/
		__set_PRIMASK(uwPRIMASK_Bit);     /**< Restore PRIMASK bit*/
		return TRUE;
	}	
	__set_PRIMASK(uwPRIMASK_Bit);     /**< Restore PRIMASK bit*/	
	return FALSE;

}

/**
 * @brief Return the size of the Queue.
 * @note simple checking of the queue size;
 * @return This funtion return TRUE if the queue is full otherwise FALSE.
 */
int ary_queue_get_size(heap_t * root){
	 uint32_t uwPRIMASK_Bit;
	 int size;
	 uwPRIMASK_Bit = __get_PRIMASK();  /**< backup PRIMASK bit */
	  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
	  size = root->size;
	  __set_PRIMASK(uwPRIMASK_Bit);     /**< Restore PRIMASK bit*/	
	  return size;
}
/**
 * @brief Insert a node according to their priority.
 * @note ;
 * @return This funtion return 0 if everything is ok.
 */
error_msg_t arry_queue_push (heap_t *root, int entry_priority, void * entry_data){/*insert a packet in the queue*/
	tClockTime  arrival_timestamp = Clock_Time();
	
	/*check it the priority*/
	if(entry_priority < MIN_PRIORITIY_LEVEL || entry_priority >= MAX_PRIORITY_LEVEL){
		return QUEUE_PRIORITY_ERROR;
	}
		
	/*insert entry*/	
	if((ary_queue_is_full(root))){
		/*check is the queue is full before to Insert*/
		return QUEUE_IS_FULL;
	}else if(ary_queue_is_empty(root)){
		/*first element of the queue*/
		root->nodes[0]->data = entry_data;
		root->nodes[0].priority = entry_priority;
		root->size+1;
	}else{
		/*inserting will be according to the index of the queue*/
		int position_index = root->size;
		root->size+1;
		while(position_index => 0){
			if(entry_priority < root->nodes[PARENT_ENTRY(position_index)]->priority){
				/*switch entry*/
				root->nodes[position_index].priority = root->nodes[PARENT_ENTRY(position_index)].priority;
				root->nodes[position_index]->data = root->nodes[PARENT_ENTRY(position_index)]->data;
				if(position_index==0)return OPT_SUCCESS;/*this is the high priority packect in top of the heap then return*/
				position_index = PARENT_ENTRY(position_index);
				
			}else{
				/*the entry is in a correct possition, then insert*/
				root->nodes[position_index].priority = entry_priority;
				root->nodes[position_index]->data = entry_data;
				return OPT_SUCCESS;
			}
		}
				
	}
	
}

/**
 * @brief extract the root packet.
 * @note it is possible to extract only the packet without modified the queue or a full extaction modifing the queue;
 * @return  This funtion return 0 if everything is ok..
 */

error_msg_t arry_queue_pop (heap_t *root,void * destination_pkt,int full_extract){ /*return the next element of the queue*/
	tClockTime  extraction_timestamp = Clock_Time(); /*I guess this is not necessary*/
	/*check if the queue is not empty*/
	if(ary_queue_is_empty(root)){
		return QUEUE_EMPTY;
	}
	/*extracting the next packet in the queue*/
	destination_pkt = root->nodes[0].data;
	
	if(full_extract){
		_index = 0;
		_last_index = root->size;
		root->nodes[0] = root->nodes[root->size-1];/*switch the last packet on top of the heap*/
		priority = root->nodes[0].priority;
		
		 while (Get_min_priority_index < _last_index) {
			 index_tmp = Get_min_priority_index(root, _index);
			 if(index_tmp = -1) break;
				 
			if(priority > GET_PRIORITY(root,index_tmp)){
				
				root->nodes[_index].priority = root->nodes[index_tmp].priority;
				root->nodes[_index].data = root->nodes[index_tmp].data;
				_index = index_tmp;
			}
		}
	/*end_sequence*/	
		
	}
	root->size -=1; 
	return OPT_SUCCESS;
	
}



#if DEBUG
void Test_queue(void){
	/*This function include a brute test script for this priority queue*/
	
}
#endif







