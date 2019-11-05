#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <time.h>	//For random_generator()
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project



/* ------------------  Used Variables  ------------------ */


static PLA** platforms_list;
static PLAY** players_list;
int playersCount;
int bestScore;
int playersExtinction;
int indexGenerations;
int allPlayersDead;
int score;
int indexMaxYPos;
BOOL scrollingActive;


/* ------------------  Main  ------------------ */

int main(int argc, char *argv[])
{
	srand(time(NULL)); //Initialise of the rand() function used in gamemechanics.c
	initialiseGfx(argc, argv);
	prepareFenetreGraphique(argv[argc-1], 480, 900);
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
			playersCount=numberPlayers;
			allPlayersDead=0;
			score=0;
			playersExtinction=0;
			bestScore=0;
			indexMaxYPos=0;
			platforms_list = malloc_platforms_list();
			players_list=malloc_players_list();
			demandeTemporisation(20);
			break;

		case Temporisation:
			for (int index=0; index<numberPlayers;index++)
			{
				if (players_list[index]->Ypos>=550)
				{
					indexMaxYPos=index;
					scrollingActive=TRUE;
					break;
				}
			}
			if (scrollingActive==TRUE)
						{
							scrolling(platforms_list);
							scrolling_player(players_list); //I needed to cut down in two part the scrolling function to get more players
							scrollingActive=FALSE;
						}
			for (int index=0; index<numberPlayers;index++)
				{
					if (players_list[index]->alive==TRUE)
					{
						platform_bounce(players_list[index], platforms_list);
						check_platforms(platforms_list);
						move_bot(players_list[index], platforms_list);
						playersCount=death_player(players_list[index],playersCount);
						allPlayersDead=0;
					}
					else
					{
						allPlayersDead++;
					}
				}
			score=best_score(players_list);
			if (allPlayersDead>=numberPlayers)
			{
				indexGenerations++;
			}
			if ((allPlayersDead>=numberPlayers)&&(indexGenerations<numberGenerations))
			{
				for (int index=0;index<numberPlayers;index++) //keeping the best score of this generation to print it on the screen
				{
					if (players_list[index]->score>=bestScore)
					{
						bestScore=players_list[index]->score;
					}
				}
				playersExtinction=natural_selection(players_list);
				regen_platforms_list(platforms_list);
				spawn_players(players_list);	//Does not modify the genome of the players; only the coordinates, state (alive/dead, jumping/on the ground) and color
				allPlayersDead=0;
				playersCount=numberPlayers;
			}
		break;

		case Affichage:
			draw_background();
			draw_platforms(platforms_list);
			if (indexGenerations<numberGenerations)	
			{
				draw_generation_score(indexGenerations+1, score, playersCount, playersExtinction, bestScore);
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

				case 'R':	//Just in case a player block itself: it reloads the current generation process
				case 'r':
					regen_platforms_list(platforms_list);
					spawn_players(players_list);	//Does not modify the genome of the players; only the coordinates, state (alive/dead, jumping/on the ground) and color
					allPlayersDead=0;
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
