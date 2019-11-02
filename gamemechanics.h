#ifndef GAMEMECHANICS_H_
#define GAMEMECHANICS_H_


/* ------------------  Stuff  ------------------ */

typedef int BOOL;   //Definition of a BOOL type for the project
#define TRUE  1
#define FALSE 0


typedef struct PLATFORM //Definition of a platform structure
{
    int Xpos;
    int Ypos;
} PLA;


typedef struct PLAYER   //Definition of a player structure
{
	int Xpos;
    int Ypos;
	int score;
    BOOL jump;
	BOOL alive;
    int* color[3];
} PLAY;


int random_generator(int min, int max); //Give a random integer in the [min,max] range


/* ------------------  Player Stuff  ------------------ */

PLAY spawn_player(void);   //Set up the spawn of a player

PLAY move_player(PLAY player, int keyboard); //Allow to move the player from right to left

void platform_bounce(PLAY* player, PLA** platforms_list); //Make a player bounce as soon as he touches a platform

void wall_bounce(PLAY* player);    //Make a player bounce off the walls when in contact

PLAY score_up(PLAY player);    //Incrementation of the score of each player when they climb

PLAY death_player(PLAY player); //Change the status of a player to dead as soons as he fells off the screen

void despawn_player(PLAY player);  //Erase the player, his score, ...



/* ------------------  Plateforme Stuff  ------------------ */

PLA** malloc_platforms_list(void);  //Set up a Platform List with up to 6 slots

PLA** spawn_platform(PLA** platforms_list, int index);  //Allow a platform to spawn as soon as a slot is free in the list

PLA** initial_spawn_platform(PLA** platforms_list, int index); //Set the spawn position of the initial platforms

PLA** scrolling(PLA** platforms_list, PLAY player); //Used to pull down platforms as the player go up

PLA** check_platform(PLA** platforms_list);  //Check if a platform in the list is at level 0, in which case it triggers its despawn and a respawn of a new one

PLA** despawn_platform(PLA** platforms_list, int index);    //Make a platform despawn as soon as it fells of the screen

void desalloc_platforms_list(PLA** platforms_list); //Free all the platforms list


/* ------------------  Drawing Stuff  ------------------ */

void draw_background(void); //Draw the background

void draw_platforms(PLA** platforms_list);  //Draw the platforms

void draw_player(PLAY player);  //Draw the player


#endif