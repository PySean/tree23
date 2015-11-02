#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree23.h"

#define GET 2
#define SET 3
/*
 * "tree23.c", by Sean Soderman
 * Implementation of all necessary 2-3 tree functions, as well as
 * auxiliary "helper" functions.
 */

/*
 * Swaps two values.
 * I'll be doing this a lot, so it is helpful.
 */
static void swap(float * one, float * two) {
   float temp = *one;
   *one = *two;
   *two = temp;
}

/*
 * Wraps a boolean variable that can either be set or retrieved.
 * This value denotes whether an insertion was sucessful or not.
 */
static bool success(bool val, int op) {
   static bool is_successful = false;
   if (op != GET || op != SET) {
      fprintf(stderr, "Error: 'op' only takes GET or SET as parameters.\n");
      return false;
   }
   if (op == GET)
      return is_successful;
   else
      is_successful = val;
}

/*
 * Wraps a region of memory to write values to that is utilized by the
 * tree.
 * This might seem a little weird, but it's a simpler alternative
 * to emulating a class with a struct.
 * free: a flag that tells grabmem whether it needs to free the tree's
 * memory.
 * Returns: a pointer to a node-sized region of memory.
 */
node * modmem(bool del) {
   static uint64_t buf_size = 8192; //Beginning size
   static node * mem_buf = NULL; //malloc(sizeof(node) * buf_size);
   //Initialize first-time use of mem_buf.
   mem_buf = mem_buf == NULL ? malloc(sizeof(node) * buf_size) : mem_buf;
   //Index into the buffer that provides data to pointers.
   static uint64_t buf_ndx = 0; 
   uint64_t temp = buf_ndx;
   if (del == false) {
      //Can't change the index after returning, so save the old value.
      uint64_t temp = buf_ndx++;
      if (buf_ndx > buf_size) {
         buf_size *= 2;
         mem_buf = realloc(mem_buf, buf_size * sizeof(node));
      }
      return mem_buf + temp;
   }
   else {
      memset(mem_buf, '\0', sizeof(node) * buf_size);
      free(mem_buf);
      return NULL;
   }
}
