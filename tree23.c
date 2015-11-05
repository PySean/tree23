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
static void simpleswap(float val, node * n);
static void swapsort(float val, node * n);
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
 * TODO: The assumption that a value is empty if it == 0 is weak.
 * I should define a boolean field within the struct to really determine
 * whether a spot is taken or not.
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
   else if (n->ldata && n->rdata) {
      node * new_root = modmem(GET);
      new_root->left = n;
      swapsort(val, n);
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
   else 
      simpleswap(val, n);

}

/*
 * Prints all values of the tree in order, using depth-first traversal.
 */
void treeprint(node * root) {
   if (root->left != NULL)
      treeprint(root->left);
   printf("ldata: %f rdata: %f\n", root->ldata, root->rdata);
   if (root->middle != NULL)
      treeprint(root->middle);
   if (root->right != NULL)
      treeprint(root->right);
}

//Helper function for insert. Does all the heavy lifting save for growth
//at the root node, which is reserved for insert itself.
static void minsert(float val, node * n, direction dir) {
   //Shameless copy from insert. The logic is identical...
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
   else if (n->ldata && !n->rdata) { //I am a leaf 2-node
      simpleswap(val, n);
   }
   else { //I am a leaf 3-node and I'm ready to overflow!
      swapsort(val, n);
   }
   //The node has overflowed! Split accordingly.
   if (n->ldata && n->mdata && n->rdata) {
      node * parent = n->parent;
      float promoted_val = n->mdata;
      if (parent->ldata && !parent->rdata) { //Parent is a 2-node
         simpleswap(promoted_val, parent);
         node * new_node = modmem(GET);
         new_node->parent = parent;
         parent->middle = new_node;
         switch(dir) {
            case left:
               new_node->ldata = n->rdata;
               //Transfer pointers unconditionally, since it wouldn't hurt
               //either way
               new_node->left = n->mid_right;
               new_node->right = n->right;
               n->right = n->middle;
               break;
            case right:
               new_node->ldata = n->ldata;
               n->ldata = n->rdata;
               //As above, unconditional pointer xfer.
               new_node->left = n->left;
               new_node->right = n->middle;
               n->left = n->mid_right;
               break;
         }
         n->mid_right = NULL;
         n->middle = NULL;
         n->rdata = 0;
      }
      else { //Parent is a 3-node.
         swapsort(promoted_val, parent);
         node * new_node = modmem(GET);
         new_node->parent = parent;
         switch(dir) {
            case left: //Rearrange for left
               parent->mid_right = parent->middle;
               parent->middle = new_node;
               new_node->ldata = n->rdata;
               new_node->left = n->mid_right;
               new_node->right = n->right;
               n->right = n->middle;
               break;
            case middle: //Rearrange for middle
               parent->mid_right = new_node;
               new_node->ldata = n->rdata;
               new_node->left = n->mid_right;
               new_node->right = n->right;
               n->right = n->middle;
               break;
            case right: //Rearrange for right
               parent->mid_right = new_node;
               new_node->ldata = n->ldata;
               n->ldata = n->rdata;
               new_node->left = n->left;
               new_node->right = n->middle;
               n->left = n->mid_right;
               break;
         }
         n->rdata = 0;
         n->middle = NULL;
         n->mid_right = NULL;
         //TODO: This code here is fine. Something happened at the top.
         fprintf(stderr, "At the end of the 3-node case. Data follows:\n");
         fprintf(stderr, "Parent node values: %f:%f:%f\n", parent->ldata,
                 parent->mdata, parent->rdata);
         fprintf(stderr, "Left: %f Middle: %f Midright: %f Right: %f\n",
                 parent->left->ldata, parent->middle->ldata,
                 parent->mid_right->ldata, parent->right->ldata);
         fprintf(stderr, "Current node: %f\n", n->ldata);
      }
      n->mdata = 0; //Clean up temp value storage.
   }
}

/*
 * Swaps 'val' into the 2-node in such a way that 
 * the left value is smaller than (or equal to) the right one.
 */
static void simpleswap(float val, node * n) {
   if (val > n->ldata)
      n->rdata = val;
   else {
      n->rdata = n->ldata;
      n->ldata = val;
   }
}
/*
 * Swaps 'val' into the 3-node in such a way that 
 * the values in the node are in sorted order (from left to right).
 * Should only be used on filled up nodes.
 */
static void swapsort(float val, node * n) {
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
