#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
	char letter;
	int score;
} LETTER;

typedef struct
{
	int pos_x;
	int pos_y;
	LETTER * currentLetter;
} BOX;


void shuffleCharacterInBag(LETTER** bag);

/* Using a fixed board size of 15*15, initialize the board
   and return it*/

BOX *** boardInit()
{
	int i = 0;
	int j = 0;
	BOX *** board = malloc(sizeof(BOX **) * 15 ); 

	for (i = 0; i < 15; i++) 
	{
		board[i] = malloc(sizeof(BOX*) * 15);
		for (j = 0; j < 15; j++)
		{
			board[i][j] = malloc(sizeof(BOX));
			board[i][j]->pos_x = i;
			board[i][j]->pos_y = j;
			board[i][j]->currentLetter = 0;
		} 
	}

	return board;
}

/*function to initialize a bag, call this once in the entire run
  because it need to use the same seed*/
LETTER ** initBag()
{
	/*Variables*/
	LETTER** bag = malloc(100 * sizeof(LETTER*));
	LETTER* newL;
	char occ;
	char score;
	char index = 0;
	char tabL[26][2] = 
	{
		/*{occ, score} letter*/
		{9, 1},  /*a*/
		{2, 3},  /*b*/
		{2, 3},  /*c*/
		{4, 2},	 /*d*/
		{12, 1}, /*e*/
		{2, 4},	 /*f*/
		{3, 2},  /*g*/
		{3, 4},  /*h*/
		{9, 1},	 /*i*/
		{1, 8},	 /*j*/
		{1, 5},  /*k*/
		{4, 1},	 /*l*/
		{2, 3},  /*m*/
		{6, 1},  /*n*/
		{8, 1},  /*o*/
		{2, 3},  /*p*/
		{1, 10}, /*q*/
		{6, 1},  /*r*/
		{4, 1},  /*s*/
		{6, 1},  /*t*/
		{4, 1},  /*u*/
		{2, 4},  /*v*/
		{3, 4},  /*w*/
		{1, 8},  /*x*/
		{2, 4},	 /*y*/
		{1, 10}, /*z*/
	};

	/*Insertion of each letters on the bag*/
	for (size_t i = 0; i < 26; i++) 
	{
		occ = tabL[i][0];
		score = tabL[i][1];
		for (size_t j = 0; j < occ; j++) 
		{
			newL = malloc(sizeof(LETTER));
			newL->letter = i +'a';
			newL->score = score;
			bag[index] = newL;
			index++;
		}	
	}

	return bag;

}

/* function to pull letter and leave 0 instead of ASCII number and return it
   return ASCII number in case of success
   return NULL in case of fail
   in the bag, 0 indicate that that letter has been played on the player's hand or played on the board
   argument bag : the letter bag that will be pulled */
LETTER* pullLetterFromBag(LETTER ** bag)
{
	int i;
	LETTER* selectLetter;
 	shuffleCharacterInBag(bag);

	for(i = 0; i < 100; i++)
		if(bag[i])
		{
			selectLetter = bag[i];
			bag[i] = NULL;
			return selectLetter;
		}

	/* in case characters in the bag run out, will return NULL */
	return NULL;
}

int returnCharacterToBag(LETTER ** bag, LETTER* returnLetter)
{
	int i;

	for(i = 0; i < 100; i++)
	{
		if(!bag[i])
		{
			//memcpy(&bag[i],&returnLetter,sizeof(LETTER*));
			bag[i] = returnLetter;
			return 1;
		} 
	}
	return 0;
}

LETTER ** initHand(LETTER** bag)
{
	int i;
	LETTER** lettersOnHand = malloc(7 * sizeof(LETTER*));

	for(i = 0; i < 7; i++)
	{
		lettersOnHand[i] = pullLetterFromBag(bag);
	}
	return lettersOnHand;
}

void shuffleCharacterInBag(LETTER** bag)
{
	srand(time(NULL));
	int i;
	int size = 100;
	int randomLetter;
	LETTER* tmp;

	for(i = 0; i < 100; i++)
	{ 
		randomLetter = rand() % size;
		tmp = bag[randomLetter];
		bag[randomLetter] = bag[size - 1];
		bag[100 - i - 1] = tmp;
		size--;	
	}
}


/* Argument 1 : The Bag *
 * Argument 2 : Letter on the hand that user want to swap
 * put the letter on the hand back to the bag , then return the new character from the bag
 */
void swapCharacterFromHandToBag(LETTER** bag, LETTER** hand)
{
	int i;
	int j = 0;

	for(i = 0; i < 7; i++)
	{
		while (bag[j] != NULL)
			j++;
		bag[j] = hand[i];
		hand[i] = NULL;
	}
	shuffleCharacterInBag(bag);
	for(i = 0; i < 7; i++)
		hand[i] = bag[i];
	shuffleCharacterInBag(bag);
}


	
