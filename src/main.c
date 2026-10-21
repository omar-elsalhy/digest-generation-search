#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <getopt.h>
#include "binaryTree.h"

#define MAX_LINE_LENGTH 2048


char getMode(int argc, char* argv[]){
	if(argc > 1){
		if(strcmp(argv[1], "G") == 0){
			return 'G';
		}
		else if(strcmp(argv[1], "L") == 0){
			return 'L';
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
}



void usage(){
	printf("\nUtilisation:\n\n");
	printf("Mode G\n./t3c G -i <dictionnaire> -o <t3c_output> [-a <algo>]\n");
	printf("-i / --input <path> : dictionnaire (obligatoire)\n-o / --output <path> : sortie T3C (obligatoire)\n-a / --algo <nom> : algorithme (optionnel).\n\n");
	printf("Mode L\n./t3c L -t <t3c_path>\n-t / --table <path> : T3C à charger (obligatoire)\n");
	printf("Les condensats sont fournis uniquement via stdin, séparés par des sauts de ligne.\n");
}



//Génère le condensat d'une chaîne de caractères passée en paramètres
unsigned char* generateMD5(unsigned char* mdp){
	unsigned char *condensat = (unsigned char*)malloc(EVP_MAX_MD_SIZE * sizeof(unsigned char));
	unsigned int len;//=16 (2*16 = 32 octets MD5)
	
	if (EVP_Digest(mdp, strlen((const char *)mdp) - 1, condensat, &len, EVP_md5(), NULL) != 1) {
		fprintf(stderr, "Erreur dans le calcul du MD5.\n");
		free(condensat);
		condensat=NULL;
		return NULL;
	}
	
	return condensat;
}



void md5HashToHex(unsigned char *hash, unsigned char *hash_hexa){
	char hexa[] = "0123456789abcdef";
	for (int i=0; i<16; i++){
		hash_hexa[2*i] = hexa[(hash[i] & 0xF0) >> 4 ];
		hash_hexa[2*i + 1] = hexa[(hash[i] & 0x0F) ];
	}
	hash_hexa[32]='\0';
	return;
}



//Génération des hash, insertion dans arbre binaire, sauvegarde arbre dans fichier t3c_output
int generate(char *input, char *output, char *algo){
	if(!input || !output || !algo){
		fprintf(stderr, "Impossible de générer les condensats. Le nom d'un des fichiers ou de l'algo est NULL.\n");
		return 1;
	}
	
	//Un seul algo pour le moment
	if(strcmp(algo, "MD5") != 0){
		fprintf(stderr, "Algo spécifié inconnu.\n");
		return 1;
	}
	
	
	//Fichier de dictionnaire
	FILE *fp = NULL;
	fp = fopen(input, "r");
	if(fp == NULL){
		fprintf(stderr, "Erreur à l'ouverture du fichier .\n");
		return 1;
	}
	
	
	//Fichier d'output
	FILE *fp2 = NULL;
	fp2 = fopen(output, "w");
	if(fp2 == NULL){
		fprintf(stderr, "Erreur à l'ouverture du fichier .\n");
		return 1;
	}
	
	
	//Variable pour stocker les lignes lues dans le fichier de dictionnaire
	unsigned char* ligne = (unsigned char*)malloc(MAX_LINE_LENGTH*sizeof(unsigned char));


	//Arbre binaire pour stocker les infos (algo, mdp, hash)
	//node_t *tree = NULL;
	
	//Ici, fscanf lit tous les caractères de la ligne jusqu'au \n, puis consomme le \n. La ligne lue aura au maximum 2048 caractères en comptant le \0.
	//(fscanf(fp, "%2047[^\n]\n", ligne))
	//brouillon - ne marche pas avec la dernière ligne
	/*while((fscanf(fp, "%2047[^\n]\n", ligne)) != EOF){
		//Process le hash et insertion dans l'arbre binaire
		unsigned char *hash = generateMD5(ligne);
		if(hash != NULL){
			printf("%s\n", ligne);
			for (int i=0; i<strlen((const char *)hash); i++){
				printf("%02x", hash[i]);
			}
			printf("\n");
		}
	}*/
	
	
	//Je préfère savoir si la ligne a été entièrement lue:
	while( (fscanf(fp, "%2047[^\n]", ligne)) != EOF){
		//"consommation" du \n et le reste éventuel de la ligne pour passer à la ligne suivante
		int c = fgetc(fp);
		if (c!='\n'){
			fprintf(stderr, "Ligne trop longue ou impossible à lire. Limite de ligne %d caractères.\n", MAX_LINE_LENGTH - 1);
			while((c = fgetc(fp)) != '\n' && c != EOF){
				;//ne fait rien, consomme le reste des caractères de la ligne
			}
		}
		else{
			//Process le hash et insertion dans le fichier t3c_output (//dans l'arbre binaire, non directement dans le fichier t3c_output)
			unsigned char *hash = generateMD5(ligne);
			unsigned char *hash_hexa = (unsigned char *)malloc(EVP_MAX_MD_SIZE*sizeof(unsigned char));
			//unsigned char *mdp = (unsigned char*)malloc(MAX_LINE_LENGTH*sizeof(unsigned char));
			
			//Je copie la ligne lue dans mdp pour la garder
			//memcpy(mdp, ligne, 2048);
			
			if(hash != NULL && hash_hexa != NULL){ //&& mdp != NULL){
				md5HashToHex(hash, hash_hexa);
				fprintf(fp2, "%s\n%s\n%s\n", algo, hash_hexa, ligne);
				//tree = insertNode(tree, mdp, hash_hexa, algo);
			}
			
			//plus besoin du condensat au format string
			free(hash);
			hash=NULL;
			free(hash_hexa);
			hash_hexa = NULL;
		}
	}

	
	//Sauvegarde arbre dans fichier t3c_output
	//treeToFile(fp2, tree);	
	
	//Free les pointeurs (l'arbre)
	//freeTree(tree);
	//tree=NULL;
	free(ligne);
	ligne=NULL;
	fclose(fp);
	fp=NULL;
	fclose(fp2);
	fp2 = NULL;
	
	return 0;
}




//Recherche de condensats dans arbre binaire
int lookup(char* tab){
	//Fichier de correspondances condensats-mdp
	FILE *fp = NULL;
	fp = fopen(tab, "r");
	if(fp == NULL){
		fprintf(stderr, "Erreur à l'ouverture du fichier.\n");
		return 1;
	}
	
	//Arbre binaire pour stocker les infos (algo, mdp, hash)
	node_t *tree = NULL;
	tree = fileToTree(fp);
	//printTree(tree);//juste pour tester
	
	//Recherche dans l'arbre
	unsigned char* ligne = (unsigned char*)malloc(MAX_LINE_LENGTH*sizeof(unsigned char));
	//Lit toutes les lignes de stdin jusqu'à CTRL+D ou EOF
    while(fscanf(stdin, "%2047[^\n]", ligne) == 1) {
		//"consommation" du \n et le reste éventuel de la ligne pour passer à la ligne suivante
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {
			//Ne fait rien (consomme le reste de la ligne)
		}
		node_t *noeud_correspondant = getNodeFromHash(tree, ligne);
		if(noeud_correspondant == NULL){
			//Cas où il ne trouve pas le hash, affiche ligne vide
			printf("\n");
		}
		else{
			//Cas où il trouve le hash, affiche le mdp correspondant
			printf("%s\n", noeud_correspondant->mdp);
		}
		
    }
	
	
	//Libération mémoire
	free(ligne);
	ligne=NULL;
	freeTree(tree);
	tree=NULL;
	fclose(fp);
	fp=NULL;
	printf("fin\n");
	
	return 0;
}




int main(int argc, char* argv[]){
	//Variables globales
	char *dictionnaire = NULL;
	int dico_specifie = 0;
	char *t3c_output = NULL;
	int t3c_out_specifie = 0;
	char *algo = NULL;
	int algo_specifie = 0;
	char *table = NULL;
	int table_specifiee = 0;

	//Détection du mode
	char mode = getMode(argc, argv);
	if(mode!='G' && mode!='L'){
		fprintf(stderr, "Mode inconnu ou non renseigné.\n");
		usage();
		exit(EXIT_FAILURE);
	}


	//Lecture des options selon le mode
	struct option long_options_G[] = {
		{"input", required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"algo", required_argument, 0,  'a' },
		{0, 0, 0, 0}
	};
	
	struct option long_options_L[] = {
		{"table", required_argument, 0, 't'},
		{0, 0, 0, 0}
	};


	optind=2;//ignore les modes (G ou L) //indique l'emplacement du prochain argument à traiter dans argv (ici, le 2e)
	int opt;
	if (mode == 'G'){
		while ((opt = getopt_long(argc, argv, "i:o:a:", long_options_G, 0)) != -1){
			switch (opt){
				case 'i':
					if(optarg){
						dico_specifie = 1;
						dictionnaire = optarg;
					}
					else{
						fprintf(stderr, "Vous n'avez pas spécifié le fichier dictionnaire.\n\n");
						//usage();
						exit(EXIT_FAILURE);
					}
					break;
				case 'o':
					if(optarg){
						t3c_out_specifie = 1;
						t3c_output = optarg;
					}
					else{
						fprintf(stderr, "Vous n'avez pas spécifié le fichier de sortie pour sauvegarder les condensats.\n\n");
						//usage();
						exit(EXIT_FAILURE);
					}
					break;
				case 'a':
					if(optarg){
						algo_specifie = 1;
						algo = optarg;
					}
					else{
						fprintf(stderr, "Vous n'avez pas spécifié l'algorithme en option.\nIl n'est pas obligatoire (MD5 par défaut), mais vous avez spécifié l'option sans arguments.\n\n");
						//usage();
						exit(EXIT_FAILURE);
					}
					break;
				case '?':
					fprintf(stderr, "Option inconnue ou argument manquant.\n\n");
					//usage();
					exit(EXIT_FAILURE);
					break;
				default:
					fprintf(stderr, "Option inconnue.\n\n");
					//usage();
					exit(EXIT_FAILURE);
					break;
			}
		}
		
		
		if(algo_specifie != 1){
			algo = "MD5";
		}
		
		if(dico_specifie != 1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier dictionnaire.\n");
			//usage();
			exit(EXIT_FAILURE);
		}
		
		if(t3c_out_specifie !=1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier output de condensats.\n");
			//usage();
			exit(EXIT_FAILURE);
		}
		
		//Mode generation de condensats
		if(generate(dictionnaire, t3c_output, algo) != 0){
			exit(EXIT_FAILURE);
		}
	}
	
	
	
	if (mode == 'L'){
		while ((opt = getopt_long(argc, argv, "t:", long_options_L, 0)) != -1){
			switch (opt){
				case 't':
					if(optarg){
						table_specifiee = 1;
						table = optarg;
					}
					else{
						fprintf(stderr, "Vous n'avez pas spécifié le fichier table de correspondances MDP-hash.\n\n");
						//usage();
						exit(EXIT_FAILURE);
					}
					break;
				case '?':
					fprintf(stderr, "Option inconnue ou argument manquant.\n\n");
					//usage();
					exit(EXIT_FAILURE);
					break;
				default:
					fprintf(stderr, "Option inconnue.\n\n");
					//usage();
					exit(EXIT_FAILURE);
					break;
			}
		}
		
		if(table_specifiee != 1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier table de correspondances MDP-hash.\n");
			//usage();
			exit(EXIT_FAILURE);
		}
		
		//Mode recherche parmi une liste de condensats
		lookup(table);
	}

	exit(EXIT_SUCCESS);
	//return 0;
}
