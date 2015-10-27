/*
 * "tree23.h", by Sean Soderman
 * Hmm. What should I define here?
 */


typedef struct n {
   struct n * left;
   struct n * right;
   float ldata;
   float rdata;
}node;

typedef struct t {
   uint64_t size;
   node * root;
}tree;
