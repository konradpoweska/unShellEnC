## Shell en C
Projet Systèmes - 8 juin 2018

L'objectif de ce projet était de recréer un shell en implémentant des fonctionnalités de base du shell Unix.

Nous avons décidé de nous concentrer sur les parties A et B du sujet, c'est-à-dire la boucle d'interaction, et les commandes internes :
- les redirections d'entrée, sortie, erreurs
- les multiples tubes, permettant d'enchaîner plusieurs commandes
- les commandes `cd`, `exit`, `ls`, `cat`, `find`

### Compilation et lancement
Le `Makefile` fourni permet d'automatiser d'automatiser la compilation. Il permet de compiler les différents modules nécessaires pour le fichier souhaité.
Pour compiler tout le programme, il suffit de taper :
```
make
```
Ensuite, pour lancer,
```
./mini-shell
```

### Fonctionnalités

- Possibilité de séparer l'entrée en plusieurs lignes, grâce au caractère d'échappement `\`
- Possibilité de quitter le shell avec un caractère `EOF` _(Ctrl+D)_
- Prise en charge des guillemets, pour les chaînes de caractères comportant des espaces
- Possibilité de rediriger la sortie d'un programme dans un fichier avec `>`
Exemple : `ls > resultat.txt`
De même pour l'entrée, avec `<`, et la sortie d'erreurs avec `2>`
- Possibilité de relier plusieurs commandes à l'aide d'un tube
Exemple : `cat < resultat.txt | grep key | less`
La sortie de chaque commande sera l'entrée de la commande suivante

#### Commandes internes
 - `cd`
 - `exit`
 - `ls` (et ses options `-a` et `-l`)
 - `cat`
 - `find`


### Bugs connus

- `cat < test.txt | cat` quitte le shell
C'est sûrement dû à un EOF restant dans le buffer d'entrée, qui a son effet lors de la demande d'une nouvelle entrée
- `find` essaye de faire la recherche récursive dans les fichiers :(

### To-do
- Corriger les bugs listés ci-dessus
- Implémenter l'option `-exec` pour la commande `find`
