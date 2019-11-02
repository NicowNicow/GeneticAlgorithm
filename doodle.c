#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <time.h>	//For random_generator()
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "./GfxLib/BmpLib.h"
#include "gamemechanics.h"



/* ------------------  Used Variables  ------------------ */

static PLA** platforms_list;
static PLAY player;
int keyboard;


/* ------------------  Main  ------------------ */

int main(int argc, char *argv[])
{
	srand(time(NULL)); //Initialise of the rand() function used in gamemechanics.c
	initialiseGfx(argc, argv);
	prepareFenetreGraphique(argv[argc-1], 480, 800);
	lanceBoucleEvenements();
	return 0;
}


/* ------------------  Events Loop  ------------------ */

void gestionEvenement(EvenementGfx event)
{
	switch (event)
	{
		case Initialisation:
			platforms_list = malloc_platforms_list();
			player=spawn_player();
			demandeTemporisation(20);
			break;

		case Temporisation:
			platform_bounce(&player, platforms_list);
/* 			if (Ypos==0){
				Jump=TRUE;
				printf("Test d'aller %d\n", Jump);
			} */
			/* if (Jump==TRUE){
				if (Ypos<200){
					printf("Test lol %d\n", Ypos);
					Ypos=Ypos+5;
					rafraichisFenetre();
				}
				if (Ypos>=200){
					Jump=FALSE;
					printf("Test de retour %d\n", Jump);
				}	
			}
			if (Jump==FALSE){
				if (Ypos>0){
					Ypos=Ypos-10;
					rafraichisFenetre();	
				}
				
			} */
			keyboard=toucheClavier();
			player=move_player(player,keyboard);
			break;

		case Affichage:
			draw_background();
			draw_platforms(platforms_list);
			draw_player(player);
			demandeTemporisation(20);
			break;

		case Clavier:
			switch (caractereClavier())
			{		
				case 'Q':
				case 'q':
					termineBoucleEvenements();
					break;
			}
			break;

		case ClavierSpecial:
			break;
		case BoutonSouris:
			break;
		case Souris:
			break;
		case Inactivite:
			break;
		case Redimensionnement:
			break;
	}

}
