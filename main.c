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
   int i = 1;
   /*
   insert(20, t);
   insert(500, t);
   insert(5, t);
   insert(21, t);
   */
   /*
   for (i; i < 100000; i++)
      insert(i % 5, t);
   node * root = t->root;
   treeprint(root);
   */
   insert(15, t);
   insert(61, t);
   insert(100, t);
   insert(21, t);
   insert(16, t);
   //rmval(16, t);
   treeprint(t->root);
   deltree(t);
   return 0;
}
