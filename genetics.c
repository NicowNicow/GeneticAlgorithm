#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <time.h>	//For random_generator()
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "./GfxLib/BmpLib.h"
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project


/* ------------------  New Game Stuff  ------------------ */

void move_bot(PLAY* player)
{
    player->keyboard=(int)random_generator(0,2);
    move_player(player);
}


PLAY** malloc_players_list(void)
{
	PLAY** players_list;
	players_list=(PLAY**)malloc(sizeof(PLAY*)*7);
	for (int index=0; index<numberPlayers; index++)
	{
		players_list[index]=(PLAY*)malloc(sizeof(PLAY));
	}
    spawn_players(players_list);
	return(players_list);
}

void desalloc_players_list(PLAY** players_list)
{
	for (int index=0;index<numberPlayers;index++)
	{
		free(players_list[index]);
	}
	free(players_list);
}


void regen_platforms_list(PLA** platforms_list)
{
    for (int index=0; index<7; index++)
	{
		if (index==0)
		{
			platforms_list[index]->Ypos=100;
			platforms_list[index]->Xpos=random_generator(155,225);
			
		}
		else initial_spawn_platform(platforms_list, index);
	}
}


void regen_players_list(PLAY** players_list)
{
    spawn_players(players_list);
}


int best_score(PLAY** players_list)
{
    int scoretemp;
    int bestScore;
    scoretemp=0;
    for (int index=0; index<numberPlayers; index++)
    {
        if (players_list[index]->score>=scoretemp)
        {
            scoretemp=players_list[index]->score;
        }
    }
    bestScore=scoretemp;
    return(bestScore);
}


/* ------------------  Genetic Algorithm Stuff  ------------------ */

