#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <stdint.h> //for intptr_t
#include "gamemechanics.h" // For all things game-related
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "./GfxLib/BmpLib.h"

//* ------------------  Stuff  ------------------ */

int random_generator(int min, int max) 
{ 
    return((rand()%(max-min+1))+min);
}


/* ------------------  Player Stuff  ------------------ */
PLAY spawn_player(void)
{
	PLAY player;
	player.Xpos=215;
	player.Ypos=120;
	player.score=0;
	player.jump=TRUE;
	player.alive=TRUE; 
	for (int index=0; index<3; index++)
	{
		player.color[index]=(int)random_generator(0,255);
	}
	return(player);
}


PLAY move_player(PLAY player, int keyboard)
{
	if (keyboard==16) // to right
	{
		player.Xpos=player.Xpos+5;
	}
	if (keyboard==15)	//to left
	{
		player.Xpos=player.Xpos-5;
	}
	return(player);
}


void platform_bounce(PLAY* player, PLA** platforms_list)
{
	int posTemp;
	for (int index=0;index<7;index++)
	{
		if (player->Xpos >= platforms_list[index]->Xpos && player->Xpos <= platforms_list[index]->Xpos+70 && player->Ypos == platforms_list[index]->Ypos+20){
			player->jump=TRUE;
			posTemp=player->Ypos;
			printf(" Jump = %d", player->jump);
			break;
		}
	}
	if (player->jump==TRUE)
	{
		if (player->Ypos-posTemp<200){
			player->Ypos=player->Ypos+5;
			rafraichisFenetre();
		}
	}
	else
	{
		if (player->Ypos>0)
		{
			player->Ypos=player->Ypos-10;
			rafraichisFenetre();
		}
	}
}


void wall_bounce(PLAY* player)
{

}


PLAY score_up(PLAY player)
{

}


PLAY death_player(PLAY player)
{

}


void despawn_player(PLA player)
{
	free(player);
}


/* ------------------  Platform Stuff  ------------------ */




PLA** malloc_platforms_list(void)
{
	PLA** platforms_list=NULL;
	platforms_list=(PLA**)malloc(sizeof(PLA*)*7);
	for (int index=0; index<7; index++)
	{
		platforms_list[index]=(PLA*)malloc(sizeof(PLA));
		if (index==0)
		{
			platforms_list[index]->Ypos=100;
			platforms_list[index]->Xpos=random_generator(155,225);
			
		}
		else platforms_list=initial_spawn_platform(platforms_list, index);
	}
	return(platforms_list);
}


PLA** initial_spawn_platform(PLA** platforms_list, int index)
{	
	platforms_list[index]->Ypos=random_generator(100,150)+(platforms_list[index-1]->Ypos);
	platforms_list[index]->Xpos=random_generator(0,410);
	return(platforms_list);
}


PLA** spawn_platform(PLA** platforms_list, int index)
{

}


PLA** scrolling(PLA** platforms_list, PLAY player)
{

}


PLA** check_platforms(PLA** platforms_list)
{

}


PLA** despawn_platform(PLA** platforms_list, int index)
{

}


vois desalloc_platforms_list(PLA** platform_list)
{

}


/* ------------------  Drawing Stuff  ------------------ */

void draw_background(void)
{
	couleurCourante(255, 255, 255);
	rectangle(0, 0, 480, 800);
}


void draw_platforms(PLA** platforms_list)
{
	couleurCourante(0, 120, 0);
	for(int index=0;index<7;index++)
	{
		rectangle(platforms_list[index]->Xpos, platforms_list[index]->Ypos, platforms_list[index]->Xpos+70,platforms_list[index]->Ypos+20);
	}
}


void draw_player(PLAY player)
{
	couleurCourante((intptr_t)player.color[0], (intptr_t)player.color[1], (intptr_t)player.color[2]);
	rectangle(player.Xpos, player.Ypos, player.Xpos + 50, player.Ypos + 50);
}