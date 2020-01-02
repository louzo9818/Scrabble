#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "abstractHashTable.h"
#include "CheckBoard.h"
#include "save.h"

void write(BOX *** board, int nbplayers, LETTER** bag, LETTER** playersHands[4], int playersScores[4], int playerTurn)
{
	FILE* file = fopen("save.txt", "w");
	int i;
	int j;

	for (i = 0; i < 15; i++) 
	{
		for (j = 0; j < 15; j++) 
		{
		    if (board[i][j]->currentLetter != NULL)
		   		fputc(board[i][j]->currentLetter->letter, file);
			else
				fputc('0', file);
		}   
		fputc('\n', file);
	}

	fputc(nbplayers + '0', file);
	fputc('\n', file);

	for (i = 0; i < 100; i++)
	{
		if (bag[i] != NULL)
			fputc(bag[i]->letter, file);
		else
			fputc('0', file);
	}
	fputc('\n', file);

	for (i = 0; i < nbplayers; i++)
	{
		for (j = 0; j < 7; j++)
			fputc(playersHands[i][j]->letter, file);
		fputc('\n', file);
	}

	for (i = 0; i < nbplayers; i++)
	{
		fputc(playersScores[i] + '0', file);
		fputc('\n', file);
	}

	fputc(playerTurn + '0', file);
	
	fclose(file);
}

void read(BOX *** board, int* nbplayers, LETTER** bag, LETTER** playersHands[4], int playersScores[4], int* playerTurn)
{
	FILE* file = fopen("save.txt", "r");
	int i;
	int j;
	char line[100];

	for (i = 0; i < 15; i++) 
	{
		fgets(line, 100, file);
		board[i] = malloc(sizeof(BOX*) * 15);
		for (j = 0; j < 15; j++) 
		{
			board[i][j] = malloc(sizeof(BOX));
		 	if (line[j] == '0')
				board[i][j]->currentLetter = NULL;
			else
			{
				board[i][j]->currentLetter = malloc(sizeof(LETTER));
				board[i][j]->currentLetter->letter = line[j];
			}
		}
		printf("\n");  
	}
	
	fgets(line, 100, file);
	*nbplayers = line[0] - '0';

	fgets(line, 100, file);
	for (i = 0; i < 100; i++)
	{
		bag[i] = malloc(sizeof(LETTER));
		if (line[i] == 0)
			bag[i] = NULL;
		else
		{
			bag[i]->letter = line[i];
		}
	}

	fgets(line, 100, file);
	for (i = 0; i < *nbplayers; i++)
	{
		fgets(line, 100, file);
		//playersHands[i] = malloc(7 * sizeof(LETTER*));
		for (j = 0; j < 7; j++)
		{
			playersHands[i][j] = malloc(sizeof(LETTER));
			playersHands[i][j]->letter = line[j];
		}
		printf("\n");
	}

	for (i = 0; i < *nbplayers; i++)
	{
		fgets(line, 100, file);
		playersScores[i] = atoi(line);
	}
	
	fgets(line, 100, file);
	*playerTurn = atoi(line);
}
