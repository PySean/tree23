#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tree23.h"

/*
 * "main.c", by Sean Soderman
 * Simply tests the 2-3 tree functions I've implemented.
 */
//Debug printing function that exposes all values within a node.
//TODO: Add in ability to add random values and remove n of them,
//where n is the number of random values added (of course). Perhaps
//do a time benchmark too.
void nodecheck(node * n);
//Runs a standard test of the program using 100,000 
//randomised insertions/deletions.
void standard_test();
//Runs a customised test specified by the user.
void custom_test(uint64_t num_to_insert, char * filename);

int main(int argc, char * argv[]) {
   if (argc < 2) {
      fprintf(stderr, "No options specified. Will run standard test.\n");
      fprintf(stderr, "Usage: %s [num_to_insert] [filename]\n", argv[0]);
      standard_test();
   }
   else {
      uint64_t num_to_insert = (uint64_t)atoll(argv[1]);
      char * filename = argv[2];
      custom_test(num_to_insert, filename);
   }
   /*
   tree * t = create();
   int i = 1998;
   insert(20, t);
   insert(500, t);
   insert(5, t);
   insert(12, t);
   for (i; i < 100000;  i++) {
      insert(i , t);
   }
   insert(15, t);
   insert(61, t);
   insert(100, t);
   insert(21, t);
   insert(16, t);
   insert(56, t); 
   insert(64, t);
   rmval(46, t);
   rmval(100, t);
   rmval(15, t);
   rmval(16, t);
   treeprint(t->root);
   deltree(t);
   */
   return 0;
}

void nodecheck(node * n) {
   char * debug_msg = "Value of left ptr: %p\nValue of middle ptr: %p\n"
                      "Value of mid_right ptr: %p\nValue of right ptr:%p\n"
                      "Value of parent: %p\nldata: %f\nmdata: %f\nrdata:"
                      "%f\nis2node: %s\nis3node: %s\nis4node: %s\n";
   char * arr[] = {"false", "true"};
   fprintf(stderr, debug_msg,
                    n->left, n->middle, n->mid_right, n->right, n->parent,
                    n->ldata, n->mdata, n->rdata, arr[n->is2node],
                    arr[n->is3node], arr[n->is4node]);
}
//Runs a standard test of the program using 100,000 insertions/deletions
void standard_test() {
   int testbuflen = 100000;
   int i = 0;
   time_t seed = time(NULL);
   if (errno == EOVERFLOW) {
      fprintf(stderr, "It's at least the year 2038! I've probably got a wife,"
                      " kids, and a Ph.D! Update your system please!\n");
      exit(1);
   }
   srand(seed);
   tree * t = create();
   //To decrease the overhead of inserting values as much as possible,
   //fill an array with random numbers first, then insert and delete
   //them all within two one-line for-loops.
   float * test_array = malloc(sizeof(float) * testbuflen);
   //FILE * fdump = fopen("debug.txt", "w+"); //Test output file...
   for (i; i < testbuflen; i++) {
      test_array[i] = (float)(rand());
      //fprintf(fdump, "%f\n", test_array[i]);
   }
   //fclose(fdump);
   clock_t start_time = clock();

   for (i = 0; i < testbuflen; i++)
      insert(test_array[i], t);
   for (i = 0; i < testbuflen; i++) {
      fprintf(stderr, "Removing %f\n", test_array[i]);
      rmval(test_array[i], t); //TODO: Problem in rmval...*sigh*
   }
   clock_t end_time = clock();
   treeprint(t->root);
   //FIXME: Probably want the time in ms.
   //printf("Runtime in seconds: %li\n", (end_time - start_time) / CLOCKS_PER_SEC);
   deltree(t);
}
//Runs a customised test specified by the user.
void custom_test(uint64_t num_to_insert, char * filename) {

}
