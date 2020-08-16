#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef  int (*MyFunctionType)( int); 
typedef struct{int x;int y;} testing;
struct MyStruct {
     int   m_aNumber;
     float num2;
     double ch;
 };
 
 float KrazyFunction( struct MyStruct *parm1, int p1size, int bb )
 { 
   int ix; //declaring an integer variable//
   for (ix=0; ix<p1size; ix++) {
      if (parm1[ix].m_aNumber == bb )
          return parm1[ix].num2;
   }
   return 0.0f;
 }
static int Z = 110;

int sqr(int x) {
    return x*x;
}
int add(int x){
    return x+x;
}
  int *pointer_to_Z (int x) {        /* function returning integer pointer, not pointer to function */
    return &Z;
  }

  int get_Z (char x) {    
    return Z;                       
  }



int cal(int x,MyFunctionType cc){
    return cc(x);
}
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)))
int main(int argc, char** argv)
{
    
    
    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];
    printf("i array\n");
    printf("%p\n%p\n",iarray,iarray+1);
     printf("f array\n");
    printf("%p\n%p\n",farray,farray+1);
     printf("d array\n");
    printf("%p\n%p\n",darray,darray+1);
     printf("c array\n");
    printf("%p\n%p\n",carray,carray+1);
//    char buffer[10];
//    printf("%lu\n",NUM_ELEM(argv[0]));
//    printf("%lu\n",NUM_ELEM(buffer));


    // int* (*point)(int);
    // point=pointer_to_Z;
    // printf("%d\n",*point(123));
    // int (*function_pointer_to_Z)(int); /* proper pointer to function taking int as argument; */
    // function_pointer_to_Z = &add;
    // testing x={3,4};

    // printf("%d\n%d\n", x.x,x.y);

    // struct MyStruct myArray[4];
    // int pint[3];
    // pint[2]=123;
    //  #define MY_ARRAY_SIZE (sizeof(myArray)/sizeof(*myArray))
    // float v3;
    // struct MyStruct *secondArray;
    // int   someSize=10;
    // int   ix;
    // /* initialization of myArray ... */
    // v3 = KrazyFunction( myArray, MY_ARRAY_SIZE, 4 );
    // /* ... */
    // secondArray = calloc( someSize, sizeof(struct MyStruct) );
    // for (ix=0; ix<someSize; ix++) {
    //     secondArray[ix].m_aNumber = ix *2;
    //     secondArray[ix].num2 = .304 * ix * ix;
    // }
    // int array[10];
    // array[4]=123;
    // int idx=4;
    // printf("%lu\n",sizeof(array)/sizeof(int));
    // free(secondArray);

    
     return 0;
}
