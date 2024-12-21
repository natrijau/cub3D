# CUB3D

Bienvenue dans le dépôt regroupant les travaux réalisés dans le cadre du projet Cub3D de 42. L'objectif est de créer une représentation graphique 3D réaliste d'un labyrinthe en vue subjective, en appliquant les principes du ray-casting.

---

## Table des matières

1. [Introduction](#introduction)
2. [Installation et utilisation](#installation-et-utilisation)
3. [Ressources utiles](#ressources-utiles)
4. [Règles communes](#règles-communes)
5. [Mon projet](#Mon-projet)

---

## Introduction

Les objectifs de ce projet sont similaires à tous les autres projets et leurs objectifs :
Rigueur, utilisation du C, utilisation d’algorithmes basiques, recherche d’informations, etc.
Comme c’est un projet de design graphique, Cub3D vous permettra de travailler vos talents de designer : fenêtres, couleurs, événements, formes, etc.
En conclusion, Cub3D est une aire de jeu remarquable pour explorer les applications pratiques des mathématiques sans avoir à en comprendre les spécificités.
Avec l’aide des nombreux documents disponibles sur internet, vous utiliserez les mathématiques en tant qu’outil de création d’algorithmes élégants et efficaces.

Pour une introduction sur le raycasting, référez-vous à [l'article Wikipédia sur le raycasting](https://fr.wikipedia.org/wiki/Raycasting#:~:text=Le%20ray%20casting%20est%20une,par%20une%20carte%20graphique%20d%C3%A9di%C3%A9e.).

---

## Installation et utilisation

1. Clonez le dépôt :
   ```bash
   git clone <lien-du-depot>
   cd <dossier-du-depot>

2. Pour compiler le projet, utilisez le Makefile fourni :
   ```bash
	make bonus

3. Exécutez le programme généré en fournissant un fichier de description de map avec l'extension .cub  :
	```bash
	./cub3d maps/bonus/map_greencyber.cub

## Ressources utiles

- [Documentation MinilibX](https://harm-smits.github.io/42docs/libs/minilibx)
- [Tutoriel sur le raycasting](https://lodev.org/cgtutor/raycasting.html)
- [Cub3D Tutorial](https://medium.com/@afatir.ahmedfatir/cub3d-tutorial-af5dd31d2fcf)

## Règles communes

- Votre projet doit être écrit en C.
- Votre projet doit être codé à la Norme. 
- Si vous avez des fichiers ou fonctions bonus, celles-ci seront inclues dans la vérification de la norme et vous aurez 0 au projet en cas de faute de norme.
- Vos fonctions ne doivent pas s’arrêter de manière inattendue (segmentation fault, bus error, double free, etc) mis à part dans le cas d’un comportement indéfini. Si cela arrive, votre projet sera considéré non fonctionnel et vous aurez 0 au projet.
- Toute mémoire allouée sur la heap doit être libérée lorsque c’est nécessaire. Aucun leak ne sera toléré.
- Si le projet le demande, vous devez rendre un Makefile qui compilera vos sources pour créer la sortie demandée, en utilisant les flags -Wall, -Wextra et -Werror. Votre Makefile ne doit pas relink.
- Si le projet demande un Makefile, votre Makefile doit au minimum contenir les règles $(NAME), all, clean, fclean et re.
- Pour rendre des bonus, vous devez inclure une règle bonus à votre Makefile qui ajoutera les divers headers, librairies ou fonctions qui ne sont pas autorisées dans la partie principale du projet. Les bonus doivent être dans un fichier différent : _bonus.{c/h}. L’évaluation de la partie obligatoire et de la partie bonus sont faites séparément.
- Si le projet autorise votre libft, vous devez copier ses sources et son Makefile associé dans un dossier libft contenu à la racine. Le Makefile de votre projet doit compiler la librairie à l’aide de son Makefile, puis compiler le projet.
- Nous vous recommandons de créer des programmes de test pour votre projet, bien que ce travail ne sera pas rendu ni noté. Cela vous donnera une chance de tester facilement votre travail ainsi que celui de vos pairs.
- Vous devez rendre votre travail sur le git qui vous est assigné. Seul le travail déposé sur git sera évalué.

  ## Mon projet

  [![Démonstration]([https://img.youtube.com/vi/<ID_DE_LA_VIDEO>/0.jpg](https://www.youtube.com/watch?v=7nkBCA-63H0))](https://www.youtube.com/watch?v=7nkBCA-63H0)
