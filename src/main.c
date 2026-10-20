#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <getopt.h>
#include "binaryTree.h"


//https://linux.die.net/man/3/evp_md5
char* generateMD5(char* mdp){

	return NULL;
}


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


int generate(char *input, char *output, char *algo){
	if(!input || !output || !algo){
		fprintf(stderr, "Impossible de générer les condensats. Le nom d'un des fichiers ou de l'algo est NULL.\n");
		exit(EXIT_FAILURE);
	}
	
	
	
	
	
	
	
	
	return 0;
}



int lookup(char* tab){
	
	
	
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
						usage();
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
						usage();
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
						usage();
						exit(EXIT_FAILURE);
					}
					break;
				case '?':
					fprintf(stderr, "Option inconnue ou argument manquant.\n\n");
					break;
				default:
					fprintf(stderr, "Option inconnue.\n\n");
					break;
			}
		}
		
		
		if(algo_specifie != 1){
			algo = "MD5";
		}
		
		if(dico_specifie != 1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier dictionnaire.\n");
			usage();
			exit(EXIT_FAILURE);
		}
		
		if(t3c_out_specifie !=1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier output de condensats.\n");
			usage();
			exit(EXIT_FAILURE);
		}
		
		//Mode generation de condensats
		generate(dictionnaire, t3c_output, algo);
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
						usage();
						exit(EXIT_FAILURE);
					}
					break;
				case '?':
					fprintf(stderr, "Option inconnue ou argument manquant.\n\n");
					break;
				default:
					fprintf(stderr, "Option inconnue.\n\n");
					break;
			}
		}
		
		if(table_specifiee != 1){
			fprintf(stderr, "Vous n'avez pas spécifié le fichier table de correspondances MDP-hash.\n");
			usage();
			exit(EXIT_FAILURE);
		}
		
		//Mode recherche parmi une liste de condensats
		lookup(table);
	}

	//exit(EXIT_SUCCESS);



	// Brouillon
	
	unsigned char password[] = "test";
	unsigned char condensat[EVP_MAX_MD_SIZE];
	unsigned int len;
	
	

	if (EVP_Digest(password, sizeof(password) - 1,
				   condensat, &len, EVP_md5(), NULL) != 1) {
		fprintf(stderr, "MD5 calculation failed\n");
		return 1;
	}


	//Affichage du résultat (le condensat du mdp):
	for (int i=0; i<len; i++){
		printf("%02x", condensat[i]);
	}
	printf("\n");
	return 0;

}
