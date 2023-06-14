/*
 * Game.c
 *
 *  Created on: Oct 28, 2014
 *      Author: dan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Game.h"
#include "LinkedList.h"
#include "viewPort.h"
#include "UpdatingView.h"
//makes that node based on row and column seen on the viewPort and Map
void makeDiscoverable(NODE ** headPtr, int row,int column,NODE ** viewPort){
	NODE * prev, *curr;
	prev=(*headPtr);
	curr=prev->down;
	while(prev->xPos!=row){
		prev=curr;
		curr=curr->down;
	}
	curr=prev;
	curr=curr->right;
	while(prev->yPos!=column){
		prev=curr;
		curr=curr->right;
	}
	prev->discovered=true;
	prev=(*viewPort);
	curr=prev->down;
	while(prev->xPos!=row){
		prev=curr;
		curr=curr->down;
	}
	curr=prev;
	curr=curr->right;
	while(prev->yPos!=column){
		prev=curr;
		curr=curr->right;
	}
	prev->discovered=true;

}
//check what type of node it is and drops the food count by x amount
int notAnimal(NODE * pos,int foodCount){
	if(pos->type.typeofNone->animalOrDisease[0]=='F'){
		printf("You've contracted the Flu you loose 10 food\n");
		fflush(stdout);
		foodCount=foodCount-10;
	}else if(pos->type.typeofNone->animalOrDisease[0]=='D'){
		printf("You've contracted Dysentery you loose 15 food\n");
		fflush(stdout);
		foodCount=foodCount-15;
	}else if(pos->type.typeofNone->animalOrDisease[0]=='R'){
		printf("You've crossed a River you loose 20 food\n");
		fflush(stdout);
		foodCount=foodCount-20;
	}else if(pos->type.typeofNone->animalOrDisease[0]=='C'){
		printf("You've contracted a cold you loose 5 food\n");
		fflush(stdout);
		foodCount=foodCount-5;
	}
	return foodCount;

}
//determines what the animal is and prints a statement asking if they want to hunt the animal
void whatAnimal(NODE * pos){
	if(pos->type.typeofNone->animalOrDisease[0]=='E'){
		printf("You've encountered an Elk ");
		fflush(stdout);
	}else if(pos->type.typeofNone->animalOrDisease[0]=='B'){
		printf("You've encountered an Boar ");
		fflush(stdout);
	}else if(pos->type.typeofNone->animalOrDisease[0]=='G'){
		printf("You've encountered an Grizzly ");
		fflush(stdout);
	}else if(pos->type.typeofNone->animalOrDisease[0]=='H'){
		printf("You've encountered an Hare ");
		fflush(stdout);
	}
}
//determines what animal the user hunted and drops the food count by x amount
int animal(NODE * pos,int foodCount){
	if(pos->type.typeofNone->animalOrDisease[0]=='B'){
		foodCount=foodCount-5;
	}else if(pos->type.typeofNone->animalOrDisease[0]=='G'){
		foodCount=foodCount-10;
	}else if(pos->type.typeofNone->animalOrDisease[0]=='H'){
		foodCount=foodCount+5;
	}
	return foodCount;

}
//moves the x node up down left or right
int moving(NODE ** viewPort,int whichWay,int foodCount, NODE ** headPtr, int viewPortSize,int mapSize){
	NODE * x=getXNode(viewPort);
	NODE * prev;
	char data[10];
	if(whichWay==0){
		if(x->up==NULL){
			printf("Can't move up\n");
			fflush(stdout);
		}else{
			if(x->up->type.typeofNone->animalOrDisease[0]=='F' ||x->up->type.typeofNone->animalOrDisease[0]=='D'  ||
					x->up->type.typeofNone->animalOrDisease[0]=='C') {
				if(x->up->aliveOrDead==1){
					foodCount=notAnimal(x->up,foodCount);
					makeDiscoverable(headPtr, x->up->xPos, x->up->yPos,viewPort);
					movingUP(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,0,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,0,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}else{
					movingUP(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,0,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,0,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else if( x->up->type.typeofNone->animalOrDisease[0]=='R' ){
				if(x->up->aliveOrDead==1){
					printf("You are attempting to ford a river; you may either choose to ford (F) or retreat(R)\n");
					fflush(stdout);
					scanf("%s", data);
					if(strcmp(data,"F")==0){
						foodCount=notAnimal(x->up,foodCount);
						makeDiscoverable(headPtr, x->up->xPos, x->up->yPos,viewPort);
						movingUP(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,0,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,0,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingUP(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,0,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,0,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else{
				if(x->up->type.typeofNone->animalOrDisease[0]!='Z'){
					if(x->up->aliveOrDead==1){
						whatAnimal(x->up);
						printf("Would you like to hunt (H) or runaway (R)\n");
						fflush(stdout);
						scanf("%s", data);
						if(strcmp(data,"H")==0){
							foodCount=animal(x->up,foodCount);
							makeDiscoverable(headPtr, x->up->xPos, x->up->yPos,viewPort);
							movingUP(viewPort,headPtr,viewPortSize);
							prev=findPrev(viewPort,0,x);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
							x=getXNode(headPtr);
							prev=findPrev(headPtr,0,x);
							swap(x,prev);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
						}else{
							makeDiscoverable(headPtr, x->up->xPos, x->up->yPos,viewPort);
						}
					}else{
						movingUP(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,0,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,0,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingUP(viewPort,headPtr, viewPortSize);
					prev=findPrev(viewPort,0,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,0,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}
			}
		}
	}else if(whichWay==1){
		if(x->down==NULL){
			printf("Can't move down\n");
			fflush(stdout);
		}else{
			if(x->down->type.typeofNone->animalOrDisease[0]=='F'  ||x->down->type.typeofNone->animalOrDisease[0]=='D'  ||
					x->down->type.typeofNone->animalOrDisease[0]=='C' ){
				if(x->down->aliveOrDead==1){
					foodCount=notAnimal(x->down,foodCount);
					makeDiscoverable(headPtr, x->down->xPos, x->down->yPos,viewPort);
					movingDown(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,1,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,1,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}else{
					movingDown(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,1,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,1,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else if( x->down->type.typeofNone->animalOrDisease[0]=='R'){
				if(x->down->aliveOrDead==1){
					printf("You are attempting to ford a river; you may either choose to ford (F) or retreat(R)\n");
					fflush(stdout);
					scanf("%s", data);
					if(strcmp(data,"F")==0){
						foodCount=notAnimal(x->down,foodCount);
						makeDiscoverable(headPtr, x->down->xPos, x->down->yPos,viewPort);
						movingDown(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,1,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,1,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingDown(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,1,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,1,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else{
				if(x->down->aliveOrDead==1){
					whatAnimal(x->down);
					printf("Would you like to hunt (H) or runaway (R)\n");
					fflush(stdout);
					scanf("%s", data);
					if(strcmp(data,"H")==0){
						foodCount=animal(x->down,foodCount);
						makeDiscoverable(headPtr, x->down->xPos, x->down->yPos,viewPort);
						movingDown(viewPort,headPtr, viewPortSize);
						prev=findPrev(viewPort,1,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,1,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}else{
						makeDiscoverable(headPtr, x->down->xPos, x->down->yPos,viewPort);
					}
				}else{
					movingDown(viewPort,headPtr, viewPortSize);
					prev=findPrev(viewPort,1,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,1,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}
			}
		}
	}else if(whichWay==2){
		if(x->left==NULL){
			printf("Can't move left\n");
			fflush(stdout);
		}else{
			if(x->left->type.typeofNone->animalOrDisease[0]=='F' ||x->left->type.typeofNone->animalOrDisease[0]=='D'  ||
					x->left->type.typeofNone->animalOrDisease[0]=='C') {
				if(x->left->aliveOrDead==1){
					foodCount=notAnimal(x->left,foodCount);
					makeDiscoverable(headPtr, x->left->xPos, x->left->yPos,viewPort);
					movingLeft(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,2,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,2,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}else{
					movingLeft(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,2,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,2,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else if( x->left->type.typeofNone->animalOrDisease[0]=='R' ){
				if(x->left->aliveOrDead==1){
					printf("You are attempting to ford a river; you may either choose to ford (F) or retreat(R)\n");
					fflush(stdout);
					scanf("%s", data);
					if(strcmp(data,"F")==0){
						foodCount=notAnimal(x->left,foodCount);
						makeDiscoverable(headPtr, x->left->xPos, x->left->yPos,viewPort);
						movingLeft(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,2,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,2,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingLeft(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,2,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,2,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else{
				if(x->left->type.typeofNone->animalOrDisease[0]!='Z'){
					if(x->left->aliveOrDead==1){
						whatAnimal(x->left);
						printf("Would you like to hunt (H) or runaway (R)\n");
						fflush(stdout);
						scanf("%s", data);
						if(strcmp(data,"H")==0){
							foodCount=animal(x->left,foodCount);
							makeDiscoverable(headPtr, x->left->xPos, x->left->yPos,viewPort);
							movingLeft(viewPort,headPtr,viewPortSize);
							prev=findPrev(viewPort,2,x);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
							x=getXNode(headPtr);
							prev=findPrev(headPtr,2,x);
							swap(x,prev);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
						}else{
							makeDiscoverable(headPtr, x->left->xPos, x->left->yPos,viewPort);
						}
					}else{
						movingLeft(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,2,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,2,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingLeft(viewPort,headPtr, viewPortSize);
					prev=findPrev(viewPort,2,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,2,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}
			}
		}
	}else if(whichWay==3){
		if(x->right==NULL){
			printf("Can't move right\n");
			fflush(stdout);
		}else{
			if(x->right->type.typeofNone->animalOrDisease[0]=='F' ||x->right->type.typeofNone->animalOrDisease[0]=='D'  ||
					x->right->type.typeofNone->animalOrDisease[0]=='C') {
				if(x->right->aliveOrDead==1){
					foodCount=notAnimal(x->right,foodCount);
					makeDiscoverable(headPtr, x->right->xPos, x->right->yPos,viewPort);
					movingRight(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,3,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,3,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}else{
					movingRight(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,3,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,3,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else if( x->right->type.typeofNone->animalOrDisease[0]=='R' ){
				if(x->right->aliveOrDead==1){
					printf("You are attempting to ford a river; you may either choose to ford (F) or retreat(R)\n");
					fflush(stdout);
					scanf("%s", data);
					if(strcmp(data,"F")==0){
						foodCount=notAnimal(x->right,foodCount);
						makeDiscoverable(headPtr, x->right->xPos, x->right->yPos,viewPort);
						movingRight(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,3,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,3,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') && (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingRight(viewPort,headPtr,viewPortSize);
					prev=findPrev(viewPort,3,x);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
					x=getXNode(headPtr);
					prev=findPrev(headPtr,3,x);
					swap(x,prev);
					if(prev->xPos==mapSize && prev->xPos==mapSize){
						prev->type.typeofNone->animalOrDisease[0]='X';
						prev->type.typeofNone->prev[0]='X';
					}
					prev->aliveOrDead=1;
				}
			}else{
				if(x->right->type.typeofNone->animalOrDisease[0]!='Z'){
					if(x->right->aliveOrDead==1){
						whatAnimal(x->right);
						printf("Would you like to hunt (H) or runaway (R)\n");
						fflush(stdout);
						scanf("%s", data);
						if(strcmp(data,"H")==0){
							foodCount=animal(x->right,foodCount);
							makeDiscoverable(headPtr, x->right->xPos, x->right->yPos,viewPort);
							movingRight(viewPort,headPtr,viewPortSize);
							prev=findPrev(viewPort,3,x);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
							x=getXNode(headPtr);
							prev=findPrev(headPtr,3,x);
							swap(x,prev);
							if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
									||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
											||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
								x->aliveOrDead=1;
								prev->aliveOrDead=1;
							}else{
								x->aliveOrDead=0;
								prev->aliveOrDead=1;
							}
						}else{
							makeDiscoverable(headPtr, x->right->xPos, x->right->yPos,viewPort);
						}
					}else{
						movingRight(viewPort,headPtr,viewPortSize);
						prev=findPrev(viewPort,3,x);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
						x=getXNode(headPtr);
						prev=findPrev(headPtr,3,x);
						swap(x,prev);
						if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
								||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
										||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
							x->aliveOrDead=1;
							prev->aliveOrDead=1;
						}else{
							x->aliveOrDead=0;
							prev->aliveOrDead=1;
						}
					}
				}else{
					movingRight(viewPort,headPtr, viewPortSize);
					prev=findPrev(viewPort,3,x);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
					x=getXNode(headPtr);
					prev=findPrev(headPtr,3,x);
					swap(x,prev);
					if((prev->type.typeofNone->prev[0]=='R'||prev->type.typeofNone->prev[0]=='C'
							||prev->type.typeofNone->prev[0]=='D'||prev->type.typeofNone->prev[0]=='F') || (x->type.typeofNone->prev[0]=='R'||x->type.typeofNone->prev[0]=='C'
									||x->type.typeofNone->prev[0]=='D'||x->type.typeofNone->prev[0]=='F')){
						x->aliveOrDead=1;
						prev->aliveOrDead=1;
					}else{
						x->aliveOrDead=0;
						prev->aliveOrDead=1;
					}
				}
			}
		}
	}
	return foodCount;
}
//plays the game
void play(NODE ** headPtr,NODE ** viewPort,int foodCount,int viewPortSize,int mapSize){
	bool game=0;
	char data[10];
	bool over=false;
	getViewPort(headPtr, viewPort,viewPortSize,mapSize);
	if(viewPort!=NULL){
		while(game==0 && foodCount>=0){
			NODE *x=getXNode(viewPort);
			if(foodCount==0){
				printf("You have lost because you lost all of your food \n");
				game=1;
			}else if(x->xPos==1 && x->yPos==1){
				printf("You have won good job!!!! \n");
				fflush(stdout);
				printf("Food: ");
				fflush(stdout);
				printf("%d\n",foodCount);
				fflush(stdout);
				game=1;
			}else{
				printViewPort(viewPort,viewPortSize);
				printf("Food: ");
				fflush(stdout);
				printf("%d\n",foodCount);
				fflush(stdout);
				printf("What would you like to do? ");
				fflush(stdout);
				scanf("%s", data);
				if(strcmp(data,"U")==0){
					foodCount=moving(viewPort,0,foodCount,headPtr,viewPortSize,mapSize);
				}else if(strcmp(data,"D")==0){
					foodCount=moving(viewPort,1,foodCount,headPtr,viewPortSize,mapSize);
				}else if(strcmp(data,"L")==0){
					foodCount=moving(viewPort,2,foodCount,headPtr,viewPortSize,mapSize);
				}else if(strcmp(data,"R")==0){
					foodCount=moving(viewPort,3,foodCount,headPtr,viewPortSize,mapSize);
				}else if(strcmp(data,"Q")==0){
					game=1;
				}else{
					printf("Wrong input!!!!\n");
					fflush(stdout);
				}
			}
		}
		if(foodCount<0){
			printf("You have lost because you lost all of your food \n");
		}
		NODE *prev,*curr;
		prev=(*viewPort);
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
				deleteNode(viewPort,curr,1);
				curr=prev;
				prev=prev->left;
			}
			if(prev->up->up!=NULL){
				deleteNode(viewPort,curr,1);
				curr=prev;
				prev=prev->up;
				while(prev->right!=NULL){
					prev=prev->right;
				}
				deleteNode(viewPort,curr,1);
				curr=prev;
				prev=prev->left;
			}else{
				deleteNode(viewPort,curr,1);
				curr=prev;
				prev=prev->up;
				deleteNode(viewPort,curr,1);
				while(prev->right!=NULL){
					prev=prev->right;
				}
				curr=prev;
				while(prev->left!=NULL){
					prev=prev->left;
					deleteNode(viewPort,curr,0);
					curr=prev;
				}
				deleteNode(viewPort,curr,0);
				over=true;
			}
		}
	}
}
