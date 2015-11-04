#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree23.h"


/*
 * Allows children nodes to determine if they are a left, middle, or
 * right node.
 */
typedef enum dir {
   left,
   middle,
   right
}direction;

typedef enum f {
   GET,
   FREE
}fetch_style;

static void minsert(float val, node * n, direction dir);
static void swap(float * one, float * two);
static node * modmem(fetch_style f);
/*
 * "tree23.c", by Sean Soderman
 * Implementation of all necessary 2-3 tree functions, as well as
 * auxiliary "helper" functions to cut down on redundant code.
 */



/*
 * Handles the initialization of the tree.
 */
tree * create() {
   tree * seed = malloc(sizeof(tree));
   seed->root = modmem(GET);
   return seed;
}
/*
 * Takes the value "val" and inserts it into the tree.
 * Grows at the root if necessary.
 */
void insert(float val, tree * root) {
   node * n = root->root;
   if (n->left || n->right) { //If I am a 2 or 3 node w/ children.
      if (val < n->ldata) {
         minsert(val, n->left, left);
      }
      else if (n->middle != NULL && val < n->rdata) {
         minsert(val, n->middle, middle);
      }
      else {
         minsert(val, n->right, right);
      }
   }
   //The root is a temp-4 node w/children, grow a new root and right node.
   else if (n->mid_right && n->mdata) {
      //Create new root, have old root's parent ptr point to it.
      //Make sure to clear out the middle data as well.
      node * new_root = modmem(GET);
      n->parent = new_root;
      new_root->ldata = n->mdata;
      n->mdata = 0;
      //Have the new root point to the old one.
      new_root->left = n;
      //Create the new right branch of the tree as well. Migrate 
      //the proper pointers over.
      node * new_right = modmem(GET);
      new_root->right = new_right;
      new_right->parent = new_root;
      new_right->ldata = n->rdata;
      n->rdata = 0;
      new_right->left = n->mid_right;
      new_right->right = n->right;
      n->right = n->middle;
      n->middle = NULL;
      n->mid_right = NULL;
      root->root = new_root;
   }
   //Initial case of inserting data: a full root node with no children.
   //TODO: Just swap val into the node and then do the typical operations.
   else if (n->ldata && n->rdata) {
      node * new_root = modmem(GET);
      new_root->left = n;
      if (val > n->ldata && val <= n->rdata) {
         n->mdata = val;
      }
      else if (val <= n->ldata) {
         n->mdata = n->ldata;
         n->ldata = val;
      }
      else {
         n->mdata = n->rdata;
         n->rdata = val;
      }
      node * new_right = modmem(GET);
      n->parent = new_root;
      new_right->parent = new_root;
      new_root->ldata = n->mdata;
      n->mdata = 0;
      new_root->right = new_right;
      new_right->ldata = n->rdata;
      n->rdata = 0;
      root->root = new_root;
   }
   else if (!(n->ldata || n->rdata)) {
      n->ldata = val;
   }
   else if (val <= n->ldata) {
      n->rdata = n->ldata;
      n->ldata = val;
   }
   else if (val > n->ldata)
      n->rdata = val;
}

/*
 * Prints all values of the tree in order, using depth-first traversal.
 */
void treeprint(node * root) {
   if (root->left != NULL)
      treeprint(root->left);
   printf("ldata: %f rdata: %f\n", root->ldata, root->rdata);
   if (root->middle != NULL)
      treeprint(root->right);
   if (root->right != NULL)
      treeprint(root->right);
}

//Helper function for insert. Does all the heavy lifting save for growth
//at the root node, which is reserved for insert itself.
static void minsert(float val, node * n, direction dir) {
   
   
}

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
/*
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
*/

/*
 * Wraps a region of memory to write values to that is utilized by the
 * tree.
 * This might seem a little weird, but it's a simpler alternative
 * to emulating a class with a struct.
 * free: a flag that tells grabmem whether it needs to free the tree's
 * memory.
 * Returns: a pointer to a node-sized region of memory.
 */
static node * modmem(fetch_style f) {
   static uint64_t buf_size = 8192; //Beginning size
   static node * mem_buf = NULL; //malloc(sizeof(node) * buf_size);
   //Initialize first-time use of mem_buf.
   mem_buf = mem_buf == NULL ? malloc(sizeof(node) * buf_size) : mem_buf;
   //Index into the buffer that provides data to pointers.
   static uint64_t buf_ndx = 0; 
   uint64_t temp = buf_ndx;
   if (f == GET) {
      //Can't change the index after returning, so save the old value.
      uint64_t temp = buf_ndx++;
      if (buf_ndx > buf_size) {
         buf_size *= 2;
         mem_buf = realloc(mem_buf, buf_size * sizeof(node));
      }
      return mem_buf + temp;
   }
   else if (f == FREE) {
      memset(mem_buf, '\0', sizeof(node) * buf_size);
      free(mem_buf);
      return NULL;
   }
}
