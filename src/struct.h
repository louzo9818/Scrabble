#ifndef STRUCT_H
#define STRUCT_H


typedef struct
{
	char letter;
	int index;
	int score;
} LETTER;

typedef struct
{
	int pos_x;
	int pos_y;
	int effect;
	LETTER * currentLetter;
} BOX;

BOX*** boardInit();
LETTER ** initBag();
LETTER ** initHand(LETTER** bag);
void shuffleCharacterInBag(LETTER** bag);
LETTER* pullLetterFromBag(LETTER ** bag);
int returnCharacterToBag(LETTER ** bag, LETTER* returnLetter);
void swapCharacterFromHandToBag(LETTER** bag, LETTER** hand);


#endif
