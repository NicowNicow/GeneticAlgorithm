#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <stdint.h> //for intptr_t
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project
#include "./GfxLib/GfxLib.h" // To do simple graphics

//* ------------------  Stuff  ------------------ */

float random_float_generator(float value) 
{ 
    return(((float)rand()/(float)RAND_MAX)*value);
}


int random_int_generator(int min, int max) 
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
			players_list[index]->color[index2]=random_int_generator(0,255);
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
		player->Xpos=player->Xpos+lateralSpeed;
	}
	if (player->keyboard==2 && player->Xpos>0)	//to left
	{
		player->Xpos=player->Xpos-lateralSpeed;
	}
}


void platform_bounce(PLAY* player, PLA** platforms_list)
{
	for (int index=0;index<6;index++)
	{
		if ((((platforms_list[index]->Ypos)+15<=player->Ypos)&&((platforms_list[index]->Ypos)+25>=player->Ypos))&&(((platforms_list[index]->Xpos)-48<=player->Xpos)&&((platforms_list[index]->Xpos)+69>=player->Xpos)))
		{
			player->jump=index;
		    score_up(player);
		}
		if (player->jump!=10)
		{
			break;
		}
	}
	if (player->jump!=10)
	{
		player->Ypos=player->Ypos+jumpSpeed;
		player->jumpTime=1;
		rafraichisFenetre();
	}
	else if (player->jumpTime!=0)
	{
		player->Ypos=player->Ypos+jumpSpeed;
		player->jumpTime=player->jumpTime+1;
		if (player->jumpTime>=40)
		{
			player->jumpTime=0;
		}
		rafraichisFenetre();
	}
	else
	{
		player->Ypos=player->Ypos-fallSpeed;
		rafraichisFenetre();
	}
}


void score_up(PLAY* player)
{
	player->score=player->score+1;
}


int death_player(PLAY* player, int playersCount)
{
	if (player->Ypos<=0)
	{
		player->alive=FALSE;
		playersCount=playersCount-1;
	}
	return(playersCount);
}


/* ------------------  Platform Stuff  ------------------ */


PLA** malloc_platforms_list(void)
{
	PLA** platforms_list;
	platforms_list=(PLA**)malloc(sizeof(PLA*)*7); //IDK why, but if I dont malloc 7 slots, even if we only use 6, I get a Segfault
	for (int index=0; index<7; index++)
	{
		platforms_list[index]=(PLA*)malloc(sizeof(PLA));
		if (index==0)
		{
			platforms_list[index]->Ypos=100;
			platforms_list[index]->Xpos=random_int_generator(155,225);
			
		}
		else initial_spawn_platform(platforms_list, index);
		if (index==6)
		{
			platforms_list[index]->Xpos=0;	//Allocation of the 7th slot to (0,0) so that it doesn't mess with replace_platform()
			platforms_list[index]->Ypos=0; //Temporary solution tho, I really need to find what cause the Segfault
		}
	}
	return(platforms_list);
}


void initial_spawn_platform(PLA** platforms_list, int index)
{	
	platforms_list[index]->Ypos=random_int_generator(100,150)+(platforms_list[index-1]->Ypos);
	platforms_list[index]->Xpos=random_int_generator(0,410);
}


void scrolling(PLA** platforms_list)
{
	static int countdownPlat;
	if (countdownPlat<=60)
	{
		for (int index=0;index<6;index++)
		{
			platforms_list[index]->Ypos=platforms_list[index]->Ypos-5;
		}
		countdownPlat=countdownPlat+1;
	}
	else
	{
		countdownPlat=0;
	}
	rafraichisFenetre();
}


void scrolling_player(PLAY** players_list)
{
	static int countdownPlay;
	if (countdownPlay<=60)
	{
		for (int index=0;index<numberPlayers;index++)
		{
			players_list[index]->Ypos=players_list[index]->Ypos-5;
		}
		countdownPlay=countdownPlay+1;
	}
	else
	{
		countdownPlay=0;
	}
	for (int index=0;index<numberPlayers;index++) //Player->jump designate the platform on which the player is jumping
	{											  //During the scrolling, the player is not on any platform, hence the reset here
		players_list[index]->jump=10;
	}
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
	platforms_list[previousIndex]->Ypos=Ymax+random_int_generator(70,120);
	if (platforms_list[previousIndex]->Ypos>=900)
	{
		platforms_list[previousIndex]->Ypos=(int)(2*platforms_list[previousIndex]->Ypos)/3;
	}
	platforms_list[previousIndex]->Xpos=random_int_generator(0,410);
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
	rectangle(0, 0, 480, 900);
}


void draw_platforms(PLA** platforms_list)
{
	couleurCourante(0, 120, 0);
	for(int index=0;index<6;index++)
	{
		rectangle(platforms_list[index]->Xpos, platforms_list[index]->Ypos, platforms_list[index]->Xpos+70,platforms_list[index]->Ypos+20);
	}
}


void draw_player(PLAY* player)
{
	couleurCourante((intptr_t)player->color[0], (intptr_t)player->color[1], (intptr_t)player->color[2]);
	rectangle(player->Xpos, player->Ypos, player->Xpos + 50, player->Ypos + 50);
}


void draw_generation_score(int indexGenerations, int score, int playersCount, int playersExtinction, int bestScore)
{
	char displayGeneration[64]="";
	char displayScore[64]="";
	char displayPlayersCount[64]="";
	char displayPlayersExtinction[64]="";
	char displaybestScore[64]="";
	sprintf(displayPlayersExtinction, "Players Modified = %d ", playersExtinction);
	sprintf(displayGeneration, "Generation = %d ", indexGenerations);
	sprintf(displayPlayersCount, "Players left = %d", playersCount);
	sprintf(displayScore, "Score = %d ", score);
	sprintf(displaybestScore, "Best Score = %d", bestScore);
	couleurCourante(0,0,0);
	//First Line
	afficheChaine(displayGeneration, 20, 10, 820);
	afficheChaine(displaybestScore, 20, 220, 820);
	//Second Line
	afficheChaine(displayPlayersExtinction, 20,10,790); 
	afficheChaine(displayPlayersCount, 20, 280,790);
	//Third Line
	afficheChaine(displayScore, 20, 180, 760);
	rectangle(0, 752, 480, 753);
}