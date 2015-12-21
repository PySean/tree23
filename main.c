#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree23.h"
/*
 *
 * "main.c", by Sean Soderman
 * Simply tests the 2-3 tree functions I've implemented.
 */
//Debug printing function that exposes all values within a node.
void nodecheck(node * n);
int main(int argc, char * argv[]) {
   tree * t = create();
   int i = 1998;
   /*
   insert(20, t);
   insert(500, t);
   insert(5, t);
   insert(21, t);
   */
   for (i; i < 2005; i++) {
      insert(i , t);
      //nodecheck(t->root);
   }
   //The integrity of these first values is verified.
   //node * root = t->root;
   insert(15, t);
   insert(61, t);
   insert(100, t);
   insert(21, t);
   insert(16, t); //Incorrect assignment to parent happens here, for midchild.
   //FIXME: The tree order is off with 14 values (of this sort.) 
   //I need to trace it (TODO: Check integrity of tree before 
   //the problematic insertion, then trace through how that insertion would
   //affect the tree.)
   //Extra inserts to aggravate segfault (actually, the tree order is off)
   insert(56, t); 
   //treeprint(t->root);
   insert(64, t); //This is the straw that breaks the camel's back.

   //insert(99, t);
   //insert(1, t);
   treeprint(t->root);
   
   //fprintf(stderr, "blip\n");
   //nodecheck(t->root);
   ///************
   //rmval(46, t);
   //treeprint(t->root);
   //rmval(100, t);
   //treeprint(t->root);
   //rmval(15, t);
   //treeprint(t->root);
   //rmval(16, t);
   //nodecheck(t->root);
   //treeprint(t->root);
   deltree(t);
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
