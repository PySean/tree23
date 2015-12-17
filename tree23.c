#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree23.h"
/*
 * "tree23.c", by Sean Soderman
 * Implementation of all necessary 2-3 tree functions, as well as
 * auxiliary "helper" functions to cut down on redundant code.
 * TODO: Need "delete" function.
 */

/*
 * Allows children nodes to determine if they are a left, middle, or
 * right node.
 */
typedef enum dir {
   left,
   middle,
   right
}direction;

/*
 * Denotes the type of operation the modmem function will execute.
 */
typedef enum f {
   GET,
   FREE,
   DEL
}fetch_style;

//Inserts val into the tree pointed to by n.
static void minsert(float val, node * n, direction dir);
//Turns n into a 2-node by inserting val into it.
static void simpleswap(float val, node * n);
//Turns n into a 3-node by inserting val into it.
static void swapsort(float val, node * n);
//Function that encompasses (almost) all memory management the tree needs.
static node * modmem(fetch_style f, node * node_to_clear);
//Helper function for rmval that does all the heavy lifting.
static node * mrmval(float val, node * item);

/*
 * Handles the initialization of the tree.
 */
tree * create() {
   tree * seed = malloc(sizeof(tree));
   seed->root = modmem(GET, NULL);
   return seed;
}
/*
 * Takes care of the deletion of the entire tree, including the tree struct.
 */
void deltree(tree * root) {
  (void)modmem(FREE, NULL);
  memset(root, '\0', sizeof(root));
  free(root);
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
   if (n->is4node && n->mid_right) {
      //fprintf(stderr, "Temp 4 node split growth\n");
      //Create new root, have old root's parent ptr point to it.
      //Make sure to clear out the middle data as well.
      node * new_root = modmem(GET, NULL);
      n->parent = new_root;
      new_root->ldata = n->mdata;
      new_root->is2node = true;
      n->mdata = 0;
      //Have the new root point to the old one.
      new_root->left = n;
      //Create the new right branch of the tree as well. Migrate 
      //the proper pointers over (including the parent pointers!)
      node * new_right = modmem(GET, NULL);
      new_root->right = new_right;
      new_right->parent = new_root;
      new_right->ldata = n->rdata;
      new_right->is2node = true;
      n->rdata = 0;
      n->is4node = false;
      n->is3node = false;
      n->is2node = true;
      new_right->left = n->mid_right;
      new_right->right = n->right;
      n->right = n->middle;
      n->middle = NULL;
      n->mid_right = NULL;
      //Have grandchild parent pointers point to new right node.
      new_right->left->parent = new_right;
      new_right->right->parent = new_right;
      root->root = new_root;
   }
   //Initial case of inserting data: a full root node with no children.
   else if (n->is3node && n->left == NULL) {
      //fprintf(stderr, "Initial split growth\n");
      node * new_root = modmem(GET, NULL);
      new_root->left = n;
      swapsort(val, n);
      node * new_right = modmem(GET, NULL);
      n->parent = new_root;
      new_right->parent = new_root;
      new_root->ldata = n->mdata;
      new_root->is2node = true;
      n->mdata = 0;
      new_root->right = new_right;
      new_right->ldata = n->rdata;
      new_right->is2node = true;
      n->rdata = 0;
      n->is3node = false;
      n->is2node = true;
      root->root = new_root;
   }
   else if (n->is2node != true && n->left == NULL) {
      //fprintf(stderr, "Filling up empty root.\n");
      n->ldata = val;
      n->is2node = true;
   }
   else if (n->is2node && n->left == NULL) {
      //fprintf(stderr, "Root has empty spot, swapping value in.\n");
      simpleswap(val, n);
   }

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
   else if (n->is2node && n->left == NULL) { //I am a leaf 2-node
      simpleswap(val, n);
   }
   else { //I am a leaf 3-node and I'm ready to overflow!
      swapsort(val, n);
      n->is4node = true;
      //fprintf(stderr, "ldata: %f mdata: %f rdata: %f\n", 
      //n->ldata, n->mdata, n->rdata);
   }
   //The node has overflowed! Split accordingly.
   if (n->is4node) {
      node * parent = n->parent;
      float promoted_val = n->mdata;
      if (parent->is2node) { //Parent is a 2-node
         //fprintf(stderr,"Inside overflow case for 2-nodes\n");
         simpleswap(promoted_val, parent);
         //fprintf(stderr, "parent ldata: %f rdata: %f\n", 
         //parent->ldata, parent->rdata);
         node * new_node = modmem(GET, NULL);
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
         new_node->is2node = true;
      }
      else { //Parent is a 3-node.
         //fprintf(stderr,"Inside overflow case for 3-nodes\n");
         swapsort(promoted_val, parent);
         parent->is4node = true;
         node * new_node = modmem(GET, NULL);
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
         new_node->is2node = true;
      }
      n->mdata = 0; //Clean up temp value storage.
      n->is2node = true;
      n->is3node = false;
      n->is4node = false;
   }
}

/*
 * Removes the value "val" from the tree.
 */
void rmval(float val, tree * root) {
   node * top_node = root->root;
   //If this is just a root with no children..
   if (top_node->left == NULL) {
      if (top_node->ldata == val) {
         top_node->ldata = 0;
      }
      else if (top_node->rdata == val) {
         top_node->rdata = 0;
      }
      return;
   }
      
   node * new_root = mrmval(val, top_node);
   //If my root node has been cleared...
   if (new_root != NULL) {
     modmem(DEL, top_node);
     root->root = new_root;
   }
}

//Helper function for rmval that does all the heavy lifting.
static node * mrmval(float val, node * item) {
   
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
   n->is2node = false;
   n->is3node = true;
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
 * Wraps a region of memory to write values to that is utilized by the
 * tree.
 * This might seem a little weird, but it's a simpler alternative
 * to emulating a class with a struct. Almost every call to free
 * and malloc is localized within this function.
 *
 * f: a flag that tells grabmem whether it needs to free the tree's
 * memory, fetch more memory, or clear a node and add it to the deleted
 * node buffer.
 * node_to_clear: A memory address that specifies the node to clear
 * and recycle.
 * Returns: a pointer to a node-sized region of memory, or NULL
 * if f is set to FREE.
 */
static node * modmem(fetch_style f, node * node_to_clear) {
   static uint64_t buf_size = 8192; //Beginning size
   //The current memory buffer utilised by the program.
   static node * mem_buf = NULL;
   static uint64_t buf_ndx = 0; 
   //Contains all memory buffers allocated by the program.
   //This obviates the use of "realloc", which can render all
   //tree node pointers useless.
   static node ** buffers = NULL;
   static uint64_t buffers_len = 8192;
   static uint64_t buffers_ndx = 0;
   //The buffer which stores pointers to nodes cleared by the rmval
   //function.
   static node ** delbuf = NULL;
   static uint64_t delbuf_len = 8192;
   static uint64_t delbuf_ndx = 0;
   //Initialize first-time use of mem_buf, as well as aux. buffers.
   if (mem_buf == NULL) {
      mem_buf = malloc(sizeof(node) * buf_size);
      memset(mem_buf, '\0', sizeof(node) * buf_size);
      //I am over-allocating a *lot* here, but that will mean far fewer
      //reallocs for this array of node pointers.
      buffers = malloc(sizeof(node *) * buffers_len);
      buffers[0] = mem_buf;
      delbuf = malloc(sizeof(node *) * delbuf_len);
   }
   //Index into the buffer that provides data to pointers.
   if (f == GET) {
      //Can't change the index after returning, so save the old value.
      uint64_t temp = 0;//buf_ndx++;
      //Return a previously cleared node pointer if there are any left in the
      //buffer filled with them.
      if (delbuf_ndx > 0) {
         return delbuf[--delbuf_ndx];
      }
      temp = buf_ndx++;
      if (buf_ndx > buf_size) {
         //uint64_t tmp_sz = buf_size;
         buf_size *= 2;
         mem_buf = malloc(sizeof(node) * buf_size);
         memset(mem_buf, '\0', sizeof(node) * buf_size);
         //Prefix increment used here because the first element is always
         //full.
         buffers[++buffers_ndx] = mem_buf;
         if (buffers_ndx == buffers_len) {
            buffers_len *= 2;
            //Not going to bother using memset here, as the memory is
            //never read from before it's allocated.
            buffers = realloc(buffers, sizeof(node *) * buffers_len);
         }
         temp = 0;
         buf_ndx = 1;
      }
      return mem_buf + temp;
   }
   //A call to rmval was made, clear up the passed in address's data
   //and add its address to the "free" buffer.
   else if (f == DEL) {
      if (node_to_clear == NULL) {
         fprintf(stderr, "Please pass in a valid address to clear.\n");
         return NULL; //Perhaps ret a value other than NULL for an error...
      }
      delbuf[delbuf_ndx++] = node_to_clear;
      memset(node_to_clear, '\0', sizeof(node));
      if (delbuf_ndx == delbuf_len) {
         delbuf_len *= 2;
         delbuf = realloc(delbuf, sizeof(node *) * delbuf_len);
      }
      return NULL;
   }
   //Return everything to its initial state, free all buffers.
   else if (f == FREE) {
      uint64_t i = 0;
      for (i; i <= buffers_ndx; i++) {
         memset(buffers[i], '\0', sizeof(node) * 8192 * (i + 1));
         free(buffers[i]);
      }
      memset(buffers, '\0', sizeof(node *) * buffers_len);
      free(buffers);
      memset(delbuf, '\0', sizeof(node *) * delbuf_len);
      free(delbuf);
      buf_size = 8192;
      mem_buf = NULL;
      buf_ndx = 0;
      buffers = NULL;
      buffers_len = 8192;
      buffers_ndx = 0;
      delbuf = NULL;
      delbuf_len = 8192;
      delbuf_ndx = 0;
      return NULL;
   }
}
