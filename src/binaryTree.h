#ifndef BINARYTREE_H
#define BINARYTREE_H

//Structure(s)
typedef struct node {
  int val;
  struct node *right;
  struct node *left;
} node_t;


//Prototypes de fonctions
node_t* createNode(int val);
node_t* insertNode(node_t *tree, int val);
int treeHeight(node_t *tree);
int printTree(node_t *tree, int depth);
int freeTree(node_t* tree);


#endif