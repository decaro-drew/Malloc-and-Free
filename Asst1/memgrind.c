
//MEMGRIND
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>



//**************************************
//Testcases A-F
//void functions to be called upon
//during the timing portion
//
//**************************************


void testA(){
  

    // Test A
    // malloc() 1 byte and immediately free it - do this 150 times

        printf("Test A\n");
        int i;
        char * p;
        for(i = 0; i < 150; i++){
            p = malloc(1);
            free(p);
        }

}

void testB(){

    // Test B
    // malloc() 1 byte, store the pointer in an array - do this 150 times.
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.

        printf("Test B\n");
        
        char * ptr;
        char * ptrs[150];

        int i;
        for(i = 0; i < 50; i++){ //first 50 chucks to be malloc()ed
            ptr = malloc(1);
            ptrs[i] = ptr;
            ptr = NULL;
        }
        int j;
        for(j = 0; j< 50; j++)
        {
	  free(ptrs[j]);      //free first 50
        }


        for(; i < 100; i++)   //next 50 bytes
        {
            ptr = malloc(1);
            ptrs[i] = ptr;
            ptr = NULL;

        }
        for(; j< 100; j++)
        {
	  free(ptrs[j]);     //freeing 50 more
        }


        for(; i < 150; i++)    //final 50
        {
            ptr = malloc(1);
            ptrs[i] = ptr;
            ptr = NULL;

        }
        for(; j< 150; j++)
        {
	  free(ptrs[j]);     //final free
        }



}


void testC(){

    // Test C
    // Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer 
    //     > do this until you have allocated 50 times
    // - Keep track of each operation so that you eventually malloc() 50 bytes, in total
    //     > if you have already allocated 50 times, disregard the random and just free() on each
    //     iteration
    // - Keep track of each operation so that you eventually free() all pointers
    //     > don't allow a free() if you have no pointers to free()
    
        printf("Test C\n");
        
        char * ptr;
        int allocCount = 0;//counting the malloc calls
        char * allocs[50];//capturing the malloc pointers
        int rando = 0;//variable for random
        int freeCount = 0;//counting the free calls
        srand(time(0));

        while(allocCount<50){//ensure 50 malloc calls
        
            rando = rand()%2;//generate a 1 or 0 at random to decide whether to malloc or free
            

            if(rando==1){
                //allocate
                ptr = malloc(1);
                allocs[allocCount] = ptr;//adds malloc pointer to array
                allocCount = allocCount +1;
                ptr = NULL;

            }else{
                

                if(freeCount>=allocCount){//nothing available to free
                    continue;//start again
                }else{
		  //Guarding against freeing an invalid input
		  meta* mp = (meta*)((char*)allocs[freeCount] -sizeof(ptr));
		  meta* new= allocs[freeCount];
		  if((mp->size == 0 || mp->size > 4096) || (new->flag == 1 && mp->flag == 0))
		    continue;
		  //Valid free
		  if(mp->flag ==0){
                    free(allocs[freeCount]);
                    freeCount = freeCount + 1;
		  }
		  else
		    continue;
                }
            }
        

        }
    
        //finish freeing the rest if necessary
        while(freeCount<50){
	    free(allocs[freeCount]);
            freeCount = freeCount + 1;
        }

}

void testD(){

    //Test D
    // Randomly choose between a randomly-sized malloc() or free()ing a pointer \u2013 do this many times (see below)
    // - Keep track of each malloc so that all mallocs do not exceed your total memory capacity 
    // - Keep track of each operation so that you eventually malloc() 50 times
    // - Keep track of each operation so that you eventually free() all pointers
    // - Choose a random allocation size between 1 and 64 bytes

        printf("Test D\n");//similar to Test C, reusing same format, using rand for size
        
        char * ptr;
        int allocCount = 0;//counting the malloc calls
        char * rallocs[50];//capturing the new random malloc pointers
        int rando = 0;//variable for random
        int freeCount = 0;//counting the free calls
        int sizeRand = 0;
        srand(time(0));
        int lower = 1;
        int upper = 64;
	int sum = 0;

        while(allocCount<50){//ensure 50 malloc calls
        
            rando = rand()%2;//generate a 1 or 0 at random to decide whether to malloc or free
            sizeRand = (rand() % (upper - lower + 1)) + lower; 
	    
	    //using the sum to make sure we're not going over 4096
	   if(sum >= 4096)
	    rando = 0;
            if(rando==1){
                //allocate
                ptr = malloc(sizeRand);
                rallocs[allocCount] = ptr;//adds malloc pointer to array
                allocCount = allocCount +1;
                ptr = NULL;
		//increment sum
		sum = sum + sizeRand;

            }else{
                if(freeCount>=allocCount){//nothing available to free
                    continue;//start again
                }else{

                  //Guarding against invalid inputs for free
		  meta* mp = (meta*)((char*)rallocs[freeCount] - sizeof(meta));
		  meta* new= rallocs[freeCount];
		  int x = mp->size;
		  if((mp->size == 0 || mp->size > 4096) || (new->flag == 1 && mp->flag == 0))
		    continue;

		  //Freeing, and decrementing sum
		  if(mp->flag == 0){
                    free(rallocs[freeCount]);
                    freeCount = freeCount + 1;
		     sum = sum - x;
		  }
                }
            }
        

        }
    
        //finish freeing the rest if necessary, while guarding against invalid inputs
        while(freeCount<50){
	   meta* mp = (meta*)((char*)rallocs[freeCount] - sizeof(meta));
	   meta* new= rallocs[freeCount];
	   if((mp->size == 0 || mp->size > 4096) || (new->flag == 1 && mp->flag == 0)){
	      freeCount = freeCount+1;
	       continue;
	    }
	   else
	      free(rallocs[freeCount]);

            freeCount = freeCount + 1;
        }

}

void testE(){

    //Test E (custom)
    //string "french fries" to malloc()ed random number of times
    //
    //french fries + null terminator is 13 bytes
    //
    //stored in a 2D array of [random][13] size
    //
    //every iteration of french fries will be free()d afterward
    //
    //random number range is from 1 to 100

    int i = 0;
    srand(time(0));
    int lower = 1; //lower bound for random number
    int upper = 100;//upper bound for random number
    int random = (rand() % (upper - lower + 1)) + lower; //placing random number in a valid range
    char ** box[random][13]; //array to store strings
    for(i = 0; i < random; i++){

            
            char *string = (char*)malloc(13);

            string[0] = 'f';
            string[1] = 'r';
            string[2] = 'e';
            string[3] = 'n';
            string[4] = 'c';
            string[5] = 'h';
            string[6] = ' ';
            string[7] = 'f';
            string[8] = 'r';
            string[9] = 'i';
            string[10] = 'e';
            string[11] = 's';
            string[12]= '\0';
            box[i][0]=string;

    }
    //freeing
    int j;
    for(j = 0; j < random; j++){

      free(box[j][0]);

    }
    
    

}

void testF(){
    
        printf("Test F\n");

        int i;
        for(i = 0; i<10; i++){
	  
	  //case designed to cause an error
            char *p = malloc(10);
            int x;
	    printf("this should print: a freeing a non-malloc pointer error:\n");
            free(&x);
	    
	    printf("\n");

            free(p);
          //case designed to cause an error
	    printf("we should see a redundant freeing error:\n");
            free(p);        
	    printf("\n");
        }
}





int main(int argc, char * argv[]){    

 
 
    /*Workload Stress Tests*/
    
   /*
    * Each test will be called upon and timed with the gettimeofday function
    * Each test time will be converted to microseconds cleaner results
    */

      
    int i;
    long int aTime=0;
    long int bTime=0;
    long int cTime=0;
    long int dTime=0;
    long int eTime=0;
    long int fTime=0;
    long int sec = 0;
    for(i = 0; i<100; i++){
    
        struct timeval begin, end;
        gettimeofday(&begin, NULL);
        testA();//TEST A
        gettimeofday(&end,NULL);
        sec = ((end.tv_sec*1000000)+end.tv_usec) - ((begin.tv_sec*1000000)+begin.tv_usec);
        printf("A instance %d: %ld microseconds.\n",i,sec);
        aTime = aTime+sec;

        sec = 0;
        
        struct timeval bBegin, bEnd;
        gettimeofday(&bBegin, NULL);
        testB();//TEST B
        gettimeofday(&bEnd,NULL);
        sec = ((bEnd.tv_sec*1000000)+bEnd.tv_usec) - ((bBegin.tv_sec*1000000)+bBegin.tv_usec);
        printf("B instance %d: %ld microseconds.\n",i,sec);
        bTime = bTime+sec;

        sec = 0;        
    
        struct timeval cBegin, cEnd;
        gettimeofday(&cBegin, NULL);
        testC();//TEST C
        gettimeofday(&cEnd,NULL);
        sec = ((cEnd.tv_sec*1000000)+cEnd.tv_usec) - ((cBegin.tv_sec*1000000)+cBegin.tv_usec);
        printf("C instance %d: %ld microseconds.\n",i,sec);
        cTime = cTime+sec;

        sec = 0;
	
        struct timeval dBegin, dEnd;
        gettimeofday(&dBegin, NULL);
        testD();//TEST D
        gettimeofday(&dEnd,NULL);
        sec = ((dEnd.tv_sec*1000000)+dEnd.tv_usec) - ((dBegin.tv_sec*1000000)+dBegin.tv_usec);
        printf("D instance %d: %ld microseconds.\n",i,sec);
        dTime = dTime+sec;

        sec = 0;
	
        struct timeval eBegin, eEnd;
        gettimeofday(&eBegin, NULL);
        testE();//TEST E
        gettimeofday(&eEnd,NULL);
        sec = ((eEnd.tv_sec*1000000)+eEnd.tv_usec) - ((eBegin.tv_sec*1000000)+eBegin.tv_usec);
        printf("E instance %d: %ld microseconds.\n",i,sec);
        eTime = eTime+sec;

        sec = 0;

        struct timeval fBegin, fEnd;
        gettimeofday(&fBegin, NULL);
        testF();//TEST F
        gettimeofday(&fEnd,NULL);
        sec = ((fEnd.tv_sec*1000000)+fEnd.tv_usec) - ((fBegin.tv_sec*1000000)+fBegin.tv_usec);
        printf("F instance %d: %ld microseconds.\n",i,sec);
        fTime = fTime+sec;

        

        

    }

    printf("total time for A: %ld microseconds\n",aTime);
    printf("Average timefor A: %ld microseconds\n",aTime/100);

    printf("total time for B: %ld microseconds\n",bTime);
    printf("Average time for B: %ld microseconds\n",bTime/100);

    printf("total time for C: %ld microseconds\n",cTime);
    printf("Average time for C: %ld microseconds\n",cTime/100);

    printf("total time for D: %ld microseconds\n",dTime);
    printf("Average time for D: %ld microseconds\n",dTime/100);

    printf("total time for E: %ld microseconds\n",eTime);
    printf("Average time for E: %ld microseconds\n",eTime/100);

    printf("total time for F: %ld microseconds\n",fTime);
    printf("Average time for F: %ld microseconds\n",fTime/100);

    

return 0;
}

