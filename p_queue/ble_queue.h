/*Prority Queue Implementation for a better packet control in the BLUE-voice sync aplication*/

#ifndef _BLE_QUEUE_H_
#define _BLE_QUEUE_H_

#define MAX_HEAP_SIZE (5)

typedef struct {
    int priority;
	tClockTime  arraival_timestamp;
    void *data;
} node_t;


typedef struct {
	 node_t *nodes[MAX_HEAP_SIZE];
	int len;
    int size;
}heap_t;

typedef enum error_msg_t{OPT_SUCCESS=0,QUEUE_IS_FULL,QUEUE_EMPTY,QUEUE_PRIORITY_ERROR,NOT_ENOUGH_SPACE}error_msg_t;


void ary_init_queue (heap_t * root,int queue_size);/*Initialize the queue*/
uint8_t ary_queue_is_empty(heap_t * root);/*return true if the queue is empty*/
uint8_t ary_queue_is_full(heap_t * root); /*return true if the queue is full*/
int ary_queue_get_size(heap_t * root); /*return the size of the queue*/
error_msg_t arry_queue_push (heap_t *root, int priority);/*insert a packet in the queue*/
error_msg_t * arry_queue_pop(heap_t *root,void * destination_pkt,int full_extract); /*return the next element of the queue*/


#endif /* _BLE_QUEUE_H_ */
