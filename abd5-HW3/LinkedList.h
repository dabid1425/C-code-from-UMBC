/*
 * LinkedList.h
 *
 *  Created on: Oct 27, 2014
 *      Author: dan
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
typedef struct node * NODEPTR;

typedef int bool;
#define true 1
#define false 0
typedef struct disease{
	char animalOrDisease[1];
}DISEASE_P;
typedef struct animal{
	char animalOrDisease[1];
}ANIMAL_P;
typedef struct none{
	char animalOrDisease[1];
	char prev[1];
}NONE_P;
union threat{
	DISEASE_P * typeofDisease;
	ANIMAL_P * typeofAnimal;
	NONE_P * typeofNone;
};
typedef struct node
{
	enum {DISEASE, ANIMAL,NONE} ThreatType;
	union threat type;
	int xPos;
	int yPos;
	bool discovered;
	int aliveOrDead;
	NODEPTR left;    /* OR struct node *next; */
	NODEPTR right;    /* OR struct node *next; */
	NODEPTR up;    /* OR struct node *next; */
	NODEPTR down;    /* OR struct node *next; */

}NODE;
void deleteNode(NODE ** headPtr, NODE * newdata,int lastRow);
void InsertColumn(NODE **viewPort,NODE **headPtr, int row,int uporDown);
void InsertRow(NODE **viewPort,NODE **headPtr, int row,int uporDown);
void DeleteRow(NODE ** list, int row,int upOrDown);
void DeleteColumn(NODE ** list, int row,int rightOrLeft);
void getViewPortThree(NODE ** headPtr, NODE** viewPort);
void getViewPortFive(NODE ** headPtr, NODE** viewPort);
NODE* CreateNode(char line,int row, int column);
void Insert(NODE ** headPtr, NODE * newdata);
void InsertViewPort(NODE ** headPtr, NODE * newdata);
NODE * getXNode(NODE ** headPtr);
void printViewPort(NODE** viewPort,int viewPortSize);
#endif /* LINKEDLIST_H_ */
