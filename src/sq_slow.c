#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "sq.h"



/**
* file:  sq_slow.c
*
* description:  simple implementation of the 
*		service queue ADT as specified
*		in sq.h
*
*		(Believed to be) functionally correct,
*		but doe not meet all of the runtime
*		requirements specified in sq.h
*
*		Acts as a client of the linked list
*		ADT previously developed.
*/

struct service_queue {
    LIST * the_queue;
    LIST * buzzer_bucket;
};

/**
* Function: sq_create()
* Description: creates and intializes an empty service queue.
* 	It is returned as an SQ pointer.
*/
SQ * sq_create() {
SQ *q;

  q = malloc(sizeof(SQ));

  q->the_queue = lst_create();
  q->buzzer_bucket = lst_create();

  return q;
}

/**
* Function: sq_free()
* Description:  see sq.h
*
* RUNTIME REQUIREMENT:  O(N_t)  where N_t is the number of ticket
*	IDs that have been used during the lifetime of the
*	service queue; in general, at any particular instant
*	the actual queue length may be less than N_t.
*
*	[See discussion of "recycling tickets" below]
*
* RUNTIME ACHIEVED:  ???
*
*/
void sq_free(SQ *q) {

  lst_free(q->the_queue);
  lst_free(q->buzzer_bucket);

  free(q);
}

/**
* Function: sq_display()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(N)  (N is the current queue length).
* ACHIEVED RUNTIME:  O(N)  YAY!!
*/
void sq_display(SQ *q) {

  printf("current-queue contents:\n    ");
  lst_print(q->the_queue);
  printf("\n");
}

/**
* Function: sq_length()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int  sq_length(SQ *q) {
  return lst_length(q->the_queue);
}

/**
* Function: sq_give_buzzer()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)  ON AVERAGE or "AMORTIZED":  in other words, if 
*		there have been M calls to sq_give_buzzer, the total
*		time taken for those M calls is O(M).
*
*		An individual call may therefore not be O(1) so long
*		as when taken as a whole they average constant time.
*
*		(Hopefully this reminds you of an idea we employed in
*		the array-based implementation of the stack ADT).
*
* ACHIEVED RUNTIME:  ???
*/
int  sq_give_buzzer(SQ *q) {
int buzzer;

  if(!lst_is_empty(q->buzzer_bucket)) {
	buzzer = lst_pop_front(q->buzzer_bucket);
	lst_push_back(q->the_queue, buzzer);
	return buzzer;
  }
  else {
	/*  invariant:  
		if no re-useable buzzers, the buzzers 
		in the queue are {0,1,2,...,N-1} where
		N is the queue length.

		Thus, the smallest available new buzzer 
		is N
	*/
	buzzer = sq_length(q);
	lst_push_back(q->the_queue, buzzer);
	return buzzer;
  }
}

/**
* function: sq_seat()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_seat(SQ *q) {
int ticket;

	if(lst_is_empty(q->the_queue))
	  return -1;
	else{
	  ticket = lst_pop_front(q->the_queue);
	  lst_push_front(q->buzzer_bucket, ticket);
	  return ticket;
	}
} 



/**
* function: sq_kick_out()
*
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_kick_out(SQ *q, int buzzer) {

  if(lst_remove_first(q->the_queue, buzzer)) {
	lst_push_front(q->buzzer_bucket, buzzer);
	return 1;
  }
  else
	return 0;
}

/**
* function:  sq_take_bribe()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_take_bribe(SQ *q, int buzzer) {

  /* remove ticket then push it on front */
  if(lst_remove_first(q->the_queue, buzzer)) {
	lst_push_front(q->the_queue, buzzer);
	return 1;
  }
  else {
	/* person has to be in line to offer a bribe */
	return 0;
  }
}





