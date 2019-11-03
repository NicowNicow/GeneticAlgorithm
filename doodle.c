#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <time.h>	//For random_generator()
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "./GfxLib/BmpLib.h"
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project



/* ------------------  Used Variables  ------------------ */

#define numberGenerations 10
static PLA** platforms_list;
static PLAY** players_list;
int indexGenerations;
int allPlayersDead;
int scoreMax;


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
			indexGenerations=0;
			allPlayersDead=0;
			scoreMax=0;
			platforms_list = malloc_platforms_list();
			players_list=malloc_players_list();
			demandeTemporisation(20);
			break;

		case Temporisation:
			for (int index=0; index<numberPlayers;index++)
				{
					if (players_list[index]->alive==TRUE)
					{
						platform_bounce(players_list[index], platforms_list);
						scrolling(platforms_list, players_list[index]);
						check_platforms(platforms_list);
						move_bot(players_list[index]);
						death_player(players_list[index]);
						allPlayersDead=0;
					}
					else
					{
						allPlayersDead++;
					}
				}
			scoreMax=best_score(players_list);
			if ((allPlayersDead>=numberPlayers)&&(indexGenerations<numberGenerations))
			{
				regen_platforms_list(platforms_list);
				regen_players_list(players_list);
				allPlayersDead=0;
				indexGenerations++;
			}
		break;

		case Affichage:
			draw_background();
			draw_platforms(platforms_list);
			if (indexGenerations<numberGenerations)	
			{
				draw_generation_score(indexGenerations+1, scoreMax);
			}
			if (indexGenerations==numberGenerations)	//Just so that it does print "Generation X+1" after finishing the X generation
			{
				draw_generation_score(indexGenerations, scoreMax);
			}
			for (int index=0;index<numberPlayers;index++)
			{
				draw_player(players_list[index]);
			}
			demandeTemporisation(20);
			break;

		case Clavier:
			switch (caractereClavier())
			{		
				case 'Q':
				case 'q':
					desalloc_players_list(players_list);
					desalloc_platforms_list(platforms_list);
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
