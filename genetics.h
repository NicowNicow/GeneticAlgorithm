#ifndef GENETICS_H_
#define GENETICS_H_

#define numberPlayers 50


/* ------------------  Drawing Stuff  ------------------ */

void move_bot(PLAY* player); //Used to choose randomly left, right or middle

PLAY** malloc_players_list(void); //used to allocate a list of players

void desalloc_players_list(PLAY** players_list);    //used to free the list of players

void regen_platforms_list(PLA** platforms_list);    //used to generate a new list of platforms

void regen_players_list(PLAY** players_list);   //used to generate a new list of players

int best_score(PLAY** players_list); //used to get the score of the highest player


/* ------------------  Genetic Algorithm Stuff  ------------------ */

#endif