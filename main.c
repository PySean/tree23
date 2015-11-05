#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree23.h"
/*
 *
 * "main.c", by Sean Soderman
 * Simply tests the 2-3 tree functions I've implemented.
 */
int main(int argc, char * argv[]) {
   tree * t = create();
   insert(5, t);
   insert(6, t);
   insert(7, t);
   insert(4, t);
   insert(3, t);
   insert(10, t);
   insert(50, t);
   node * root = t->root;
   treeprint(root);
   return 0;
}
