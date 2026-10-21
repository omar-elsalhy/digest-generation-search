//Arbre binaire de recherche
//Insérer données dans l'arbre de recherche et essayer de les retrouver

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "binaryTree.h"

#define MAX_MDP_LENGTH 2048


node_t* createNode(unsigned char *mdp, unsigned char *hash, char *algo){
  node_t* noeud = malloc(sizeof(node_t));
  noeud->right = NULL;
  noeud->left = NULL;
  noeud->mdp = mdp;
  noeud->hash = hash;
  noeud->algo = algo;
  return noeud;
}



//Insertion ordonnée de noeud
node_t* insertNode(node_t *tree, unsigned char *mdp, unsigned char *hash, char *algo){
  if(tree==NULL){
    node_t *newNode = NULL;
    newNode = createNode(mdp, hash, algo);
    return newNode;
  }
  
  if(strcmp((const char *)hash, (const char *)tree->hash) < 0){
    tree->left=insertNode(tree->left, mdp, hash, algo);
  }
  else if(strcmp((const char *)hash, (const char *)tree->hash) > 0){
    tree->right=insertNode(tree->right, mdp, hash, algo);
  }
  else if(strcmp((const char *)hash, (const char *)tree->hash) == 0){
	//free infos du node sans le créer car pas besoin de duplicatat
	free(mdp);
	mdp=NULL;
	free(hash);
	hash=NULL;
	free(algo);
	algo=NULL;
  }

  return tree;
}



//Pas besoin finalement. On va plutôt faire du fileToTree
int treeToFile(FILE* fp, node_t* tree){
	if(tree==NULL){
		return 0;
	}
	treeToFile(fp, tree->left);
	fprintf(fp, "%s\n%s\n%s\n", tree->algo, tree->hash, tree->mdp);
	treeToFile(fp, tree->right);
	return 0;
}



node_t* fileToTree(FILE* fp){
	node_t* tree = NULL;
	
	unsigned char* ligne = (unsigned char*)malloc(MAX_MDP_LENGTH*sizeof(unsigned char));
	
	int ret;
	while( (ret = (fscanf(fp, "%2047[^\n]", ligne))) != EOF && ret !=0){
		fgetc(fp);//consomme retour à la ligne
		char *algo = (char*)malloc(30*sizeof(char));
		memcpy(algo, ligne, 30);
		
		fscanf(fp, "%2047[^\n]", ligne);
		fgetc(fp);//consomme retour à la ligne
		unsigned char *hash_hexa = (unsigned char *)malloc(EVP_MAX_MD_SIZE*sizeof(unsigned char));
		memcpy(hash_hexa, ligne, EVP_MAX_MD_SIZE);
		hash_hexa +='\0';
		
		fscanf(fp, "%2047[^\n]", ligne);
		fgetc(fp);//consomme retour à la ligne
		unsigned char *mdp = (unsigned char*)malloc(MAX_MDP_LENGTH*sizeof(unsigned char));
		memcpy(mdp, ligne, MAX_MDP_LENGTH);
		
		//printf("%s\n%s\n%s\n", algo, hash_hexa, mdp);//test
		
		//Insertion ordonnée du noeud pour optimiser la recherche
		tree = insertNode(tree, mdp, hash_hexa, algo);
	}
	
	free(ligne);
	ligne=NULL;
	
	return tree;
}



node_t* getNodeFromHash(node_t *tree, unsigned char *hash){
	if(hash == NULL || tree == NULL){
		fprintf(stderr, "Erreur en récupérant un noeud de l'arbre.\n");
		return NULL;
	}

	if(strcmp((const char *)hash, (const char *)tree->hash) == 0){
		return tree;
	}
	else if(strcmp((const char *)hash, (const char *)tree->hash) < 0){
		return getNodeFromHash(tree->left, hash);
	}
	else if(strcmp((const char *)hash, (const char *)tree->hash) > 0){
		return getNodeFromHash(tree->right, hash);
	}
	
	return NULL;
}



void printTree(node_t *tree){
	if(tree==NULL){
		return;
	}
	printTree(tree->left);
	printf("%s\n%s\n%s\n\n", tree->algo, tree->hash, tree->mdp);
	printTree(tree->right);
	return;
}



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
  
  free(tree->mdp);
  tree->mdp=NULL;
  free(tree->hash);
  tree->hash=NULL;
  free(tree->algo);
  tree->algo=NULL;
  
  free(tree);
  tree=NULL;
  
  return 0;
}

