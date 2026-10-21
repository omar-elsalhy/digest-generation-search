#ifndef BINARYTREE_H
#define BINARYTREE_H

//Structure(s)
typedef struct node {
  unsigned char *mdp;
  unsigned char *hash;
  char *algo;//MD5, SHA256, other...
  struct node *right;
  struct node *left;
} node_t;


//Prototypes de fonctions
node_t* createNode(unsigned char *mdp, unsigned char *hash, char *algo);
node_t* insertNode(node_t *tree, unsigned char *mdp, unsigned char *hash, char *algo);
int treeToFile(FILE* fp, node_t* tree);
node_t* fileToTree(FILE* fp);
node_t* getNodeFromHash(node_t *tree, unsigned char *hash);
void printTree(node_t *tree);
int freeTree(node_t* tree);


#endif