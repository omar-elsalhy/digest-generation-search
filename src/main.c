#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>



char* getMD5(string maChaine){

}


int main(int argc, char* argc[]){

	unsigned char password[] = "test";
	unsigned char condensat[MD5_DIGEST_LENGTH];
	MD5(password, sizeof(password) - 1, condensat);

	//Affichage du résultat (le condensat du mdp):
	for (int i=0; i<MD5_DIGEST_LENGTH; i++){
		printf("%02x", condensat[i]);
	}
	printf("\n");
	return 0;

}
