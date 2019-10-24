#include <stdio.h>
#include <stdlib.h>

#ifndef _mymalloc_h__
#define _mymalloc_h__

#define malloc( x ) mymalloc( x , __FILE__ , __LINE__)
#define free( x ) myfree( x , __FILE__ , __LINE__)
static char array[4096];
typedef struct meta{

  int size;
  int flag;
  struct meta *next;

} meta;
//meta *list = (void*)array;

void* mymalloc(int size, char *file, int line);
void myfree(void *ptr, char* file, int line);
#endif
