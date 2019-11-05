#ifndef GENETICS_H_
#define GENETICS_H_

#define numberGenerations 10
#define numberPlayers 50



/* ------------------  New Game Stuff  ------------------ */

void move_bot(PLAY* player, PLA** platforms_list); //Used to choose randomly left, right or middle

int bot_eyes(PLAY* player, PLA** platforms_list); //Give to the "bot" the user-accessible information of its relative-position from the next plateform

PLAY** malloc_players_list(void); //used to allocate a list of players

void desalloc_players_list(PLAY** players_list);    //used to free the list of players

void regen_platforms_list(PLA** platforms_list);    //used to generate a new list of platforms

int best_score(PLAY** players_list); //used to get the score of the highest player

void select_keyboard(PLAY* player, int Xrelat); //used to choose the next action of a player

/* ------------------  Genetic Algorithm Stuff  ------------------ */

PLAY crossover(PLAY** players_list);  //Generate a radom mask (format mask[3][3]) and apply it to the best two genomes to creat a new third one

//There's no explicit fitness function as it is determined by the player score, which is accessible in the player structure

void mutation(PLAY** players_list); //Select a random gene in the genome of a random player in order to change it either radically, slightly or not at all

int natural_selection(PLAY** players_list); //Change the genome of the weakest players with the crossover genome

#endif