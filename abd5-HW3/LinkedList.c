/*
 * LinkedList.c
 *
 *  Created on: Oct 27, 2014
 *      Author: dan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "viewPort.h"
//inserts the column if uporDown=0 inserts left if =1 inserts right
void InsertColumn(NODE **viewPort,NODE **headPtr, int row,int uporDown){
	NODE *prev,*curr,*temp;
	prev=(*headPtr);
	NODE *x=(*viewPort);
	curr=prev->down;
	while(prev->xPos!=x->xPos){
		prev=curr;
		curr=curr->down;
	}
	curr=prev;
	while(prev->yPos!=row){
		prev=curr;
		curr=curr->right;
	}
	while(x->down!=NULL){
		x=x->down;
	}
	if(uporDown==0){
		curr=prev;
		while(prev!=NULL){
			if(prev->ThreatType==NONE){
				temp=CreateNode(prev->type.typeofNone->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=NONE;
			}else if(prev->ThreatType==ANIMAL){
				temp=CreateNode(prev->type.typeofAnimal->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=ANIMAL;
			}else{
				temp=CreateNode(prev->type.typeofDisease->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=DISEASE;
			}
			temp->discovered=prev->discovered;
			temp->aliveOrDead=prev->aliveOrDead;
			temp->type.typeofNone->prev[0]=curr->type.typeofNone->prev[0];
			InsertViewPortLeft(viewPort,temp);
			if(curr->xPos!=x->xPos){
				curr=curr->down;
				prev=curr;
			}else{
				prev=NULL;
			}
		}
	}else{
		curr=prev;
		while(prev!=NULL){
			if(prev->ThreatType==NONE){
				temp=CreateNode(prev->type.typeofNone->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=NONE;
			}else if(prev->ThreatType==ANIMAL){
				temp=CreateNode(prev->type.typeofAnimal->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=ANIMAL;
			}else{
				temp=CreateNode(prev->type.typeofDisease->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=DISEASE;
			}
			temp->discovered=prev->discovered;
			temp->aliveOrDead=prev->aliveOrDead;
			temp->type.typeofNone->prev[0]=curr->type.typeofNone->prev[0];
			InsertViewPortRight(viewPort,temp);
			if(curr->xPos!=x->xPos){
				curr=curr->down;
				prev=curr;
			}else{
				prev=NULL;
			}
		}
	}


}
//inserts the row if uporDown=0 inserts up if =1 inserts down
void InsertRow(NODE **viewPort,NODE **headPtr, int row,int uporDown){
	NODE *prev,*curr,*temp;
	prev=(*headPtr);
	NODE *x=(*viewPort);
	curr=prev->down;
	while(prev->xPos!=row){
		prev=curr;
		curr=curr->down;
	}
	curr=prev;
	while(prev->yPos!=x->yPos){
		prev=curr;
		curr=curr->right;
	}
	while(x->right!=NULL){
		x=x->right;
	}
	if(uporDown==0){
		curr=prev;
		while(prev!=NULL){
			if(prev->ThreatType==NONE){
				temp=CreateNode(prev->type.typeofNone->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=NONE;
			}else if(prev->ThreatType==ANIMAL){
				temp=CreateNode(prev->type.typeofAnimal->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=ANIMAL;
			}else{
				temp=CreateNode(prev->type.typeofDisease->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=DISEASE;
			}
			temp->discovered=prev->discovered;
			temp->aliveOrDead=prev->aliveOrDead;
			temp->type.typeofNone->prev[0]=curr->type.typeofNone->prev[0];
			InsertViewPortUp(viewPort,temp);
			if(curr->yPos!=x->yPos){
				curr=curr->right;
				prev=curr;
			}else{
				prev=NULL;
			}

		}
	}else{
		curr=prev;
		while(prev!=NULL){
			if(prev->ThreatType==NONE){
				temp=CreateNode(prev->type.typeofNone->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=NONE;
			}else if(prev->ThreatType==ANIMAL){
				temp=CreateNode(prev->type.typeofAnimal->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=ANIMAL;
			}else{
				temp=CreateNode(prev->type.typeofDisease->animalOrDisease[0],prev->xPos,prev->yPos);
				temp->ThreatType=DISEASE;
			}
			temp->discovered=prev->discovered;
			temp->aliveOrDead=prev->aliveOrDead;
			temp->type.typeofNone->prev[0]=curr->type.typeofNone->prev[0];
			InsertViewPort(viewPort,temp);
			if(curr->yPos!=x->yPos){
				curr=curr->right;
				prev=curr;
			}else{
				prev=NULL;
			}
		}
	}

}
//deletes the column if rightOrLeft=0 deletes column to the right if rightOrLeft=1 deletes column to the left
void DeleteColumn(NODE ** list, int column,int rightOrLeft){
	NODE* prevNode, *currNode;
	//set curr node to head
	if(rightOrLeft==0){
		prevNode = (*list);
		currNode=prevNode;
		while(currNode->yPos!=column){
			prevNode=currNode;
			currNode=currNode->right;
		}
		prevNode=currNode;
		while(prevNode->down!=NULL){
			currNode->right->left=NULL;
			currNode->right=NULL;
			prevNode=currNode->down;
			prevNode->up=NULL;
			currNode->down=NULL;
			if(currNode->ThreatType==NONE){
				free(currNode->type.typeofNone);
			}else if(currNode->ThreatType==ANIMAL){
				free(currNode->type.typeofAnimal);
			}else if(currNode->ThreatType==DISEASE){
				free(currNode->type.typeofDisease);
			}
			free(currNode);
			currNode=prevNode;
		}
		currNode->right->left=NULL;
		currNode->right=NULL;
		if(currNode->ThreatType==NONE){
			free(currNode->type.typeofNone);
		}else if(currNode->ThreatType==ANIMAL){
			free(currNode->type.typeofAnimal);
		}else if(currNode->ThreatType==DISEASE){
			free(currNode->type.typeofDisease);
		}
		free(currNode);
	}else{
		prevNode = (*list);
		currNode=prevNode;
		while(currNode->yPos!=column){
			prevNode=currNode;
			currNode=currNode->right;
		}
		prevNode=currNode;
		while(prevNode->down!=NULL){
			currNode->left->right=NULL;
			currNode->left=NULL;
			prevNode=currNode->down;
			prevNode->up=NULL;
			currNode->down=NULL;
			if(currNode->ThreatType==NONE){
				free(currNode->type.typeofNone);
			}else if(currNode->ThreatType==ANIMAL){
				free(currNode->type.typeofAnimal);
			}else if(currNode->ThreatType==DISEASE){
				free(currNode->type.typeofDisease);
			}
			free(currNode);
			currNode=prevNode;
		}
		currNode->left->right=NULL;
		currNode->left=NULL;
		if(currNode->ThreatType==NONE){
			free(currNode->type.typeofNone);
		}else if(currNode->ThreatType==ANIMAL){
			free(currNode->type.typeofAnimal);
		}else if(currNode->ThreatType==DISEASE){
			free(currNode->type.typeofDisease);
		}
		free(currNode);
	}
}
//deletes a row if upOrDown=0 deletes row above x if =1 deletes row below x
void DeleteRow(NODE ** list, int row,int upOrDown) {
	//Create pointers to nodes, prev and curr
	NODE* prevNode, *currNode;
	//set curr node to head
	if(upOrDown==0){
		prevNode = (*list);
		currNode=prevNode->down;
		while(prevNode->xPos!=row){
			prevNode = currNode;
			currNode=currNode->down;
		}
		currNode=prevNode;
		while(prevNode->right!=NULL){
			prevNode=currNode;
			currNode=currNode->right;
		}
		currNode=prevNode;
		while(prevNode->left!=NULL){
			if(currNode->up!=NULL){
				currNode->up->down=NULL;
				currNode->up=NULL;
			}
			prevNode=currNode->left;
			prevNode->right=NULL;
			currNode->left=NULL;
			if(currNode->ThreatType==NONE){
				free(currNode->type.typeofNone);
			}else if(currNode->ThreatType==ANIMAL){
				free(currNode->type.typeofAnimal);
			}else if(currNode->ThreatType==DISEASE){
				free(currNode->type.typeofDisease);
			}
			free(currNode);
			currNode=prevNode;
		}
		if(currNode->up!=NULL){
			currNode->up->down=NULL;
			currNode->up=NULL;
		}
		if(currNode->ThreatType==NONE){
			free(currNode->type.typeofNone);
		}else if(currNode->ThreatType==ANIMAL){
			free(currNode->type.typeofAnimal);
		}else if(currNode->ThreatType==DISEASE){
			free(currNode->type.typeofDisease);
		}
		free(currNode);
	}else{
		prevNode = (*list);
		currNode=prevNode->up;
		if(prevNode->xPos!=row){
			prevNode=currNode;
			currNode=currNode->up;
		}
		currNode=prevNode;
		while(prevNode->right!=NULL){
			prevNode=currNode;
			currNode=currNode->right;
		}
		currNode=prevNode;
		while(prevNode->left!=NULL){
			if(currNode->down!=NULL){
				currNode->down->up=NULL;
				currNode->down=NULL;
			}
			prevNode=currNode->left;
			prevNode->right=NULL;
			currNode->left=NULL;
			if(currNode->ThreatType==NONE){
				free(currNode->type.typeofNone);
			}else if(currNode->ThreatType==ANIMAL){
				free(currNode->type.typeofAnimal);
			}else if(currNode->ThreatType==DISEASE){
				free(currNode->type.typeofDisease);
			}
			free(currNode);
			currNode=prevNode;
		}
		if(currNode->down!=NULL){
			currNode->down->up=NULL;
			currNode->down=NULL;
		}
		if(currNode->ThreatType==NONE){
			free(currNode->type.typeofNone);
		}else if(currNode->ThreatType==ANIMAL){
			free(currNode->type.typeofAnimal);
		}else if(currNode->ThreatType==DISEASE){
			free(currNode->type.typeofDisease);
		}
		free(currNode);

	}

}
// inserts a new node
void Insert(NODE ** headPtr, NODE * newdata){
	if ((*headPtr) == NULL){
		(*headPtr)=newdata;
	}else{
		NODE *prev, *curr,*temp;
		prev=(*headPtr);
		curr=prev->down;
		while(prev->xPos < newdata->xPos && curr!=NULL){
			prev=curr;
			curr=curr->down;
		}
		if(newdata->yPos==1){
			prev->down=newdata;
			newdata->up=prev;
		}else{
			curr=prev->right;
			while(newdata->yPos > prev->yPos && curr!=NULL){
				prev=curr;
				curr=curr->right;
			}
			prev->right=newdata;
			temp=prev->right;
			temp->left=prev;
			if(newdata->xPos!=1){
				prev=prev->up;
				curr=prev->right;
				while(newdata->yPos > prev->yPos && curr!=NULL){
					prev=curr;
					curr=curr->right;
				}
				prev->down=newdata;
				newdata->up=prev;

			}
		}
	}
}
//deletes a node
void deleteNode(NODE ** headPtr, NODE * newdata,int lastRow){
	NODE *prev,*curr;
	prev=(*headPtr);
	curr=prev->down;
	if(lastRow==1){
		while(curr->xPos!=newdata->xPos){
			prev=curr;
			curr=curr->down;
		}
		prev=curr;
		if(curr->right!=NULL){
			while(curr->right!=newdata){
				prev=curr;
				curr=curr->right;
			}
		}
		curr->right=NULL;
		newdata->left=NULL;
		newdata->up=NULL;
		if(curr->up!=NULL){
			curr=curr->up->right;
			curr->down=NULL;
		}
	}else{
		curr=prev;
		if(curr->right!=NULL){
			while(curr->right!=newdata){
				prev=curr;
				curr=curr->right;
			}
		}
		curr->right=NULL;
		newdata->left=NULL;
	}
	if(newdata->ThreatType==NONE){
		free(newdata->type.typeofNone);
	}else if(newdata->ThreatType==ANIMAL){
		free(newdata->type.typeofAnimal);
	}else if(newdata->ThreatType==DISEASE){
		free(newdata->type.typeofDisease);
	}
	free(newdata);

}
//creates a node
NODE* CreateNode(char line,int row, int column){
	NODE * newnode;
	newnode = (NODE *) malloc (sizeof(NODE));
	if (newnode == NULL){
		printf ("No Memory left !!\n");
		fflush(stdout);
		return NULL;
	}
	if(line == 'O' || line == 'X'||line == 'Z'||line == 'U'||line == 'R'){
		newnode->discovered = true;
		newnode->aliveOrDead=1;
		newnode->ThreatType=NONE;
		newnode->type.typeofNone=malloc(sizeof(NONE_P));
		if(line == 'O'){
			newnode->type.typeofNone->animalOrDisease[0]='O';
			newnode->type.typeofNone->prev[0]='O';
		}else if(line == 'X'){
			newnode->type.typeofNone->animalOrDisease[0]='X';
			newnode->type.typeofNone->prev[0]='X';
		}else if(line == 'Z'){
			newnode->type.typeofNone->animalOrDisease[0]='Z';
			newnode->type.typeofNone->prev[0]='Z';
		}else if(line == 'U'){
			newnode->type.typeofNone->animalOrDisease[0]='U';
			newnode->type.typeofNone->prev[0]='U';
		}else{
			newnode->type.typeofNone->animalOrDisease[0]='R';
			newnode->type.typeofNone->prev[0]='R';
		}
	}else{
		if(line=='G'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='G';
			newnode->type.typeofNone->prev[0]='G';
		}else if(line == 'B'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='B';
			newnode->type.typeofNone->prev[0]='B';
		}else if(line == 'E'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='E';
			newnode->type.typeofNone->prev[0]='E';
		}else if(line == 'H'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='H';
			newnode->type.typeofNone->prev[0]='H';
		}else if(line == 'C'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='C';
			newnode->type.typeofNone->prev[0]='C';
		}else if(line == 'F'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='F';
			newnode->type.typeofNone->prev[0]='F';
		}else if(line == 'D'){
			newnode->ThreatType=NONE;
			newnode->type.typeofNone=malloc(sizeof(NONE_P));
			newnode->type.typeofNone->animalOrDisease[0]='D';
			newnode->type.typeofNone->prev[0]='D';
		}
		newnode->discovered = false;
		newnode->aliveOrDead=1;
	}
	newnode->xPos=row;
	newnode->yPos=column;
	newnode->left = NULL;
	newnode->up = NULL;
	newnode->down = NULL;
	newnode->right = NULL;
	return newnode;
}
