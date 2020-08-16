#include <stdio.h>
#include<stdlib.h>
#include <string.h>

/* this is the type of object we have, with a single int member */
typedef struct WIDGET_T {
  int member;
} WIDGET_T;

/* functions that deal with WIDGET_T */

/* constructor function */
void WIDGETctor (WIDGET_T *this, int x)
{
  this->member = x;
}

/* destructor function */
void WIDGETdtor (WIDGET_T *this)
{
  /* In this case, I really don't have to do anything, but
     if WIDGET_T had internal pointers, the objects they point to
     would be destroyed here.  */
  this->member = 0;
}

/* create function - this function returns a new WIDGET_T */
WIDGET_T * WIDGETcreate (int m)
{
  WIDGET_T *x = 0;

  x = malloc (sizeof (WIDGET_T));
  if (x == 0)
    abort (); /* no memory */
  WIDGETctor (x, m);
  return x;
}

/* destroy function - calls the destructor, then frees the object */
void
WIDGETdestroy (WIDGET_T *this)
{
  WIDGETdtor (this);
  free (this);
}

/* END OF CODE */



 struct mystruct
{
    int int_member;
    double double_member;
    struct mystruct *next;

};
struct mystruct x;



union myunion{
    int x;
    int y;
};


int BUFFERSIZE=2;

int main()
{

  

  // enum colors{
  //     red,
  //     blue,
  //     yellow,
  //     black,
  //     white,
  //     green,
  //     brown,
  //     grey
  // }color=red;
  // x.int_member=1;
  // struct mystruct* y=&x;
  // struct mystruct mn;
  // x.next=&mn;
  // mn.int_member=123;
  // y->double_member=22.3;

  // union myunion w;
  // w.x=10;
  // w.y=123;

  // printf("%u",color);
  // printf("%d\n%d\n",w.x,w.y);
  // int* x= calloc(3,sizeof(int));
}

