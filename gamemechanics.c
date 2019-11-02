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
	player.jumpTemp=0;
	player.alive=TRUE; 
	for (int index=0; index<3; index++)
	{
		player.color[index]=(intptr_t)random_generator(0,255);
	}
	return(player);
}


PLAY move_player(PLAY player, int keyboard)
{
	if (keyboard==16) // to right
	{
		player.Xpos=player.Xpos+7;
	}
	if (keyboard==15)	//to left
	{
		player.Xpos=player.Xpos-7;
	}
	return(player);
}


void platform_bounce(PLAY* player, PLA** platforms_list)
{
	for (int index=0;index<6;index++)
	{
		if ((((platforms_list[index]->Ypos)+15<=player->Ypos)&&((platforms_list[index]->Ypos)+25>=player->Ypos))&&(((platforms_list[index]->Xpos)-48<=player->Xpos)&&((platforms_list[index]->Xpos)+69>=player->Xpos)))
		{
			player->jump=TRUE;
		}
		printf("plateforme %d, Ypos+20: %d, Xpos-48: %d, Xpos+69: %d\n", index,platforms_list[index]->Ypos+20,platforms_list[index]->Xpos-48,platforms_list[index]->Xpos+69);
		printf("player Ypos: %d, Xpos: %d\n", player->Ypos, player->Xpos);
		printf("%d\n", player->jump);
		if (player->jump==TRUE)
		{
			break;
		}
	}
	printf("======================================\n");
	if (player->jump==TRUE)
	{
		player->Ypos=player->Ypos+5;
		player->jumpTemp=1;
		player->jump=FALSE;
		rafraichisFenetre();
	}
	else if (player->jumpTemp!=0)
	{
		player->Ypos=player->Ypos+5;
		player->jumpTemp=player->jumpTemp+1;
		if (player->jumpTemp>=40)
		{
			player->jumpTemp=0;
		}
		rafraichisFenetre();
	}
	else
	{
		player->Ypos=player->Ypos-10;
		rafraichisFenetre();
	}
}


// void wall_bounce(PLAY* player)
// {

// }


// PLAY score_up(PLAY player)
// {

// }


// PLAY death_player(PLAY player)
// {

// }


// void despawn_player(PLA player)
// {
// 	free();
// }


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


// PLA** spawn_platform(PLA** platforms_list, int index)
// {

// }


// PLA** scrolling(PLA** platforms_list, PLAY player)
// {

// }


// PLA** check_platforms(PLA** platforms_list)
// {

// }


// PLA** despawn_platform(PLA** platforms_list, int index)
// {

// }


// void desalloc_platforms_list(PLA** platform_list)
// {

// }


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