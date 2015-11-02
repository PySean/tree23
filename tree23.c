#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
 * Wraps a region of memory to write values for that is utilized by the
 * tree.
 * free: a flag that tells grabmem whether it needs to free
 *
 * HMMM...idea. Wrap this and "freemem" in a struct that can emulate
 * a class instead, using function pointers for passing in the
 * buffer variable... is that even possible? I'll find out!
 */
static node * grabmem(bool free) {
   static uint64_t buf_size = 8192; //Beginning size
   static node * mem_buf = malloc(sizeof(node) * buf_size);
   //Index into the buffer that provides data to pointers.
   static uint64_t buf_ndx = 0; 
   uint64_t temp = buf_ndx;
   buf_ndx++;
   if (buf_ndx > buf_size) {
      buf_size *= 2;
      realloc(
   }
   
}
