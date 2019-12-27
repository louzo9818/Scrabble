#ifndef SAVE_H
#define SAVE_H

void write(BOX *** board, int nbplayers, LETTER** bag, LETTER** playersHands[4], int playersScores[4], int playerTurn);
void read(BOX *** board, int* nbplayers, LETTER** bag, LETTER** playersHands[4], int playersScores[4], int* playerTurn);

#endif
