
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// very incomplete small test program...

int main() {
LIST *lst1;
int i;

  lst1 = lst_create();

  for(i=0; i<5; i++) {
     lst_push_front(lst1, i);
  }
  for(i=0; i<5; i++) {
     lst_push_back(lst1, i);
  }

  printf("lst_length(lst1) : %i\n", lst_length(lst1));
  printf("lst1 contents: ");
  lst_print(lst1);


  lst_pop_front(lst1);
  lst_pop_front(lst1);
  printf("lst1 contents after two pops: ");
  lst_print(lst1);



  /** TODO:  test code here! **/


  /** test code for lst_count **/

  printf("number of 3's = %i\n", lst_count(lst1, 3));
  printf("number of 0's = %i\n", lst_count(lst1, 0));


  /** test code for lst_print_rev **/





  /** test code for lst_push_back **/

  lst_free(lst1);
}
  
