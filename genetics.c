#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include "./GfxLib/GfxLib.h" // To do simple graphics
#include "gamemechanics.h" // For all things game-related
#include "genetics.h"	//For all the AI part of the project


/* ------------------  New Game Stuff  ------------------ */

void move_bot(PLAY* player, PLA** platforms_list)
{
    int Xrelat;
    Xrelat=bot_eyes(player, platforms_list);
    select_keyboard(player, Xrelat);
    move_player(player);
}


int bot_eyes(PLAY* player, PLA** platforms_list)
{
    int XrelatPos;
    int indexCloser;
    if (player->jump==10) //the initial situation
    {
        return(0);
    }
    else if (player->jump==5) //Max platform
    {
        indexCloser=0;
    }
    else
    {
        indexCloser=player->jump+1;
    }
    XrelatPos=player->Xpos-platforms_list[indexCloser]->Xpos;
    return(XrelatPos);
}


PLAY** malloc_players_list(void)
{
	PLAY** players_list;
	players_list=(PLAY**)malloc(sizeof(PLAY*)*numberPlayers);
	for (int index=0; index<numberPlayers; index++)
	{
		players_list[index]=(PLAY*)malloc(sizeof(PLAY));
	}
    spawn_players(players_list);
    for (int index=0; index<numberPlayers; index++)
	{   for (int indexDepth=0; indexDepth<3; indexDepth++)
        {
            for (int indexLine=0; indexLine<3;indexLine++)
            {
                for (int indexColumn=0; indexColumn<3;indexColumn++)
                {
                    players_list[index]->genome[indexDepth][indexLine][indexColumn]=random_float_generator(1);
                }
            }
        }
	}
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
    for (int index=0; index<6; index++)
	{
		if (index==0)
		{
			platforms_list[index]->Ypos=100;
			platforms_list[index]->Xpos=random_int_generator(155,225);
			
		}
		else initial_spawn_platform(platforms_list, index);
	}
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


void select_keyboard(PLAY* player, int Xrelat)
{
    int indexLine;
    int indexDepth;
    int indexTemp;
    indexDepth=-1;
    indexLine=-1;
    indexTemp=0;
    //Select the appropriate column in the gene tab of the player (Allow the player to know the relative position of the next platform)
    if (Xrelat<-25)   
    {
        indexLine=0;
    }
    else if ((Xrelat>=-25)&&(Xrelat<=46)) //At the middle of a platform, Xrelat=-48 or Xrelat=69 (depending on the position of the platform), so we're just taking a little marge
    {
        indexLine=1;
    }
    else if (Xrelat>46)
    {
        indexLine=2;
    }
    //Select the appropriate depth (allow the player to know their jump Time)
    if ((player->jumpTime<10)&&(player->jumpTime!=0))
    {
        indexDepth=0;
    }
    else if ((player->jumpTime>=10)&&(player->jumpTime<=30)) 
    {
        indexDepth=1;
    }
    else if ((player->jumpTime>30)||(player->jumpTime==0))
    {
        indexDepth=2;
    }
    //Find the maximum value in the selected column & depth (The player will follow the action gave to him by this maximum value)
    for (int indexColumn=0;indexColumn<3;indexColumn++)
    {
        if (indexColumn==0)
        {
            indexTemp=0;
        }
        else if (player->genome[indexDepth][indexLine][indexColumn]<=player->genome[indexDepth][indexLine][indexTemp])
        {
            indexTemp=indexColumn;
        }
    }
    //select the keyboard key according to the biggest value 
    switch(indexTemp)
    {
        case 0:
            player->keyboard=2; //To the left
            break;
        case 1:
            player->keyboard=0; //No movement
            break;
        case 2:
            player->keyboard=1; //To the right
            break;
    }
}


/* ------------------  Genetic Algorithm Stuff  ------------------ */

PLAY crossover(PLAY** players_list)
{
    PLAY playerTempo; //Used to easily return the nex genome at the end of the function
    int mask[3][3][3];
    int maskInverted[3][3][3];
    int indexPlayer1;
    int indexPlayer2;
    // generation of a random mask
    for (int indexDepth=0; indexDepth<3; indexDepth++)
    {
        for (int indexLine=0;indexLine<3;indexLine++)
        {
            for (int indexColumn=0; indexColumn<3; indexColumn++)
            {
                mask[indexDepth][indexLine][indexColumn]=random_int_generator(0,1);
            }
        }
    }
    // creation of an inverted mask
    for (int indexDepth=0; indexDepth<3; indexDepth++)
    {
        for (int indexLine=0;indexLine<3;indexLine++)
        {
            for (int indexColumn=0; indexColumn<3; indexColumn++)
            {
                switch(mask[indexDepth][indexLine][indexColumn])
                {
                    case 0:
                        maskInverted[indexDepth][indexLine][indexColumn]=1;
                        break;

                    case 1:
                        maskInverted[indexDepth][indexLine][indexColumn]=0;
                        break;
                        
                }
            }
        }
    }
    //Selection of the two best players by their score
    indexPlayer1=0;
    indexPlayer2=1;
    for (int index=0;index<numberPlayers;index++)
    {
        if ((players_list[indexPlayer1]->score<=players_list[index]->score)&&(players_list[index]->score!=players_list[indexPlayer2]->score))
        {
            indexPlayer1=index;
        }
        if ((players_list[indexPlayer2]->score<=players_list[index]->score)&&(players_list[index]->score!=players_list[indexPlayer1]->score))
        {
            indexPlayer2=index;
        }
    }
    //Application of the mask
    for (int indexDepth=0; indexDepth<3; indexDepth++)
    {
        for (int indexLine=0;indexLine<3;indexLine++)
        {
            for (int indexColumn=0;indexColumn<3;indexColumn++)
            {
                playerTempo.genome[indexDepth][indexLine][indexColumn]=((players_list[indexPlayer1]->genome[indexDepth][indexLine][indexColumn])*(mask[indexDepth][indexLine][indexColumn])+(players_list[indexPlayer2]->genome[indexDepth][indexLine][indexColumn])*(maskInverted[indexDepth][indexLine][indexColumn]));
            }
        }
    }
    return(playerTempo);
}


void mutation(PLAY** players_list)
{
    int mutationsNumber;
    int randomDepthIndex;
    int randomLineIndex;
    int randomColumnIndex;
    int mutationChances;
    float slightMutationTemp;
    mutationsNumber=random_int_generator(2,10); //Random selection of the number of potential mutations
    int toMutateIndexes[mutationsNumber-1];
    for (int index=0; index<mutationsNumber-1; index++)
    {
        toMutateIndexes[index]=random_int_generator(0, numberPlayers-1); //Random selection of the player to potentially mutate
    }
    for (int index=0; index<mutationsNumber-1; index++)
    {
        randomLineIndex=random_int_generator(0,2); //Those three are for choosing randomly a
        randomColumnIndex=random_int_generator(0,2); // gene from a player and mutate it
        randomDepthIndex=random_int_generator(0,2);
        mutationChances=random_int_generator(0,2);
        switch(mutationChances)
        {
            case 0: //no mutations
                break;
            
            case 1: //Slight mutation, adapted from the algogen one
                slightMutationTemp=(random_float_generator(1)-0.05)+(players_list[toMutateIndexes[index]]->genome[randomDepthIndex][randomLineIndex][randomColumnIndex]);
                if (slightMutationTemp<=0)
                {
                    slightMutationTemp=0;
                }
                if (slightMutationTemp>=1)
                {
                    slightMutationTemp=1;
                }
                players_list[toMutateIndexes[index]]->genome[randomDepthIndex][randomLineIndex][randomColumnIndex]=slightMutationTemp;        
                break;
            
            case 2: //Random mutation
                players_list[toMutateIndexes[index]]->genome[randomDepthIndex][randomLineIndex][randomColumnIndex]=random_float_generator(1);
                break;
        }

    }
}


int natural_selection(PLAY** players_list)
{
    PLAY storeNewGenome;
    int minimalValue;
    int mediumValue;
    int maximalValue;
    int playersExtinction; //Used to print the number of changed players
    playersExtinction=0;
    storeNewGenome=crossover(players_list);
    minimalValue=32000; //Arbitrary value that can't be reach by a mistrained player
    maximalValue=0;
    //Determination of a medium score value
    for (int index=0; index<numberPlayers;index++)
    {
        if (players_list[index]->score<=minimalValue)
        {
            minimalValue=players_list[index]->score;
        }
        if (players_list[index]->score>=maximalValue)
        {
            maximalValue=players_list[index]->score;
        }
    }
    mediumValue=(0.99*maximalValue-1.22*minimalValue)/2; //Values chosen after some tests
    //Determination of all the indexes of players whose genomes must be changed by the crossover one and replacement of those genomes
    for (int index=0; index<numberPlayers;index++)
    {
        if (players_list[index]->score<mediumValue)
        {
            for (int indexDepth=0; indexDepth<3; indexDepth++)
            {
                for (int indexLine=0;indexLine<3;indexLine++)
                {
                    for (int indexColumn=0;indexColumn<3;indexColumn++)
                    {
                        players_list[index]->genome[indexDepth][indexLine][indexColumn]=storeNewGenome.genome[indexDepth][indexLine][indexColumn];
                    }
                }
            }
            playersExtinction=playersExtinction+1;
        }
    }
    mutation(players_list);
    return(playersExtinction);
}