#include <stdio.h>
#include <stdlib.h>
#include "sq.h"

typedef struct node {
  int id;
  struct node *next;
  struct node *prev;
} NODE;


struct service_queue {
  int n;
  int total;
  NODE *start;
  NODE *bucket;
  NODE **buzzer;
};

/**
* Function: sq_create()
* Description: creates and intializes an empty service queue.
* 	It is returned as an SQ pointer.
* 
* RUNTIME REQUIREMENT: O(1)
*/
SQ * sq_create() {
  int i;
  SQ *ret = malloc(sizeof(SQ));
  ret->n = 0;
  ret->total = 10;
  ret->buzzer = (NODE**)malloc(ret->total * sizeof(NODE*));
  ret->start = NULL;
  ret->bucket = NULL;
  for(i = 0; i < ret->total; i++) 
    ret->buzzer[i] = NULL;
  return ret;
}

/**
* Function: sq_free()
* Description:  deallocates all memory assciated
*   with service queue given by q.
*
* RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer 
*	IDs that have been used during the lifetime of the
*	service queue; in general, at any particular instant
*	the actual queue length may be less than N_b.
*
*	[See discussion of "re-using buzzers" below]
*
*/
void sq_free(SQ *q) {
  int i;
  //free all the nodes
  for(i = 0; i < q->total; i++)
    free((q->buzzer)[i]);
  free(q->buzzer);
  //now free the SQ
  free(q);
}

/**
* Function: sq_display()
* Description:  prints the buzzer IDs currently
*    in the queue from front to back.
*
* RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
*		length).
*/
void sq_display(SQ *q) {
  NODE *p = q->start;

  printf("[");
  while(p != NULL) {
    printf(" %i ", p->id);
    p = p->next;
  }
  printf("]\n");
}

/**
* Function: sq_length()
* Description:  returns the current number of
*    entries in the queue.
*
* RUNTIME REQUIREMENT:  O(1)
*/
int sq_length(SQ *q) {
  return q->n;
}

/**
* Function: sq_give_buzzer()
* Description:  This is the "enqueue" operation.  For us
*    a "buzzer" is represented by an integer (starting
*    from zero).  The function selects an available buzzer 
*    and places a new entry at the end of the service queue 
*    with the selected buzer-ID. 
*    The assigned buzzer-ID is a non-negative integer 
*    with the following properties:
*
*       (1) the buzzer (really it's ID) is not currently 
*         taken -- i.e., not in the queue.  (It
*         may have been in the queue at some previous
*         time -- i.e., buzzer can be re-used).
*	  This makes sense:  you can't give the same
*	  buzzer to two people!
*
*       (2) If there are buzzers that can be re-used, one
*         of those buzzers is used.  A re-usable buzzer is 
*	  a buzzer that _was_ in the queue at some previous
*	  time, but currently is not.
*
*       (3) if there are no previously-used buzzers, the smallest 
*         possible buzzer-ID is used (retrieved from inventory).  
*	  Properties in this situation (where N is the current
*	  queue length):
*
*		- The largest buzzer-ID used so far is N-1
*
*		- All buzzer-IDs in {0..N-1} are in the queue
*			(in some order).
*
*		- The next buzzer-ID (from the basement) is N.
*
*    In other words, you can always get more buzzers (from
*    the basement or something), but you don't fetch an
*    additional buzzer unless you have to.
*    you don't order new buzzers 
*
* Comments/Reminders:
*
*	Rule (3) implies that when we start from an empty queue,
*	the first buzzer-ID will be 0 (zero).
*
*	Rule (2) does NOT require that the _minimum_ reuseable 
*	buzzer-ID be used.  If there are multiple reuseable buzzers, 
*	any one of them will do.
*	
*	Note the following property:  if there are no re-useable 
*	buzzers, the queue contains all buzzers in {0..N-1} where
*       N is the current queue length (of course, the buzzer IDs 
*	may be in any order.)
*
* RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"  
		In other words, if there have been M calls to 
*		sq_give_buzzer, the total time taken for those 
*		M calls is O(M).
*
*		An individual call may therefore not be O(1) so long
*		as when taken as a whole they average constant time.
*
*		(Hopefully this reminds you of an idea we employed in
*		the array-based implementation of the stack ADT).
*/
void grow_buzzers(SQ *q) {
  int size = q->total * 2;
  NODE **tmp = malloc(size * sizeof(NODE*));
  int i;
  for(i = 0; i < size; i++) {
    if(i < q->total)
      tmp[i] = q->buzzer[i];
    else
      tmp[i] = NULL;
  }
  free(q->buzzer);
  q->buzzer = tmp;
  q->total = size;
}

int enqueue_buzzer(SQ *q, NODE *p) {
  if(p == NULL){
    printf("FAIL IN ENQUEUE\n");
    abort();
  }

  int ret = p->id;
  //enqueue the buzzer
  p->next = NULL;
  
  //take care of the property that prev of front = back
  if(q->start != NULL) {
    p->prev = q->start->prev;
    q->start->prev->next = p;
    q->start->prev = p;
    }
  else {
    p->prev = p;
    q->start = p;
  }
  //update service queue properties
  (q->n)++;
  return ret;
}

int sq_give_buzzer(SQ *q) {
  NODE *new = NULL;
  //check if there are any buzzers that can be "reused"
  if(q->bucket != NULL) {
    new = q->bucket;

    //pop from free stack
    if(q->bucket->next != NULL) {
      q->bucket = q->bucket->next;
    }
    else
      q->bucket = NULL;
  }
  else {
    if(q->n >= q->total) { //out of buzzers in the basket
      grow_buzzers(q);
    }
    new = malloc(sizeof(NODE));
    new->id = q->n;
    (q->buzzer)[q->n] = new;
  }
  return enqueue_buzzer(q, new);
}

/**
* function: sq_seat()
* description:  if the queue is non-empty, it removes the first 
*	 entry from (front of queue) and returns the 
*	 buzzer ID.
*	 Note that the returned buzzer can now be re-used.
*
*	 If the queue is empty (nobody to seat), -1 is returned to
*	 indicate this fact.
*
* RUNTIME REQUIREMENT:  O(1)
*/
int sq_seat(SQ *q){
  if(q->start == NULL) //queue is empty
    return -1;
  
  NODE *buzz = q->start;
  int ret = buzz->id;
  
  //dequeue the buzzer
  if(buzz->next != NULL)
    buzz->next->prev = buzz->prev;
  q->start = buzz->next;
  
  buzz->prev = NULL;
  buzz->next = q->bucket;
  q->bucket = buzz;
  (q->n)--;
  return ret;
}

/**
* function: sq_kick_out()
*
* description:  Some times buzzer holders cause trouble and
*		a bouncer needs to take back their buzzer and
*		tell them to get lost.
*
*		Specifially:
*
*		If the buzzer given by the 2nd parameter is 
*		in the queue, the buzzer is removed (and the
*		buzzer can now be re-used) and 1 (one) is
*		returned (indicating success).
*
*		If the buzzer isn't actually currently in the
*		queue, the queue is unchanged and 0 is returned
*		(indicating failure).
*
* RUNTIME REQUIREMENT:  O(1)
*/
int sq_kick_out(SQ *q, int buzzer) {
  if( buzzer >= q->total || ((q->buzzer)[buzzer]) == NULL || ((q->buzzer)[buzzer])->prev == NULL) 
    return 0;
  
  NODE *buzz = (q->buzzer)[buzzer];
  
  if(q->start == buzz) {
    sq_seat(q);
    return 1;
  }
  
  if(buzz->next == NULL) {
    q->start->prev = buzz->prev;
    buzz->prev->next = NULL;
  }
  else {
    buzz->next->prev = buzz->prev;
    buzz->prev->next = buzz->next;
  }
  
  buzz->prev = NULL;
  buzz->next = q->bucket;
  q->bucket = buzz;
  (q->n)--;

  return 1;
}

/**
* function:  sq_take_bribe()
* description:  some people just don't think the rules of everyday
*		life apply to them!  They always want to be at
*		the front of the line and don't mind bribing
*		a bouncer to get there.
*
*	        In terms of the function:
*
*		  - if the given buzzer is in the queue, it is 
*		    moved from its current position to the front
*		    of the queue.  1 is returned indicating success
*		    of the operation.
*		  - if the buzzer is not in the queue, the queue 
*		    is unchanged and 0 is returned (operation failed).
*
* RUNTIME REQUIREMENT:  O(1)
*/
int sq_take_bribe(SQ *q, int buzzer) {
  if( buzzer >= q->total || ((q->buzzer)[buzzer]) == NULL || ((q->buzzer)[buzzer])->prev == NULL) 
    return 0;
  
  NODE *buzz = (q->buzzer)[buzzer];
  if(q->start == buzz)
    return 1;
  if(q->start->prev == buzz) {
    buzz->prev->next = NULL;
    buzz->next = q->start;
    q->start = buzz;
    return 1;
  }
  else {
    buzz->next->prev = buzz->prev;
    buzz->prev->next = buzz->next;
    buzz->prev = q->start->prev;
    buzz->next = q->start;
    q->start = buzz;
    return 1;
  }
}


