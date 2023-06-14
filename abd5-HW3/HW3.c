/*
 ============================================================================
 Name        : HW3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

int totalMapSize;
int foodCount;
int viewPortSize;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "Game.h"
#include "viewPort.h"
#include "UpdatingView.h"
//gets the map size
void getTotalMapSize(char line[]){
	int i=0;
	int size=sizeof(line);
	while(i<=size*2){
		if(line[i]>=48 && line[i]<=57){
			totalMapSize=totalMapSize*10;
			totalMapSize=totalMapSize+(line[i]-48);
		}
		i++;
	}
}
//prints the instructions
void printInstructions(){
	printf("The goal is to get from the bottom right side of the map you are (X) to the top left (Z) while trying to not loose all of the food you have\n");
	fflush(stdout);
	printf("You move by going Up(U), Down(D), Left(L), and Right(R)\n");
	fflush(stdout);
	printf("You can leave any time by typing quit(Q)\n");
	fflush(stdout);
	printf("River crossing (R) cost you 20 food, hunting: Elk(E) will cost you no food,Boar(B) will cost you 5 food, Grizzly(G) 10 food,and Hare(H) will give you 5 food\n");
	fflush(stdout);
	printf("Contracting the Cold (C) will cost you 5 food, the Flu(F) will cost you 10 food, and Dysentary(D) will cost you 15 food\n");
	fflush(stdout);
	printf("Good luck\n");
	fflush(stdout);
}
//gets the viewPort size
void getTotalView(char line[]){
	int i=0;
	int size=sizeof(line);
	while(i<=size*2){
		if(line[i]>=48 && line[i]<=57){
			viewPortSize=viewPortSize*10;
			viewPortSize=viewPortSize+(line[i]-48);
		}
		i++;
	}
}
//Gets the food count
void getFoodCount(char line[]){
	int i=0;
	int size=sizeof(line);
	while(i<=size*2){
		if(line[i]>=48 && line[i]<=57){
			foodCount=foodCount*10;
			foodCount=foodCount+(line[i]-48);
		}
		i++;
	}
}
//Gets all data from the file creates the map and asks the user to play the game when the map is ready
int main(int argc, char *argv[]){
	FILE *file;
	int counter=0;
	char line[256]={0};
	char reset[256]={0};
	int row=0;
	char data[10];
	file = fopen(argv[1], "r");
	//initialize arrays for map and view port
	NODE * map=NULL, * viewPort=NULL, *temp;
	if(file){
		//gets the size of the map view port and food count
		while (fgets(line, sizeof(line), file)!=NULL) {
			if(counter==0){
				getTotalMapSize(line);
			}else if(counter==1){
				getTotalView(line);
			}else if(counter==2){
				getFoodCount(line);
			}else if(counter>5){
				if(line[1]!='O'){
					int i;
					row++;
					int count=1;
					for(i=0;i<totalMapSize;i++){
						temp = CreateNode(line[i+1],row,count);
						Insert (&map, temp);
						count++;
					}
				}
			}
			counter++;
			strncpy(line,reset,256);
		}
		bool ready=false;
		bool over=false;
		while(!ready){
			printf("Game is ready to go would you like to start (S), see the instructions (I), or quit (Q)?\n");
			fflush(stdout);
			scanf("%s", data);
			if(strcmp(data,"S")==0){
				play(&map,&viewPort,foodCount,viewPortSize,totalMapSize);
				viewPort=NULL;
				free(viewPort);
				ready=true;
			}else if(strcmp(data,"I")==0){
				printInstructions();
			}else if(strcmp(data,"Q")==0){
				printf("BYE!!!!\n");
				fflush(stdout);
				//free nodes
				ready=true;
			}else{
				printf("Wrong input!!!!\n");
				fflush(stdout);
			}
		}
		//frees the map
		NODE *prev,*curr;
		prev=(map);
		curr=prev->down;
		while(curr->down!=NULL){
			prev=curr;
			curr=curr->down;
		}
		while(curr->right!=NULL){
			prev=curr;
			curr=curr->right;
		}
		while(!over){
			while(prev->left!=NULL){
				deleteNode(&map,curr,1);
				curr=prev;
				prev=prev->left;
			}
			if(prev->up->up!=NULL){
				deleteNode(&map,curr,1);
				curr=prev;
				prev=prev->up;
				while(prev->right!=NULL){
					prev=prev->right;
				}
				deleteNode(&map,curr,1);
				curr=prev;
				prev=prev->left;
			}else{
				deleteNode(&map,curr,1);
				curr=prev;
				prev=prev->up;
				deleteNode(&map,curr,1);
				while(prev->right!=NULL){
					prev=prev->right;
				}
				curr=prev;
				while(prev->left!=NULL){
					prev=prev->left;
					deleteNode(&map,curr,0);
					curr=prev;
				}
				deleteNode(&map,curr,0);
				over=true;
			}
		}
		map=NULL;
		free(map);
	}
	return 0;
}
