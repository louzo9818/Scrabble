/* 
 * CheckBoard.c
 *
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abstractHashTable.h"
#include "struct.h"
#include "CheckBoard.h"



int HaveLetter(char Letter, LETTER** playerhand, LETTER** letterplayed, int index)
{
	int j = 0;
	int i = 0;
	
	while (i < 7)
	{
		if (playerhand[i] != NULL && Letter == playerhand[i]->letter)
		{
			letterplayed[index] = playerhand[i];
			playerhand[i] = NULL;
			return 1;
		}
		i++;
	}
	return 0;
		
	
}

int LetterArround(int x, int y, int dir,BOX*** board)
{
	if (dir == 0)
	{
		if(y>0 && board[x][y-1]->currentLetter != NULL)
			return 1;
		if(y<14 && board[x][y+1]->currentLetter != NULL)
			return 1;
	}
	else
	{
		if(x<14 && board[x+1][y]->currentLetter != NULL)
			return 1;
		if(x>0 && board[x-1][y]->currentLetter != NULL)
			return 1;
	}
}

void StartSecondWord(int* x2, int* y2,int dir, BOX*** board)
{
	if (dir == 0)
	{
		while ( *x2-1 >= 0 && board[*x2-1][*y2]->currentLetter != NULL)
			*x2=*x2-1;				
	}
	else
	{
		while ( *y2-1 >= 0 && board[*x2][*y2-1]->currentLetter != NULL)
			*y2= *y2-1;
				
	}


}


int WordExist(BOX*** board,int dir, int x, int y, int x1, int y1, char currentletter, char* secword)
{
	char Word[15];
	int i = 0;
	int j;

	if (dir == 0)
	{
		if(x==x1 && y ==y1)
		{
			Word[i] = currentletter;
			i++;
			x++;
		}
		while ( x+1 <= 15 && board[x][y]->currentLetter != NULL)			
		{
			Word[i] = board[x][y]->currentLetter->letter;
			i++;
			x++;
			if(x==x1 && y ==y1)
			{
				Word[i] = currentletter;
				i++;
				x++;
			}
		}
							
	}
	else
	{
		if(x==x1 && y ==y1)
		{
			Word[i] = currentletter;
			i++;
			x++;
		}
		while ( y+1 <= 15 && board[x][y]->currentLetter != NULL)
		{
			Word[i] = board[x][y]->currentLetter->letter;
			i++;
			y++;
			if(x==x1 && y ==y1)
			{
				Word[i] = currentletter;
				i++;
				y++;
			}
		}
			
				
	}
	Word[i] = '\0';
	if (hashTableLookup(Word)!=NULL)
	{
		for (j = 0; j < i; j++)
			secword[j] = Word[j];
		secword[i] = '\0';
		return 1;
	}
	else
		return 0;

}

void RestoreLetters(LETTER** LettersPlayed, LETTER** playerhand)
{
	int i = 0;
	int j = 0;
	while (LettersPlayed[i] && i<7)
	{
		if(LettersPlayed[i]!= NULL)
		{
			while (j<7)
			{
				if(playerhand[j] == NULL)
				{
					playerhand[j] = LettersPlayed[i];
					LettersPlayed[i] = NULL;
					break;
				}
				j++;
			
			}
		}
		i++;

	}

}

int CheckBoard(int x, int y, int dir, char* word, BOX*** board, LETTER** playerhand, int turn1, char* secword)
{
	int x2;
	int y2;
	int xs = x;
	int ys = y;
	int dir2;
	LETTER* PlayerLetters[7];
	memset(PlayerLetters, 0, 7);
	int i = 0;
	int j = 0;
	int otherL = 0;
	char currentLetter;
	int index = 0;

	if (strlen(word) > 15)
	    return 1; /*word too big*/
	if (hashTableLookup(word) == NULL)
	    return 2; /*word does not exist on the dictionnary*/
	while ( i < strlen(word))
	{
		currentLetter = word[i];
		if (board[x][y]->currentLetter != NULL)
		{
			if (board[x][y]->currentLetter->letter != currentLetter)
			{
				RestoreLetters(PlayerLetters, playerhand);
				return 3; /*a letter is not the same than in the board*/
			}
			otherL++;
		}
		else
		{
			if (!HaveLetter(currentLetter, playerhand, PlayerLetters, index))
			{	
				RestoreLetters(PlayerLetters, playerhand);
				return 4; /*it miss a letter in your hand*/
			}
			else
			{
				if(LetterArround(x, y,dir,board))
				{
					x2 = x;
					y2 =y;
					dir2 = (dir+1)%2;
					StartSecondWord(&x2,&y2,dir2,board);
					if (!WordExist(board, dir2, x2, y2, x, y, currentLetter, secword))
					{
						RestoreLetters(PlayerLetters, playerhand);
						return 5; /*word does not exist*/
					}
					otherL=1;
			
				}
			}
			index++;	
		}
		if(dir)
			y++;
		else
			x++;

		i++;	
	}
	if(otherL == 0 && turn1 == 0)
	{
		RestoreLetters(PlayerLetters, playerhand);
		return 6; /*you need to use at least 1 letter in the board*/
	}
	PlaceWord(xs, ys, dir, word, board, PlayerLetters, index);
	return 0;
		
	


}

void PlaceWord(int x, int y, int dir,char* word, BOX*** board, LETTER** PlayerLetters, int index)
{
	int i = 0;
	int j = 0;
	while (i < strlen(word))
	{
		if(j < index && word[i] == PlayerLetters[j]->letter && board[x][y]->currentLetter == NULL)
			{
			board[x][y]->currentLetter = PlayerLetters[j];
			PlayerLetters[j] = NULL;
			j++;
			}
		if(dir == 0)
			x++;
		else
			y++;
		i++;	
	}
}

void RefeelFromBag(LETTER** bag, LETTER** playerLetters)
{
	int i = 0;
	while(i < 7)
	{
		if(playerLetters[i]==NULL)
			playerLetters[i] = pullLetterFromBag(bag);
		i++;
	}
}

int countScore(char* word)
{
	int i = 0;
	int score = 0;
	char letterScore[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

	for (i = 0; i < strlen(word); i++) 
		score += letterScore[word[i] - 'a'];
	return score;
}
















