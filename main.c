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
   insert(20, t);
   insert(500, t);
   insert(5, t);
   insert(12, t);
   for (i; i < 2005;  i++) {
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
