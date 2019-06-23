Projet Microcontroleur - Nicolas Sueur - IMR1
Lien du projet GitHub en cas de problèmes : https://github.com/Fivenn/lpc1768_project

Ce projet est un jeu qui consiste à taper le plus de fois sur un bouton en un temps restreint
(5 secondes ici) dans le but d'augmenter un score. Le plus grand score d'une session de jeu 
est sauvegardé dans un highscore.

Pour arriver à l'aboutissement de ce jeu, j'ai utilisé un écran LCD tactile sur lequel
mon jeu est affiché et sur lequel on peut intéragir. Pour pouvoir stocker le highscore d'une
session de jeu, j'ai utilisé la mémoire I2C afin de le stocker. Pour pouvoir scrupter le
tactile de l'écran, j'ai utilisé un timer en mode interruption qui me permet de récupérer 
les coordonnées d'un appui tactile toutes les 10ms.

Pour installer le projet chez vous, il est nécéssaire de remplacer le dossier "User" d'un
projet "Keil_depart" (j'ai utilisé le Keil_depart fourni aux info1 car il comportait des 
librairies avec des fonctions préfaites pour écrire sur l'écran). Une fois cela fait, il ne
reste plus qu'à lancer Keil uvison, compiler et lancer le projet sur la carte Open1768. De 
plus, j'ai choisi de laisser l'ensemble de mes fonctions dans le fichier "main.c" afin de 
réduire tout éventuel problème à l'édition de liens.

! Attention ! : L'écran tactile fourni avec ma carte comporte quelques problèmes au niveau
de la scruptation de l'écran. Cela peut donc avoir un impact sur un autre écran.