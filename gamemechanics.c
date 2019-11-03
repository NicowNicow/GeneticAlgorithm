#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <stdint.h> //for intptr_t
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "./GfxLib/BmpLib.h"

//* ------------------  Stuff  ------------------ */

int random_generator(int min, int max) 
{ 
    return((rand()%(max-min+1))+min);
}


/* ------------------  Player Stuff  ------------------ */

void spawn_players(PLAY** players_list)
{
	for (int index=0;index<numberPlayers; index++)
	{
		players_list[index]->Xpos=215;
		players_list[index]->Ypos=120;
		players_list[index]->score=0;
		players_list[index]->jump=10; //Arbitrary value that can't be obtain naturally in the program
		players_list[index]->jumpTime=0;
		players_list[index]->alive=TRUE;
		players_list[index]->keyboard=0;
		for (int index2=0; index2<3; index2++)
		{
			players_list[index]->color[index2]=random_generator(0,255);
		}
	}
}


void move_player(PLAY* player)
{
	if (player->keyboard==0) //No movements
	{
		player->Xpos=player->Xpos;
	}
	if (player->keyboard==1 && player->Xpos+50<largeurFenetre()) // to right
	{
		player->Xpos=player->Xpos+9;
	}
	if (player->keyboard==2 && player->Xpos>0)	//to left
	{
		player->Xpos=player->Xpos-9;
	}
}


void platform_bounce(PLAY* player, PLA** platforms_list)
{
	for (int index=0;index<6;index++)
	{
		if ((((platforms_list[index]->Ypos)+15<=player->Ypos)&&((platforms_list[index]->Ypos)+25>=player->Ypos))&&(((platforms_list[index]->Xpos)-48<=player->Xpos)&&((platforms_list[index]->Xpos)+69>=player->Xpos)))
		{
			player->jump=index;
		}
		if (player->jump!=10)
		{
			break;
		}
	}
	if (player->jump!=10)
	{
		player->Ypos=player->Ypos+5;
		player->jumpTime=1;
		score_up(player, platforms_list);
		rafraichisFenetre();
	}
	else if (player->jumpTime!=0)
	{
		player->Ypos=player->Ypos+5;
		player->jumpTime=player->jumpTime+1;
		if (player->jumpTime>=40)
		{
			player->jumpTime=0;
		}
		rafraichisFenetre();
	}
	else
	{
		player->Ypos=player->Ypos-10;
		rafraichisFenetre();
	}
}


void score_up(PLAY* player, PLA** platforms_list)
{
	if (platforms_list[player->jump]->scorePlat==1)
	{
		player->score=player->score+1;
		platforms_list[player->jump]->scorePlat=0;
	}
}


void death_player(PLAY* player)
{
	if (player->Ypos<=0)
	{
		player->alive=FALSE;
	}
}


/* ------------------  Platform Stuff  ------------------ */


PLA** malloc_platforms_list(void)
{
	PLA** platforms_list;
	platforms_list=(PLA**)malloc(sizeof(PLA*)*7);
	for (int index=0; index<7; index++)
	{
		platforms_list[index]=(PLA*)malloc(sizeof(PLA));
		if (index==0)
		{
			platforms_list[index]->Ypos=100;
			platforms_list[index]->Xpos=random_generator(155,225);
			
		}
		else initial_spawn_platform(platforms_list, index);
	}
	return(platforms_list);
}


void initial_spawn_platform(PLA** platforms_list, int index)
{	
	platforms_list[index]->Ypos=random_generator(100,150)+(platforms_list[index-1]->Ypos);
	platforms_list[index]->Xpos=random_generator(0,410);
	platforms_list[index]->scorePlat=1;
}


void scrolling(PLA** platforms_list, PLAY* player)
{
	static int countdown;
	static BOOL temp;
	if (player->Ypos > 550)
	{
		temp=TRUE;
	}
	if (temp==TRUE)
	{
		if (countdown<=60)
		{
			player->Ypos=player->Ypos-5;
			for (int index=0;index<6;index++)
			{
				platforms_list[index]->Ypos=platforms_list[index]->Ypos-5;
			}
			countdown=countdown+1;
			rafraichisFenetre();
		}
		else
		{
			temp=FALSE;
			countdown=0;
		}
		
	}
	player->jump=10;
	rafraichisFenetre();
}


void check_platforms(PLA** platforms_list)
{
	for (int index=0;index<6;index++){
		if (platforms_list[index]->Ypos<50){
			replace_platform(platforms_list, index);
		}
	}
} 


void replace_platform(PLA** platforms_list, int previousIndex)
{
	int Ymax;
	Ymax=0;
	for (int index=0; index<6;index++)
	{
		if (index==previousIndex)
		{
			index++;
		}
		if (platforms_list[index]->Ypos>=Ymax)
		{
			Ymax=platforms_list[index]->Ypos;
		}
	}
	platforms_list[previousIndex]->Ypos=Ymax+random_generator(70,120);
	if (platforms_list[previousIndex]->Ypos>=900)
	{
		platforms_list[previousIndex]->Ypos=(int)(2*platforms_list[previousIndex]->Ypos)/3;
	}
	platforms_list[previousIndex]->Xpos=random_generator(0,410);
	platforms_list[previousIndex]->scorePlat=1;
}


void desalloc_platforms_list(PLA** platforms_list)
{
	for (int index=0;index<6;index++)
	{
		free(platforms_list[index]);
	}
	free(platforms_list);
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


void draw_player(PLAY* player)
{
	couleurCourante((intptr_t)player->color[0], (intptr_t)player->color[1], (intptr_t)player->color[2]);
	rectangle(player->Xpos, player->Ypos, player->Xpos + 50, player->Ypos + 50);
}


void draw_generation_score(int indexGenerations, int scoreMax)
{
	char displayGeneration[64]="";
	char displayScoreMax[64]="";
	sprintf(displayGeneration, "Generation = %d ", indexGenerations);
	couleurCourante(0,0,0);
	afficheChaine(displayGeneration, 20, 10, 770);
	sprintf(displayScoreMax, "Score Max = %d ", scoreMax);
	afficheChaine(displayScoreMax, 20, 250, 770);
}