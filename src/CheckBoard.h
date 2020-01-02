#ifndef CHECKBOARD_H
#define CHECKBOARD_H

int HaveLetter(char Letter, LETTER** playerhand, LETTER** letterplayed, int index);
int LetterArround(int x, int y, int dir,BOX*** board);
void StartSecondWord(int* x2, int* y2,int dir, BOX*** board);
int WordExist(BOX*** board,int dir, int x, int y,int x1, int y1, char currentletter, char* secword);
void RestoreLetters(LETTER** LettersPlayed, LETTER** playerhand);
int CheckBoard(int x, int y, int dir,char* word, BOX*** board, LETTER** playerhand, int turn1, char* secword);
void PlaceWord(int x, int y, int dir,char* word, BOX*** board, LETTER** PlayerLetters, int index);
void RefeelFromBag(LETTER** bag,LETTER** playerLetters);
int countScore(char* word);


#endif /* ifndef CHECKBOARD_H*/
