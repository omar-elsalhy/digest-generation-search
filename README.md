# Générateur et chercheur de condensats

Dû à la limitation de fscanf, j'ai choisi de fixer la taille maximale d'une ligne du dictionnaire à 2048 caractères.
Une ligne dépassant cela sera tronquée. Cela ne gènera pas la recherche de condensat puisque la recherche de condensat sera elle aussi tronquée.
Cela reste une limitation importante.
Finalement, pas d'enregistrement des lignes vides !!
Comme le séparateur est une ligne vide, le fichier dictionnaire doit impérativement se terminer par une ligne vide

