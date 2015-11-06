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
   insert(2, t);
   insert(21, t);
   insert(47, t);
   int i = 1;
   for (i; i < 300; i++)
      insert(i, t);
   node * root = t->root;
   treeprint(root);
   fprintf(stderr, "root val: %f left val: %f right val %f\n",
           root->ldata, root->left->ldata, root->right->ldata);
   char * z[2] = {"no", "yes"};
   node * rchild = root->right;
   node * rchildparent = rchild->right->parent;
   node * lchild = root->left;
   node * lchildparent = lchild->left->parent;
   fprintf(stderr, "lchild parent == root? %s rchild parent == root? %s\n",
           z[root->left->parent == root], z[root->right->parent == root]);
   fprintf(stderr, "rchild == rchild parent? %s\n",
           z[rchild == rchildparent]); 
   fprintf(stderr, "lchild == lchild parent? %s\n",
           z[lchild == lchildparent]);
   return 0;
}
