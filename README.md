# Générateur et chercheur de condensats

## Détails sur le programme

Ce programme a 2 modes de fonctionnement: génération de condensats (G pour generate) et recherche parmi une liste de condensats (L pour lookup).


Mode G:
- lit les chaînes d'entrée depuis un fichier (dit dictionnaire).
- accepte que le séparateur des chaînes soit un saut de ligne \n.
- génère le condensat cryptographique de chaque chaîne.
- écrit la table de correspondance chaîne-condensat (T3C) dans un fichier.
- supporte MD5. (Pourra accepter d'autres algorithmes dans le futur.)


Mode L:
- charge en mémoire une T3C depuis un fichier.
- permet une recherche rapide d'une chaîne à partir de son condensat, grâce à une implémentation d'arbre binaire.
- reçoit les condensats sur l'entrée standard, séparés par \n.
- affiche les chaînes trouvées sur la sortie standard.


Pour tous les modes:
- reçoit les fichiers d'entrée (dictionnaire, T3C) via des chemins en arguments de ligne de commande.
- reçoit tous les paramètres utiles en arguments de ligne de commande.



## Prototypes d'appels

Le makefile produit, avec make à la racine, un exécutable nommé t3c à la racine du dépôt.
Le mode est le premier argument positionnel :
`./t3c <G|L> [options]`


Mode G:
```bash
./t3c G -i <dictionnaire> -o <t3c_output> [-a <algo>]
-i / --input <path> : dictionnaire
-o / --output <path> : sortie T3C
-a / --algo <nom> : algorithme #Non fonctionnel, MD5 par défaut
```

Mode L:
```bash
./t3c L -t <t3c_path>
-t / --table <path> : T3C à charger
```

Les condensats sont fournis uniquement via stdin, séparés par des sauts de ligne.


IMPORTANT:

Si le makefile ne fonctionne pas, vous pouvez compiler et lancer le programme manuellement comme ceci:

```bash
#Compilation
cd src
gcc -Wall *.c -o t3c -lcrypto

#Lancement mode G
./t3c G -i dictionnaire -o t3c_output

#Lancement en mode L
./t3c L -t t3c_output

#Exemple avec Valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./t3c G -i dictionnaire -o t3c_output
```

PS: Un pré-requis est donc l'installation du package `libssl-dev` sur Linux (avec `apt install libssl-dev` sur Debian par exemple).



## Autres informations


Sorties et codes de retour:

stdout contient uniquement le résultat utile. Tout log, ou message de progression ou avertissement va normalement sur stderr.

En mode L, le programme produit exactement une ligne par condensat lu, dans le même ordre. Chaîne trouvée : la chaîne. Non trouvée : ligne vide.

Pour toutes les fonctions, code 0 si l'exécution est normale, y compris si des condensats ne sont pas trouvés.

Code non nul en cas d'erreur réelle : argument manquant ou invalide, fichier inexistant ou illisible, etc.



Astuce:

Comme la recherche de condensat (mode L) lit les hash sur stdin, vous pouvez par exemple lancer le code de cette façon:

```bash
cat fichier_de_condensats | ./t3c L -t t3c_output
```



## Limitations


Dû à la limitation de fscanf, j'ai choisi de fixer la taille maximale d'une ligne du dictionnaire à 2048 caractères. Une ligne dépassant cela ne sera pas processée.

Aussi, comme le séparateur est une ligne vide, le fichier dictionnaire doit impérativement se terminer par une ligne vide. Sinon, le dernier condensat du fichier ne sera pas valide.



PS: Makefile et Dockerfile non vérifiés pour le moment.