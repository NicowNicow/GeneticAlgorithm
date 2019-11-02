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
    int scorePlat;
} PLA;


typedef struct PLAYER   //Definition of a player structure
{
	int Xpos;
    int Ypos;
	int score;
    int jump;
    int jumpTime;
	BOOL alive;
    int* color[3]= {0};
} PLAY;


int random_generator(int min, int max); //Give a random integer in the [min,max] range


/* ------------------  Player Stuff  ------------------ */

PLAY spawn_player(void);   //Set up the spawn of a player

PLAY move_player(PLAY player, int keyboard); //Allow to move the player from right to left

void platform_bounce(PLAY* player, PLA** platforms_list); //Make a player bounce as soon as he touches a platform

void score_up(PLAY* player, PLA** platforms_list);    //Incrementation of the score of each player when they climb

PLAY death_player(PLAY player); //Change the status of a player to dead as soons as he fells off the screen


/* ------------------  Plateforme Stuff  ------------------ */

PLA** malloc_platforms_list(void);  //Set up a Platform List with up to 6 slots

void spawn_platform(PLA** platforms_list, int index);  //Allow a platform to spawn as soon as a slot is free in the list

PLA** initial_spawn_platform(PLA** platforms_list, int index); //Set the spawn position of the initial platforms

void scrolling(PLA** platforms_list, PLAY* player); //Used to pull down platforms as the player go up

void check_platform(PLA** platforms_list);  //Check if a platform in the list is at level 0, in which case it triggers its despawn and a respawn of a new one

void despawn_platform(PLA** platforms_list, int index);    //Make a platform despawn as soon as it fells of the screen

void desalloc_platforms_list(PLA** platforms_list); //Free all the platforms list


/* ------------------  Drawing Stuff  ------------------ */

void draw_background(void); //Draw the background

void draw_platforms(PLA** platforms_list);  //Draw the platforms

void draw_player(PLAY player);  //Draw the player

void write_score(PLAY player); //Write the score on the screen


#endif