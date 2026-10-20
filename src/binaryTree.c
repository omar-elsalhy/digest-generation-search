//Arbre binaire de recherche
//Insérer données dans l'arbre de recherche et essayer de les retrouver

#include <stdio.h>
#include <stdlib.h>
#include "binaryTree.h"


node_t* createNode(int val){
  node_t* noeud = malloc(sizeof(node_t));
  noeud->right = NULL;
  noeud->left = NULL;
  noeud->val = val;
  return noeud;
}


node_t* insertNode(node_t *tree, int val){
  if(tree==NULL){
    node_t *newNode = NULL;
    newNode = createNode(val);
    return newNode;
  }
  
  if(val < tree->val){
    tree->left=insertNode(tree->left, val);
  }
  else if(val > tree->val){
    tree->right=insertNode(tree->right, val);
  }

  return tree;
}


int treeHeight(node_t *tree){

  return 0;
}


//Aussi fonction pour afficher toute la liste
int printTree(node_t *tree, int depth){
  if(tree == NULL){
    return 0;
  }
  
  printTree(tree->left, depth+1);
  for(int i=0; i<depth; i++){
    printf("   ");
  }
  printf("+%d\n", tree->val);
  printTree(tree->right, depth+1);
  
  return 0;
}


//Free la liste
int freeTree(node_t* tree){
  if(tree==NULL){
    return 0;
  }
  
  if(tree->left !=NULL){
    freeTree(tree->left);
  }
  
  if(tree->right !=NULL){
    freeTree(tree->right);
  }
  
  tree->left = NULL;
  tree->right = NULL;
  free(tree);
  tree=NULL;
  
  return 0;
}

