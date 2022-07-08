/* Programmed by: Eryn Gabriel C. Tallador S12
Description: This program is a version of minesweeper wherein you could create your own level sets
and save them into text file so you could play with it later or load text file that contains
the games that you have created. After levels are loaded or created in the game, you could play it by choosing
the play game option.
Last modified: September 29, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "interface.h"

void gameMenu();

//structure for data input for a level
struct levelData{
	int rowNumber;
	int columnNumber;
	char tileSet[15][15];
	char mineCoordinates[15][15];
};

//structure for linked list nodes
struct levelNode{
	struct levelData level;
	struct levelNode *pNext;
};

typedef struct levelData level;
typedef struct levelNode node;

/* This function delays the action of the next line of code
@param (int) number_of_seconds is the amount of seconds the next line of code would be delayed
@return (void)
*/
void delay(int number_of_seconds){ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

/* This function initializes an empty linked list
@param pHead points to the value contained within pHead
@return (void)
*/
void initialize(node **pHead){
	*pHead = NULL;
}

/* This function adds a new node at the end of the linked list
@param pHead points to the value contained within pHead
@param levelSet is the data to be added to the new node
@return (int) value of 0 if allocation is unsuccessful
*/
int addNode(node **pHead, level levelSet){
	node *pRun;
	node *pTemp;
	// Case A: The linked list is empty
	if (*pHead == NULL)
	{
		*pHead = malloc(sizeof(node));
		if (*pHead == NULL)
		{
			return 0;
		}
		(*pHead)->level = levelSet;
		(*pHead)->pNext = NULL;
	}
	// Case B: The linked list is not empty
	else
	{
		pRun = *pHead;
		while (pRun->pNext != NULL)
		{
			pRun = pRun->pNext;
		}
		pTemp = malloc(sizeof(node));
		if (pTemp == NULL)
		{
			return 0;
		}
		pTemp->level = levelSet;
		pTemp->pNext = NULL;
		pRun->pNext = pTemp;
	}
}

/* This function de-allocates all the spaces allocated to the linked list.
@param pHead points to the value contained within pHead
@return (void) 
*/
void deallocate(node *pHead){
	node *pRun;
	node *pTrail;
	
	pTrail = pHead;
	while (pTrail != NULL)
	{
		pRun = pTrail->pNext;
		free(pTrail);
		pTrail = pRun;
	}
}

/* This function checks the number of mines surrounding the tile
@param (int) rowInput is the value of inputted row within the playGame function
@param (int) columnInput is the value of inputted column within the playGame function
@param pRun is the current node/levelSet the player is playing
@return (int) mines is number of mines within the surrounding tile
*/
int findnearbymines(int rowInput, int columnInput, node *pRun){
    int mines = 0;
	
	//upper right
	if(rowInput == 0 && columnInput == pRun->level.columnNumber - 1){
		if(pRun->level.mineCoordinates[rowInput][columnInput - 1] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput - 1] == 'X')
			mines++;
	}
	
	//lower right corner
	else if(rowInput == pRun->level.rowNumber - 1 && columnInput == pRun->level.columnNumber - 1){
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput][columnInput - 1] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput - 1] == 'X')
			mines++;
	}
	
	//lower left corner
	else if(rowInput == pRun->level.rowNumber - 1 && columnInput == 0){
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput][columnInput + 1] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput + 1] == 'X')
			mines++;
	}
	
	//upper left corner
	else if(rowInput == 0 && columnInput == 0){
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput][columnInput + 1] == 'X')
			mines++;
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput + 1] == 'X')
			mines++;
	}
	
	
	else{
		//check 4 main directions
		//up
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput] == 'X')
			mines++;
		//down
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput] == 'X')
			mines++;
		//left
		if(pRun->level.mineCoordinates[rowInput][columnInput - 1] == 'X')
			mines++;
		//right
		if(pRun->level.mineCoordinates[rowInput][columnInput + 1] == 'X')
			mines++;
							
		// Check all diagonal directions
		//up-right
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput + 1] == 'X')
			mines++;
		//up-left
		if(pRun->level.mineCoordinates[rowInput - 1][columnInput - 1] == 'X')
			mines++;
		//down-right
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput + 1] == 'X')
			mines++;
		//down-left
		if(pRun->level.mineCoordinates[rowInput + 1][columnInput - 1] == 'X')
			mines++;
		}
    

    return mines;
}

/* This function checks the number of mines surrounding the tiles
until the tile it inspects equates to greater than 0.
@param (int) rowInput is the value of inputted row within the playGame function
@param (int) columnInput is the value of inputted column within the playGame function
@param pRun is the current node/levelSet the player is playing
@return (void) 
*/
void recursive(int rowInput, int columnInput, node *pRun){
	int nearbymines = 0;
    int k = 0;
    int l = 0;
	
	//checks current inspected tile
    nearbymines = findnearbymines(rowInput, columnInput, pRun);
    pRun->level.tileSet[rowInput][columnInput] = (char)(((int)'0') + nearbymines);
	
	
    nearbymines = 0;
    // Checking nearby mines going up
    while(nearbymines < 1 && k < rowInput + 1)
    {
        nearbymines = findnearbymines(rowInput - k, columnInput, pRun);
        pRun->level.tileSet[rowInput - k][columnInput] = (char)(((int)'0') + nearbymines);
        k++;
    }
    
    k = 0;
    nearbymines = 0;
    // Checking nearby mines going down
    while(nearbymines < 1 && rowInput + k < pRun->level.rowNumber)
    {
        nearbymines = findnearbymines(rowInput + k, columnInput, pRun);
        pRun->level.tileSet[rowInput + k][columnInput] = (char)(((int)'0') + nearbymines);
        k++;
    }
    
    k = 0;
    nearbymines = 0;
    // Checking nearby mines going left
    while(nearbymines < 1 && k < columnInput + 1)
    {
        nearbymines = findnearbymines(rowInput, columnInput - k, pRun);
        pRun->level.tileSet[rowInput][columnInput - k] = (char)(((int)'0') + nearbymines);
        k++;
    }
    
    k = 0;
    nearbymines = 0;
    // Checking nearby mines going right
    while(nearbymines < 1 && columnInput + k < pRun->level.columnNumber)
    {
        nearbymines = findnearbymines(rowInput, columnInput + k, pRun);
        pRun->level.tileSet[rowInput][columnInput + k] = (char)(((int)'0') + nearbymines);
        k++;
    }
    
    
    k = 0;
    l = 0;
    nearbymines = 0;
    // Checking nearby mines going up-right
    while(nearbymines < 1 && columnInput + l < pRun->level.columnNumber && k < rowInput + 1)
    {
    	if(k + 1 == rowInput + 1){
	        nearbymines = findnearbymines(rowInput - k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        l++;
		}
		else if(columnInput + l + 1 == pRun->level.columnNumber){
	        nearbymines = findnearbymines(rowInput - k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        k++;
		}
		else{
	        nearbymines = findnearbymines(rowInput - k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        k++;
	        l++;
		}
    }
    
    k = 0;
    l = 0;
    nearbymines = 0;
    // Checking nearby mines going up-left
    while(nearbymines < 1 && k < rowInput + 1 && l < columnInput + 1)
    {
        if(k + 1 == rowInput + 1){
	        nearbymines = findnearbymines(rowInput - k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        l++;
		}
		else if(l + 1 == columnInput + 1){
	        nearbymines = findnearbymines(rowInput - k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        k++;
		}
		else{
	        nearbymines = findnearbymines(rowInput - k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput - k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        k++;
	        l++;
		}
    }
    
    k = 0;
    l = 0;
    nearbymines = 0;
    // Checking nearby mines going down-right
    while(nearbymines < 1 && rowInput + k < pRun->level.rowNumber && columnInput + l < pRun->level.columnNumber)
    {
        if(rowInput + k + 1 == pRun->level.rowNumber && columnInput){
	        nearbymines = findnearbymines(rowInput + k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        l++;
		}
		else if(columnInput + l + 1 == pRun->level.columnNumber){
	        nearbymines = findnearbymines(rowInput + k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        k++;
		}
		else{
	        nearbymines = findnearbymines(rowInput + k, columnInput + l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput + l] = (char)(((int)'0') + nearbymines);
	        k++;
	        l++;
		}
    }
    
    k = 0;
    l = 0;
    nearbymines = 0;
    // Checking nearby mines going down-left
    while(nearbymines < 1 && rowInput + k < pRun->level.rowNumber && l < columnInput + 1)
    {
        if(rowInput + k + 1 == pRun->level.rowNumber && columnInput){
	        nearbymines = findnearbymines(rowInput + k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        l++;
		}
		else if(l + 1 == columnInput + 1){
	        nearbymines = findnearbymines(rowInput + k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        k++;
		}
		else{
	        nearbymines = findnearbymines(rowInput + k, columnInput - l, pRun);
	        pRun->level.tileSet[rowInput + k][columnInput - l] = (char)(((int)'0') + nearbymines);
	        k++;
	        l++;
		}
    }
}

/* This function plays the game that is currently loaded in the linked list
within the program.
@param (int) levels is the number of levels the currently loaded levelSet has
@param pHead points to the value contained within the first node
@return (void) 
*/
void playGame (node **pHead, int levels){
	
	//looping values
	int i;
	int j;
	//location inputs
	int rowInput = 0;
	int columnInput = 0;
	//choice input
	int moveChoice = 0;
	//win checkers
	int winCheck;
	int mines;
	int dashes;
	
	//checks if there is no levelSet loaded
	if (levels == 0){
		iSetColor(I_COLOR_YELLOW);
		printf("\nThere is no file loaded!");
		iSetColor(I_COLOR_WHITE);
		delay(2);
		delay(0);
		gameMenu();
	}
	
	iHideCursor();
	iSetColor(I_COLOR_GREEN);                                                                                                                                                                                                  
	printf("\nLoading Tileset!\n");
	delay(1);
	
	node *pRun;
	pRun = *pHead;
	//for loading the next level until there is none left
	while (pRun != NULL){
		iHideCursor();
		mines = 0;
		system("cls");
		
		//aesthetics
		iSetColor(I_COLOR_GREEN);
		printf("¦¦       ¦¦¦¦¦¦   ¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦      ¦¦¦¦¦¦¦ ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦           ¦¦      ¦¦      ¦¦    ¦¦ ¦¦      ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦     ¦¦      ¦¦¦¦¦   ¦¦    ¦¦ ¦¦¦¦¦   ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦     ¦¦      ¦¦       ¦¦  ¦¦  ¦¦      ¦¦              \n"); 
		printf("¦¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦ ¦¦¦¦¦¦  ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦   ¦¦¦¦   ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦\n");
		
		delay(1); 
		
		system("cls");
		iSetColor(I_COLOR_GREEN);
		printf("¦¦       ¦¦¦¦¦¦   ¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦      ¦¦¦¦¦¦¦ ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦           ¦¦      ¦¦      ¦¦    ¦¦ ¦¦      ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦     ¦¦      ¦¦¦¦¦   ¦¦    ¦¦ ¦¦¦¦¦   ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦     ¦¦      ¦¦       ¦¦  ¦¦  ¦¦      ¦¦              \n"); 
		printf("¦¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦ ¦¦¦¦¦¦  ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦   ¦¦¦¦   ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦ ¦¦\n");
		
		delay(1); 
		
		system("cls");
		iSetColor(I_COLOR_GREEN);
		printf("¦¦       ¦¦¦¦¦¦   ¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦      ¦¦¦¦¦¦¦ ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦           ¦¦      ¦¦      ¦¦    ¦¦ ¦¦      ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦     ¦¦      ¦¦¦¦¦   ¦¦    ¦¦ ¦¦¦¦¦   ¦¦              \n"); 
		printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦     ¦¦      ¦¦       ¦¦  ¦¦  ¦¦      ¦¦              \n"); 
		printf("¦¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦ ¦¦¦¦¦¦  ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦   ¦¦¦¦   ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦ ¦¦ ¦¦\n");
                                                                                                             
        delay(1);                                                                                                     

		system("cls");
		
		//converts tileSet data to all dashes
		for(i=0; i < pRun->level.rowNumber; i++){
			for(j=0; j < pRun->level.columnNumber; j++){
				pRun->level.tileSet[i][j]= '-';
			}
		}
		
		//counts the number of mines
		for(i=0; i < pRun->level.rowNumber; i++){
			for(j=0; j < pRun->level.columnNumber; j++){
				if (pRun->level.mineCoordinates[i][j] == 'X'){
					mines++;
				}
			}
		}
		
		//loop to check if player has won the current level
		while(winCheck == 0){
			
			dashes = 0;
			
			//counts the number of dashes and flags every loop
			for(i=0; i < pRun->level.rowNumber; i++){
				for(j=0; j < pRun->level.columnNumber; j++){
					if (pRun->level.tileSet[i][j] == '-' || pRun->level.tileSet[i][j] == 'F'){
						dashes++;
					}
				}
			}
			
			//win checker if the number dashes and flags is equal to the number of mines
			if (dashes == mines){
				winCheck++;
			}
			
			//statement where player hasn't won yet
			else{
				moveChoice = 0;
				
				system("cls");
				
				//displays number of columns
				printf("  ");
				for(j=0; j < pRun->level.columnNumber; j++){
					iSetColor(I_COLOR_CYAN);
					if(j < 9){
						printf(" %d ", j + 1);
					}
					else{
						printf(" %d", j + 1);
					}
				}
				printf("\n");
				//displays tileset
				for(i=0; i < pRun->level.rowNumber; i++){
					iSetColor(I_COLOR_CYAN);
					//prints the number of rows
					printf("%d", i+1);
					if( i < 9){
						printf(" ");
					}
					for(j=0; j < pRun->level.columnNumber; j++){
						//prints the value of specific tileSet location
						if(pRun->level.tileSet[i][j] == 'F'){
							iSetColor(I_COLOR_PURPLE);
							printf(" %c ",pRun->level.tileSet[i][j]);
						}
						else{
							iSetColor(I_COLOR_YELLOW);
							printf(" %c ",pRun->level.tileSet[i][j]);	
						}
					}
					
					printf("\n");
				}
				iShowCursor();
				iSetColor(I_COLOR_WHITE);
				printf("\n\n[1]Inspect Tile\n");
				printf("[2]Flag Tile\n");
				printf("[3]Remove Flag\n");
				printf("\n\n");
				
				//choice input
				//prevents the player from inputting out of bounds value
				do{
				printf("What do you want to do?:");
				scanf("%d",&moveChoice);
				printf("\n");
				}while (moveChoice > 3 || moveChoice < 1);
				
				switch(moveChoice){
					case 1:
						printf("What tile would you like to inspect?\n");
						
						//row input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input row:");
						scanf("%d",&rowInput);
						}while(rowInput > pRun->level.rowNumber || rowInput < 1);
						
						//column input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input Column:");
						scanf("%d",&columnInput);
						}while(columnInput > pRun->level.columnNumber || columnInput < 1);
						
						//statement if tile is flagged
						if (pRun->level.tileSet[rowInput-1][columnInput-1] == 'F'){
							iSetColor(I_COLOR_YELLOW);
							printf("\nThis tile is flagged!");
							delay(1);
							printf("\nPlease select another option\n");
							delay(2);
							continue;
						}
						
						//statement if tile is already inspected
						if (pRun->level.tileSet[rowInput-1][columnInput-1] != '-' && pRun->level.tileSet[rowInput-1][columnInput-1] != 'F'){
							iSetColor(I_COLOR_YELLOW);
							printf("\nThis tile is already inspected!");
							delay(1);
							printf("\nPlease select another option\n");
							delay(2);
							continue;
						}
						
						//statement if player inspects a tile with mines
						if (pRun->level.tileSet[rowInput-1][columnInput-1] != pRun->level.mineCoordinates[rowInput-1][columnInput-1]){
							system("cls");
							printf("  ");
							//displays column values
							for(j=0; j < pRun->level.columnNumber; j++){
								iSetColor(I_COLOR_CYAN);
								if(j < 9){
									printf(" %d ", j + 1);
								}
								else{
									printf(" %d", j + 1);
								}
							}
							printf("\n");
							//displays the inspected tiles and bomb locations
							for(i=0; i < pRun->level.rowNumber; i++){
								//displays row values
								iSetColor(I_COLOR_CYAN);
								printf("%d", i+1);
								if( i < 9){
									printf(" ");
								}
								for(j=0; j < pRun->level.columnNumber; j++){
									//adds the bombs to the tileSet display
									if (pRun->level.mineCoordinates[i][j] == 'X'){
										pRun->level.tileSet[i][j] = pRun->level.mineCoordinates[i][j];
									}
									//display current tileSet location values
									if(pRun->level.tileSet[i][j] == 'X'){
										iSetColor(I_COLOR_RED);
									}
									else{
										iSetColor(I_COLOR_YELLOW);
									}
									printf(" %c ",pRun->level.tileSet[i][j]);
								}
								printf("\n");
							}
							iSetColor(I_COLOR_RED);
							printf("\n\nYou lost the game!");
							iSetColor(I_COLOR_WHITE);
							deallocate(*pHead);
							exit(1);
						}
						
						//statement if player safely inpects a tile
						else{
							int k;
							int l;
							rowInput = rowInput - 1; 
							columnInput = columnInput - 1;
							
							//3 step recursive surrounding mine checker
							recursive(rowInput,columnInput,pRun);
							
							//up
							k = 0;
							while(k < rowInput + 1 && pRun->level.tileSet[rowInput - k][columnInput] == '0'){
								//if(pRun->level.tileSet[rowInput - k][columnInput] == '0'){
									recursive(rowInput - k,columnInput,pRun);
								//}
								k++;
							}
								
							//down
							k = 0;
							while(rowInput + k < pRun->level.rowNumber && pRun->level.tileSet[rowInput + k][columnInput] == '0'){
								//if(pRun->level.tileSet[rowInput + k][columnInput] == '0'){
									recursive(rowInput + k,columnInput,pRun);
								//}
								k++;
							}
							//left
							k = 0;
							while(k < columnInput + 1 && pRun->level.tileSet[rowInput][columnInput - k] == '0'){
								//if(pRun->level.tileSet[rowInput][columnInput - k] == '0'){
									recursive(rowInput,columnInput - k,pRun);
								//}
								k++;
							}
							//right
							k = 0;
							while(columnInput + k < pRun->level.columnNumber && pRun->level.tileSet[rowInput][columnInput + k] == '0'){
								//if(pRun->level.tileSet[rowInput][columnInput + k] == '0'){
									recursive(rowInput,columnInput + k,pRun);
								//}
								k++;
							}
							
							//up-left 
							k = 0;
							l = 0;
							while(pRun->level.tileSet[rowInput - k][columnInput - l] == '0' && k < rowInput + 1 && l < columnInput + 1){
								if(k + 1 == rowInput + 1){
									recursive(rowInput - k,columnInput - l,pRun);
									l++;
								}
								else if(l + 1 == columnInput + 1){
									recursive(rowInput - k,columnInput - l,pRun);
									k++;
								}
								else{
									recursive(rowInput - k,columnInput - l,pRun);
									l++;
									k++;
								}
							}
							
							//up-right
							k = 0;
							l = 0;
							while(pRun->level.tileSet[rowInput - k][columnInput + l] == '0' && k < rowInput + 1 && columnInput + l < pRun->level.columnNumber){
								if(k + 1 == rowInput + 1){
									recursive(rowInput - k,columnInput + l,pRun);
									l++;
								}
								else if(columnInput + l + 1 == pRun->level.columnNumber){
									recursive(rowInput - k,columnInput + l,pRun);
									k++;
								}
								else{
									recursive(rowInput - k,columnInput + l,pRun);
									l++;
									k++;
								}
							}
							
							//down-left
							k = 0;
							l = 0;
							while(pRun->level.tileSet[rowInput + k][columnInput - l] == '0' && rowInput + k < pRun->level.rowNumber && l < columnInput + 1){
								if(rowInput + k + 1 == pRun->level.rowNumber){
									recursive(rowInput + k,columnInput - l,pRun);
									l++;
								}
								else if(l + 1 == columnInput + 1){
									recursive(rowInput + k,columnInput - l,pRun);
									k++;
								}
								else{
									recursive(rowInput + k,columnInput - l,pRun);
									l++;
									k++;
								}
							}
							
							//down-right
							k = 0;
							l = 0;
							while(pRun->level.tileSet[rowInput + k][columnInput + l] == '0' && rowInput + k < pRun->level.rowNumber && columnInput + l < pRun->level.columnNumber){
								if(rowInput + k + 1 == pRun->level.rowNumber){
									recursive(rowInput + k,columnInput + l,pRun);
									l++;
								}
								else if(columnInput + l + 1 == pRun->level.columnNumber){
									recursive(rowInput + k,columnInput + l,pRun);
									k++;
								}
								else{
									recursive(rowInput + k,columnInput + l,pRun);
									l++;
									k++;
								}
							}
							
						}
						break;
					
					case 2:
						printf("What tile would you like to flag?\n");
						
						//row input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input row:");
						scanf("%d",&rowInput);
						}while(rowInput > pRun->level.rowNumber || rowInput < 1);
						
						//column input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input Column:");
						scanf("%d",&columnInput);
						}while(columnInput > pRun->level.columnNumber || columnInput < 1);
						
						//statement if tile is not flagged
						if (pRun->level.tileSet[rowInput-1][columnInput-1] == '-'){
							pRun->level.tileSet[rowInput-1][columnInput-1] = 'F';
						}
						
						//statement if tile is already inspected or flagged
						else{
							
							iSetColor(I_COLOR_YELLOW);
							printf("\nCan't flag this tile!");
							delay(1);
							printf("\nPlease select another option\n");
							delay(2);
							continue;
						}						
						break;
						
					case 3:
						
						printf("What tile would you like to remove flag?\n");
						
						//row input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input row:");
						scanf("%d",&rowInput);
						}while(rowInput > pRun->level.rowNumber || rowInput < 1);
						
						//column input value
						//prevents the player from inputting out of bounds value
						do{
						printf("Input Column:");
						scanf("%d",&columnInput);
						}while(columnInput > pRun->level.columnNumber || columnInput < 1);
						
						//statement if tile is flagged
						if (pRun->level.tileSet[rowInput-1][columnInput-1] == 'F'){
							pRun->level.tileSet[rowInput-1][columnInput-1] = '-';			
						}
						
						//statement if tile not flagged or already inspected
						else{
							iSetColor(I_COLOR_YELLOW);
							printf("\nCan't deflag this tile!");
							delay(1);
							printf("\nPlease select another option\n");
							delay(2);
							continue;
						}
						
						break;	
				}
			}
		}
		
		//statement if player has cleared a level and there is no next level
		if(pRun->pNext == NULL){
			system("cls");
			
			printf("  ");
			for(j=0; j < pRun->level.columnNumber; j++){
				iSetColor(I_COLOR_CYAN);
				if(j < 9){
					printf(" %d ", j + 1);
				}
				else{
					printf(" %d", j + 1);
				}
			}
			printf("\n");
			for(i=0; i < pRun->level.rowNumber; i++){
				iSetColor(I_COLOR_CYAN);
				printf("%d", i+1);
				if( i < 9){
					printf(" ");
				}
				for(j=0; j < pRun->level.columnNumber; j++){
					if (pRun->level.mineCoordinates[i][j] == 'X'){
						pRun->level.tileSet[i][j] = pRun->level.mineCoordinates[i][j];
					}
					if(pRun->level.tileSet[i][j] == 'X'){
						iSetColor(I_COLOR_RED);
					}
					else{
						iSetColor(I_COLOR_YELLOW);
					}
					printf(" %c ",pRun->level.tileSet[i][j]);
				}
				printf("\n");
			}
			iSetColor(I_COLOR_GREEN);
			printf("\n\nYou win the game!");
			iSetColor(I_COLOR_WHITE);
			deallocate(*pHead);
			exit(1);
		}
		
		//statement if player has cleared a level and there is a next level
		else{
			system("cls");
			
			
			printf("  ");
			for(j=0; j < pRun->level.columnNumber; j++){
				iSetColor(I_COLOR_CYAN);
				if(j < 9){
					printf(" %d ", j + 1);
				}
				else{
					printf(" %d", j + 1);
				}
			}
			printf("\n");
			for(i=0; i < pRun->level.rowNumber; i++){
				iSetColor(I_COLOR_CYAN);
				printf("%d", i+1);
				if( i < 9){
					printf(" ");
				}
				for(j=0; j < pRun->level.columnNumber; j++){
					if (pRun->level.mineCoordinates[i][j] == 'X'){
						pRun->level.tileSet[i][j] = pRun->level.mineCoordinates[i][j];
					}
					if(pRun->level.tileSet[i][j] == 'X'){
						iSetColor(I_COLOR_RED);
					}
					else{
						iSetColor(I_COLOR_YELLOW);
					}
					printf(" %c ",pRun->level.tileSet[i][j]);
				}
				printf("\n");
			}
			
			iSetColor(I_COLOR_GREEN);
			printf("\n\nProceeding to next level!");
			delay(2);
			printf("\nPress any key to continue...");
			getch();
			iSetColor(I_COLOR_WHITE);
			winCheck--;
			pRun = pRun->pNext;
		}	
	}	
}

/* This function allows the player to create his/her own levelSets with unlimited number of levels
@param pHead points to the value contained within the first node
@return (int) lvl is the number levels created 
*/
int createLevelSet(node **pHead){
	
	system("cls");
	
	//current level value input
	level temp;
	//used for looping
	int i;
	int j;
	//sets the boards dimensions
	int rows;
	int columns;
	//tileset array
	char tiles[15][15];
	//garbage value for flushing
	char garbage;
	//choice input
	int nChoice;
	//mine location inputs
	int mineRow;
	int mineColumn;
	//number of mines in a level checker
	int mineChecker;
	//number of levels created
	int set=0;
	
	initialize(pHead);
	
	//carbon copy of tileset array
	char mineCoordinates[15][15];
	
	//loop for checking if player has decided to quit level creation
	while(nChoice != 4){
		
		mineChecker = 0;
		
		system("cls");
		nChoice = 0;
		
		iSetColor(I_COLOR_CYAN);
		printf(" ¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦¦¦¦¦¦  ¦¦¦¦¦  ¦¦¦¦¦¦¦¦ ¦¦  ¦¦¦¦¦¦  ¦¦¦   ¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦    ¦¦ ¦¦¦¦  ¦¦\n"); 
		printf("¦¦      ¦¦¦¦¦¦  ¦¦¦¦¦   ¦¦¦¦¦¦¦    ¦¦    ¦¦ ¦¦    ¦¦ ¦¦ ¦¦ ¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦    ¦¦ ¦¦  ¦¦¦¦\n"); 
		printf(" ¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦    ¦¦    ¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦¦\n\n");
        iSetColor(I_COLOR_WHITE);                                                                
                                                                        

		//row dimension value
		//prevents the player from inputting out of bounds value
		do{
			printf("Set the number of rows (Min 5 | Max 15):");
			scanf("%d", &rows);
		}while(rows > 15 || rows < 5 );
		
		printf("\n");
		
		//column dimension value
		//prevents the player from inputting out of bounds value
		do{
			printf("\nSet the number of columns (Min 5 | Max 15):");
			scanf("%d", &columns);
		}while(columns > 15 || columns < 5 );
		scanf("%c", &garbage);
		
		//tileset creation
		for(i=0; i < rows; i++){
			for(j=0; j < columns; j++){
				tiles[i][j]= '-';
			}
			printf("\n");
		}
		
		
		system("cls");
		iHideCursor();
		iSetColor(I_COLOR_GREEN);
		//aesthetics
		printf(" ¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦¦¦¦¦¦  ¦¦¦¦¦  ¦¦¦¦¦¦¦¦ ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦¦ ¦¦ ¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦¦¦  ¦¦ ¦¦              ¦¦    ¦¦ ¦¦      ¦¦      ¦¦     \n"); 
		printf("¦¦      ¦¦¦¦¦¦  ¦¦¦¦¦   ¦¦¦¦¦¦¦    ¦¦    ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦        ¦¦    ¦¦ ¦¦      ¦¦¦¦¦   ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦        ¦¦    ¦¦ ¦¦      ¦¦           ¦¦\n"); 
		printf(" ¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦    ¦¦    ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦         ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦\n");
		
		delay(1); 
		
        system("cls");
		
		iSetColor(I_COLOR_GREEN);
		printf(" ¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦¦¦¦¦¦  ¦¦¦¦¦  ¦¦¦¦¦¦¦¦ ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦¦ ¦¦ ¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦¦¦  ¦¦ ¦¦              ¦¦    ¦¦ ¦¦      ¦¦      ¦¦     \n"); 
		printf("¦¦      ¦¦¦¦¦¦  ¦¦¦¦¦   ¦¦¦¦¦¦¦    ¦¦    ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦        ¦¦    ¦¦ ¦¦      ¦¦¦¦¦   ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦        ¦¦    ¦¦ ¦¦      ¦¦           ¦¦\n"); 
		printf(" ¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦    ¦¦    ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦         ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦ ¦¦\n"); 
		
		delay(1); 
		
		system("cls");
		
		iSetColor(I_COLOR_GREEN);
		printf(" ¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦¦¦¦¦¦  ¦¦¦¦¦  ¦¦¦¦¦¦¦¦ ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦      ¦¦¦¦¦¦¦¦ ¦¦ ¦¦      ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦¦¦  ¦¦ ¦¦              ¦¦    ¦¦ ¦¦      ¦¦      ¦¦     \n"); 
		printf("¦¦      ¦¦¦¦¦¦  ¦¦¦¦¦   ¦¦¦¦¦¦¦    ¦¦    ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦        ¦¦    ¦¦ ¦¦      ¦¦¦¦¦   ¦¦¦¦¦¦¦\n"); 
		printf("¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦    ¦¦    ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦        ¦¦    ¦¦ ¦¦      ¦¦           ¦¦\n"); 
		printf(" ¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦    ¦¦    ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦         ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦ ¦¦ ¦¦\n");                                                                                                                
        
		delay(1);                                                                                                                 

		//loop for checking if player has decided for next level or quit level creation
		while (nChoice != 3 && nChoice != 4){
			system("cls");
			printf("  ");
			for(j=0; j < columns; j++){
				iSetColor(I_COLOR_CYAN);
				if(j < 9){
					printf(" %d ", j + 1);
				}
				else{
					printf(" %d", j + 1);
				}
				iSetColor(I_COLOR_WHITE);
			}
			printf("\n");
			for(i=0; i < rows; i++){
				iSetColor(I_COLOR_CYAN);
				printf("%d", i+1);
				if( i < 9){
					printf(" ");
				}
				for(j=0; j < columns; j++){
					if(tiles[i][j] == 'X'){
						iSetColor(I_COLOR_RED);
						printf(" %c ",tiles[i][j]);
					}
					else{
						iSetColor(I_COLOR_YELLOW);
						printf(" %c ",tiles[i][j]);
					}
				}
				printf("\n");
				iSetColor(I_COLOR_WHITE);
			}
			iShowCursor();	
			printf("\n\n[1]Add Mine\n");
			printf("[2]Delete Mine\n");
			printf("[3]Save Level and Proceed to Next Level\n");
			printf("[4]Save Level and Go Back to Main Menu\n");
			printf("\n\n");
			
			//choice input
			//prevents the player from inputting out of bounds value
			do{
			printf("What do you want to do?:");
			scanf("%d",&nChoice);
			printf("\n");
			}while (nChoice > 4 || nChoice < 1);
				
			switch(nChoice){
				case 1:
					
					printf("Where would you like to add the mine?\n");
					
					//row input
					//prevents the player from inputting out of bounds value
					do{
					printf("Input row:");
					scanf("%d",&mineRow);
					}while(mineRow > rows || mineRow < 1);
					
					//row input
					//prevents the player from inputting out of bounds value
					do{
					printf("Input Column:");
					scanf("%d",&mineColumn);
					}while(mineColumn > columns || mineColumn < 1);
					
					//statement if tile already has a bomb
					if(tiles[mineRow-1][mineColumn-1] == 'X'){
						iSetColor(I_COLOR_YELLOW);
						printf("\nThat tile already has a bomb!\n");
						delay(1);
						printf("Please select another option\n");
						delay(2);
						continue;
					}
					
					//statement if tile doesn't have a bomb
					else{
						tiles[mineRow-1][mineColumn-1] = 'X';
						
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
								mineCoordinates[i][j] = tiles[i][j];
							}
						}
						mineChecker++;
					}
					break;
					
				case 2:
					
					printf("Where would you like to delete the mine?\n");
					
					//row input
					//prevents the player from inputting out of bounds value
					do{
					printf("Input row:");
					scanf("%d",&mineRow);
					}while(mineRow > rows || mineRow < 1);
					
					//column input
					//prevents the player from inputting out of bounds value
					do{
					printf("Input Column:");
					scanf("%d",&mineColumn);
					}while(mineColumn > columns || mineColumn < 1);
					
					//statement if tile doesn't have a bomb
					if(tiles[mineRow-1][mineColumn-1] == '-'){
						iSetColor(I_COLOR_YELLOW);
						printf("\nThat tile doesn't have a bomb!\n");
						delay(1);
						printf("Please select another option\n");
						delay(2);
						continue;
					}
					
					//statement if tile has a bomb
					else{
						tiles[mineRow-1][mineColumn-1]= '-';
					
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
								mineCoordinates[i][j] = tiles[i][j];
							}
						}
						mineChecker--;
					}
					break;
					
				case 3:
					//statement to check if there are no bombs in the current level
					if (mineChecker == 0){
						iSetColor(I_COLOR_YELLOW);
						printf("You haven't placed any bombs yet!\n");
						delay(1);
						printf("Please place at least one bomb.\n");
						delay(2);
						nChoice = 0;
						continue;
					}
					
					//statement if created level is valid and proceed to next level creation
					else{
						iSetColor(I_COLOR_GREEN);
						printf("Saving Level...\n");
						iSetColor(I_COLOR_WHITE);
						delay(3);
						temp.rowNumber = rows;
						temp.columnNumber = columns;
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
									temp.tileSet[i][j] = tiles[i][j];
							}
						}
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
									temp.mineCoordinates[i][j] = tiles[i][j];
							}
						}
						addNode(pHead, temp);
					}
					break;
					
				case 4:
					//statement to check if there are no bombs in the current level
					if (mineChecker == 0){
						iSetColor(I_COLOR_YELLOW);
						printf("You haven't placed any bombs yet!\n");
						delay(1);
						printf("Please place at least one bomb.\n");
						delay(2);
						nChoice = 0;
						continue;
					}
					
					//statement if created level is valid and goes back to main menu
					else{
						iSetColor(I_COLOR_GREEN);
						printf("Saving Level...\n");
						iSetColor(I_COLOR_WHITE);
						delay(3);
						temp.rowNumber = rows;
						temp.columnNumber = columns;
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
									temp.tileSet[i][j] = tiles[i][j];
							}
						}
						for(i=0; i < rows; i++){
							for(j=0; j < columns; j++){
									temp.mineCoordinates[i][j] = tiles[i][j];
							}
						}
						addNode(pHead, temp);
					}
					break;
						
						
				default:
					printf("Invalid input!");
						
			}
		}
		set++;
	}
		return set;
}

/* This function loads the player's desired levelSet by typing the txt file name
@param pHead points to the value contained within the first node
@return (int) lvl is the number levels loaded 
*/
int loadGame(node **pHead){
	
	system("cls");
	
	//level data placeholder before adding to node
	level temp;
	//loop variables
	int i;
	int j;
	//loaded levels checker
	int ctr;
	//holds the value of number of levels the text file has
	int lvl;
	//text file name array
	char loadFile[100];
	//garbage value
	char someDummyVariable;
	
	initialize(pHead);
	
	iHideCursor();
	iSetColor(I_COLOR_CYAN);
	printf("¦¦       ¦¦¦¦¦¦   ¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦ \n");
	printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦      \n");
	printf("¦¦      ¦¦    ¦¦ ¦¦¦¦¦¦¦ ¦¦   ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦\n");
	printf("¦¦      ¦¦    ¦¦ ¦¦   ¦¦ ¦¦   ¦¦ ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦\n");
	printf("¦¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦ ¦¦¦¦¦¦  ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦ \n");
	iSetColor(I_COLOR_WHITE);
                                                                
    delay(1);
	iShowCursor();
	printf("\nEnter .txt file name to load\n");
	scanf("%95s",loadFile);
	strcat(loadFile,".txt");
	
	FILE *gameLoad;
	gameLoad = fopen(loadFile,"r");
	
	//unsuccessful open
	if (gameLoad == NULL){
		delay(3);
		iSetColor(I_COLOR_RED);
		printf("\nFile not Loaded!\n");
		delay(2);
		delay(0);
		iSetColor(I_COLOR_WHITE);
		gameMenu();
	}
	
	//successful open
	else{
		fscanf(gameLoad, "%d", &lvl);
		
		//loop to check if the next scanned value is the end of file
		while (feof(gameLoad) == 0){
			fscanf(gameLoad, "%d", &temp.rowNumber);
			fscanf(gameLoad, "%d", &temp.columnNumber);
			fscanf(gameLoad, "%c", &someDummyVariable);
			
			for(i=0; i < temp.rowNumber; i++){
				for(j=0; j < temp.columnNumber; j++){
					fscanf(gameLoad, "%c", &temp.tileSet[i][j]);
				}
				fscanf(gameLoad, "%c", &someDummyVariable);
			}
			
			//converts underscores to dashes
			for(i=0; i < temp.rowNumber; i++){
				for(j=0; j < temp.columnNumber; j++){
					if(temp.tileSet[i][j] == '_'){
						temp.tileSet[i][j] = '-';
					}
				}
			}
			
			for(i=0; i < temp.rowNumber; i++){
				for(j=0; j < temp.columnNumber; j++){
					temp.mineCoordinates[i][j] = temp.tileSet[i][j];
				}
			}
			
			addNode(pHead, temp);
			ctr++;
			
			if (ctr == lvl){
				fscanf(gameLoad, "%c", &someDummyVariable);
			}
			
		
		}
		iHideCursor();
		delay(2);
		iSetColor(I_COLOR_GREEN);
		printf("\nSuccessfully Loaded!\n");
		delay(2);
		iSetColor(I_COLOR_WHITE);
		fclose(gameLoad);
		
		return lvl;
	}
}

/* This function saves the player's created level set by typing their desired txt file name
@param pHead points to the value contained within the first node
@param (int) levels is the number of levels within the created levelSet
@return (void)
*/
void saveGame(node **pHead, int levels){
	
	system("cls");
	
	//looping variables
	int i;
	int j;
	//text file name array
	char fileName[100];
	
	iHideCursor();
	iSetColor(I_COLOR_CYAN);
	printf("¦¦¦¦¦¦¦  ¦¦¦¦¦  ¦¦    ¦¦ ¦¦ ¦¦¦   ¦¦  ¦¦¦¦¦¦ \n");
	printf("¦¦      ¦¦   ¦¦ ¦¦    ¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦      \n");
	printf("¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦    ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦   ¦¦¦\n"); 
	printf("     ¦¦ ¦¦   ¦¦  ¦¦  ¦¦  ¦¦ ¦¦  ¦¦¦¦ ¦¦    ¦¦\n"); 
	printf("¦¦¦¦¦¦¦ ¦¦   ¦¦   ¦¦¦¦   ¦¦ ¦¦   ¦¦¦  ¦¦¦¦¦¦ \n"); 
    iSetColor(I_COLOR_WHITE);                                                    
    delay(1);                                                    

	iShowCursor();
	printf("\nEnter .txt file name to save\n");
	scanf("%95s",fileName);
	strcat(fileName,".txt");
	
	FILE *gameCreateLevel;
	gameCreateLevel = fopen(fileName,"w");
	
	//unsuccessful creation
	if (levels == 0){
		delay(3);
		iSetColor(I_COLOR_RED);
		printf("\nFile not saved!\n");
		delay(2);
		delay(0);
		iSetColor(I_COLOR_WHITE);
		gameMenu();
	}
	
	fprintf(gameCreateLevel,"%d\n", levels);

	node *pRun;
	pRun = *pHead;
	//loop until the end of levelSet
	while (pRun != NULL)
	{
		fprintf(gameCreateLevel,"%d\n", pRun->level.rowNumber);
		fprintf(gameCreateLevel,"%d\n", pRun->level.columnNumber);
		
		//converts dashes to underscores
		for(i=0; i < pRun->level.rowNumber; i++){
			for(j=0; j < pRun->level.columnNumber; j++){
				if(pRun->level.tileSet[i][j] == '-'){
					pRun->level.tileSet[i][j] = '_';
				}
			}
		}
		
		for(i=0; i < pRun->level.rowNumber; i++){
			for(j=0; j < pRun->level.columnNumber; j++){
				fprintf(gameCreateLevel, "%c", pRun->level.tileSet[i][j]);
			}
			fprintf(gameCreateLevel,"\n");
		}
		pRun = pRun->pNext;
	}
	
	iHideCursor();
	delay(2);
	iSetColor(I_COLOR_GREEN);
	printf("\nFile Saved!");
	iSetColor(I_COLOR_WHITE);
	delay(2);
	
	fclose(gameCreateLevel);
	
	
}

/* This function contains the gameMenu where players could choose what they want to do.
Either playing the currently loaded levelSet, load a levelSet, create a levelSet, save a
levelSet, or simply quit the game.
@return (void)
*/
void gameMenu(){
	
	//number levels of current loaded levelSet
	int levels;
	//choice input
	int choice = 0;
	node *pHead;
	//loop while choice input is not equal to 5
	while (choice != 5){
		choice = 0;
		system("cls");
		iSetColor(I_COLOR_RED);
		
		iHideCursor();
		printf("¦¦¦    ¦¦¦ ¦¦ ¦¦¦   ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦     ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦¦¦¦¦¦ ¦¦¦¦¦¦            \n");
		printf("¦¦¦¦  ¦¦¦¦ ¦¦ ¦¦¦¦  ¦¦ ¦¦      ¦¦      ¦¦     ¦¦ ¦¦      ¦¦      ¦¦   ¦¦ ¦¦      ¦¦   ¦¦     _ ¦¦ _\n");
		printf("¦¦ ¦¦¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦ ¦¦¦¦¦   ¦¦¦¦¦¦¦ ¦¦  ¦  ¦¦ ¦¦¦¦¦   ¦¦¦¦¦   ¦¦¦¦¦¦  ¦¦¦¦¦   ¦¦¦¦¦¦       ¦¦¦¦ \n");
		printf("¦¦  ¦¦  ¦¦ ¦¦ ¦¦  ¦¦¦¦ ¦¦           ¦¦ ¦¦ ¦¦¦ ¦¦ ¦¦      ¦¦      ¦¦      ¦¦      ¦¦   ¦¦     ¯ ¦¦ ¯\n");
		printf("¦¦      ¦¦ ¦¦ ¦¦   ¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦  ¦¦¦ ¦¦¦  ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦      ¦¦¦¦¦¦¦ ¦¦   ¦¦           \n");
		delay(1);
        iShowCursor();                                                                                            
                                                                                                    
 
        
		iSetColor(I_COLOR_YELLOW);                                                                                  
		printf("\nCurrent Number of Levels Loaded: %d", levels);
		
		iSetColor(I_COLOR_WHITE);
		printf("\n\n");
		printf("[1]Play Game!\n");
		printf("[2]Create Level Set!\n");
		printf("[3]Load a Level Set!\n");
		printf("[4]Save a Level Set!\n");
		printf("[5]Quit Game! :(\n\n");
		
		//choice input
		//prevents from inputting out of bounds values
		do{	
			printf("What do you want to do?:");
			scanf("%d",&choice);
		}while(choice < 1 || choice > 5);
		
		switch(choice){
			//play current loaded level set
			case 1:
				playGame(&pHead, levels);
				break;
			//create a level set
			case 2:
				levels = createLevelSet(&pHead);
				break;
			//load a text file containing a level set
			case 3:
				levels = loadGame(&pHead);
				break;
			//save current level set loaded
			case 4:
				saveGame(&pHead, levels);
				break;
			//deallocate and quit the game
			case 5:
				if (&pHead == NULL){
					deallocate(pHead);
				}
				break;
		}
	}
}

int main(){
	
	gameMenu();
	
	return 0;
}
