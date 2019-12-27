#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "abstractHashTable.h"
#include "CheckBoard.h"
#include "save.h"


void printBoard(BOX *** board)
{
	printf("------------------Scrabble Board----------------\n\n");
	int i = 0;
	int j = 0;

	printf("    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14\n");
	printf("------------------------------------------------\n");
	while(i<15)
	{
		if (i < 10)
		    printf("0%d |", i);
		else
			printf("%d |",i);
		while(j<15)
		{
			if((board[i][j])->currentLetter)
				printf("%c |",(board[i][j])->currentLetter->letter);
			else
				printf("  |");
			++j;
		}
		printf("\n");
		j = 0;
		++i;
	}
	printf("-----------------------------------------------\n");
}

int main()
{
	int input = -1;
	char input2[32];
	int nbplayers;
	int check = 1;
	char word[15];
	int retVal;
	int x;
	int y;
	int dir;
	BOX*** board;
	int i;
	int j;
	int turn1 = 1;
	LETTER** bag; 
	LETTER** playersHands[4];
	int playersScores[4];
	int chose;
	char secword[15];
	int playerTurn = 0;

	while (check)
	{
		system("clear");
		printf("                   ********************************************************************\n");
		printf("                   *               Welcome to the 'Scrabble Game' !!                  *\n");
		printf("                   * This game was created by 'La Squad' composed by                  *\n"); 
		printf("                   * Sabri Durand, Enzo Merrotto, Rattapon Jindalak and Léo Tavernier *\n");
		printf("                   * Please be sure that you are not playing on 'Windows'             *\n");
		printf("                   * Also, be sure that this window is in full screen or              *\n"); 
		printf("                   * in the maximum size you can use, thank you !                     *\n");
		printf("                   ********************************************************************\n\n");
		printf("0 : Menu\n");
		printf("1 : Quit\n\n");
		printf("Answer : ");
		scanf("%s", input2);
		input = input2[0] - '0';
		switch(input)
		{
			case 1:
				printf("\n\n");
				printf("                   **********************************\n");
				printf("                   * Thanks for playing our game !! *\n");
				printf("                   **********************************\n");
				exit(1);
			case 0:
				while (check)
				{
					system("clear");
					printf("                   **************************\n");	
					printf("                   * Welcome to the Menu !! *\n");
					printf("                   **************************\n\n");
					printf("0 : New game\n");
					printf("1 : Load game\n");
					printf("2 : Quit\n\n");
					printf("Answer : ");
					scanf("%s", input2);
					input = input2[0] - '0';
					switch(input)
					{
						case 0:
							while(check)
							{
								board = boardInit();
								printf("\nHow many players will play today (2 to 4)?\n\n");
								printf("Answer : ");
								scanf("%d", &input);
								if (input >= 2 && input <= 4)
								{
									nbplayers = input;
									check = 0;
								}
								else
									printf("\nSorry wrong input, you have to choose a number between 2 and 4...");
								bag = initBag();
								for (j = 0; j < nbplayers; j++) 
								{
									playersHands[j] = initHand(bag);
									playersScores[j] = 0;
								}
							}
							break;
						case 1:
							board = malloc(15 * sizeof(BOX **));
							bag = malloc(100 * sizeof(LETTER*));
							for (i  = 0; i  < 4; i ++) 
							{
								playersHands[i] = malloc(7 * sizeof(LETTER*));
							}
							read(board, &nbplayers, bag, playersHands, playersScores, &playerTurn);
							check = 0;
							break;
						case 2:
							printf("\n\n");
							printf("                   **********************************\n");
							printf("                   * Thanks for playing our game !! *\n");
							printf("                   **********************************\n");
							exit(1);
						default:
							break;
					}

				}
				break;
			default:
				printf("Wrong answer\n");
				break;
		}
	}

	buildDictionary();
	j = playerTurn;
	while (1)
	{
		while (j < nbplayers)
		{
			check = 0;
			while (!check)
			{
				system("clear");
				printBoard(board);
				printf("       Player %d\n", j + 1);
				printf("       Score : %d\n", playersScores[j]);
				printf("       Hand : ");
				for (i = 0; i < 7; i++)
					printf("%c", playersHands[j][i]->letter);
				printf("\n\n");
				shuffleCharacterInBag(bag);
				printf("What do you want to do ?\n\n");
				printf("0 : Place a word\n");
				printf("1 : Change letters in the hand\n");
				printf("2 : Save and quit\n\n");
				printf("Answer : ");
				scanf("%s", input2);
				chose = input2[0] - '0';
				switch(chose)
				{
					case 0:
						system("clear");
						printBoard(board);
						printf("       Player %d\n", j + 1);
						printf("       Score : %d\n", playersScores[j]);
						printf("       Hand : ");
						for (i = 0; i < 7; i++)
							printf("%c", playersHands[j][i]->letter);
						printf("\n\n");
						printf("Write a word : ");
						scanf("%s", word);
						printf("x : ");
						scanf("%d", &x);
						printf("y : ");
						scanf("%d", &y);
						printf("Direction(0 for vertical and 1 for horizontal) : ");
						scanf("%d", &dir);
						retVal = CheckBoard(y, x, dir, word, board, playersHands[j], turn1, secword);
						if (turn1 == 1 && retVal == 6)
							retVal = 0;
						switch(retVal)
						{
							case 0:
								RefeelFromBag(bag, playersHands[j]);
								playersScores[j] += countScore(word);
								if (secword != NULL)
									playersScores[j] += countScore(secword);
								turn1 = 0;
								check = 1;
								break;
							case 1:
								printf("The word is too big\n");
								break;
							case 2:
								printf("The word does not exist\n");
								break;
							case 3:
								printf("A letter does not correspond with letters on the board\n");
								break;
							case 4:
								printf("You do not have all the letters to build the word\n");
								break;
							case 5:
								printf("The other word created does not exist\n");
								break;
							case 6:
								printf("You need to use at least 1 letter in the board\n");
								break;
							default:
								printf("Error not identified\n");
								break;
						}	
						if (retVal != 0)
							scanf("%s", input2);
						break;
					case 1:
						swapCharacterFromHandToBag(bag, playersHands[j]);
						check = 1;
						break;
					case 2:
						write(board, nbplayers, bag, playersHands, playersScores, playerTurn);
						printf("\n\n");
						printf("                   **********************************\n");
						printf("                   * Thanks for playing our game !! *\n");
						printf("                   **********************************\n");
						exit(1);
					default:
						break;
				}
			}
			j = (j + 1) % nbplayers;
		}
	}
}

