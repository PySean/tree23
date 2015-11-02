/*
 * "tree23.h", by Sean Soderman
 * Specification of 2-3 tree functions.
 * This tree utilizes floating point values for an easy way to check for
 */
#ifndef STDBOOL_H
#include <stdbool.h>
#endif

#ifndef STDINT_H
#include <stdint.h>
#endif



typedef struct n {
   struct n * left;
   struct n * middle;
   struct n * right;
   float ldata;
   float rdata;
   bool isempty;
   bool isfull;
}node;

typedef struct t {
   uint64_t size;
   node * root;
}tree;

//Simply creates and initializes a 2-3 tree.
tree * create();

//Inserts a value into the tree.
void insert(float val, tree * root);

//Removes a value from the tree.
void rmval(float val, tree * root);

//Prints all values of the tree out, in order, using a depth-first traversal.
void treeprint(tree * root);
