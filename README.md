# General Problem Solver (GPS)

Rapport de SAE Algorithmique et Logique
Licence Informatique — 1ère Année
Université de Picardie Jules Verne
Année universitaire 2024-2025

Auteurs : FERHAOUI Sofiane — MEDJOUDJ Haithem

## Introduction

Le contexte de ce projet est de refaire l'un des premiers programmes d'Intelligence Artificielle.
Le General Problem Solver (GPS) a pour objectif de résoudre des problèmes (faits initiaux et but)
à l'aide de règles qui permettent de passer des faits initiaux aux buts.

Nous avons travaillé en binôme en langage C, en découpant le projet en 4 parties.
Nous avons testé et amélioré chaque partie avant de passer à la suivante.

## Partie 1 — Chargement des données

- Manipulation des fichiers en C avec fopen, fgets, fclose
- Utilisation de la fonction parseLine pour découper les lignes en faits
- Structures de données : enregistrements et tableaux pour transformer un fichier texte en données

## Partie 2 — Moteur de raisonnement basique

- applicable : vérifie si on peut appliquer la règle
- appliquer : ajoute et supprime des faits
- but_atteint : vérifie si on est dans l'état final
- trouver_regle : trouve une règle applicable

Ce moteur résout le problème des singes mais pas les autres car il réapplique les règles en boucle.
Nous avons utilisé le backtrack pour mémoriser les états et pouvoir revenir en arrière.

## Partie 3 — Moteur amélioré

Pour résoudre les trois problèmes, nous avons modifié la fonction solve_prblm et utilisé le backtracking.
Il permet de mémoriser les états dans un enregistrement etat et de revenir en arrière sans entrer
dans une boucle infinie.

- chaque état est stocké dans tab_etat
- IR permet de reprendre la recherche à partir d'un indice
- etat_deja_vu vérifie si l'état a déjà été visité pour éviter les cycles

## Partie 4 — Formalisation du problème loup, chèvre, berger

Nous avons formalisé le problème dans un fichier chou.txt.
Nous avons défini les préconditions pour éviter les cas non permis :
- loup et chèvre seuls
- chèvre et chou seuls

Le GPS a réussi à atteindre le but après 5 états.

## Résultats

Le programme résout les trois problèmes : le singe, l'enfant à l'école et les blocs.
Il résout également le problème formalisé en partie 4.
Un menu permet de choisir quelle partie du projet exécuter.

## Conclusion

Dans ce projet, nous avons appris :
- Manipulation des fichiers en C
- Structures de données
- Principe du backtracking dans un système de résolution
- Détection de cycles pour garantir la terminaison de la recherche

## Compilation et exécution

gcc program.c -o gps
./gps

## Auteurs

FERHAOUI Sofiane — 22512969
MEDJOUDJ Haithem — 22514361
