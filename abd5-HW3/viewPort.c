/*
 * viewPort.c
 *
 *  Created on: Oct 28, 2014
 *      Author: dan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "viewPort.h"
//inserts a node to the right of the x node
void InsertViewPortRight(NODE **viewPort, NODE* temp){
	NODE * prev,*curr;
	prev=(*viewPort);
	curr=prev->down;
	if(prev->xPos==temp->xPos){
		while(prev->yPos+1!=temp->yPos){
			prev=prev->right;
		}
		prev->right=temp;
		temp->left=prev;
	}else{
		while(curr->xPos!=temp->xPos){
			curr=curr->down;
		}
		while(curr->yPos+1!=temp->yPos){
			curr=curr->right;
		}
		prev=curr->up->right;
		temp->up=prev;
		prev->down=temp;
		curr->right=temp;
		temp->left=curr;
	}
}
//inserts a node to the left of the x node
void InsertViewPortLeft(NODE **viewPort, NODE* temp){
	NODE * prev,*curr;
	prev=(*viewPort);
	curr=prev->down;
	if(temp->xPos==prev->xPos){
		temp->right=prev;
		prev->left=temp;
	}else{
		while(curr->xPos!=temp->xPos){
			curr=curr->down;
		}
		prev=curr->up->left;
		curr->left=temp;
		temp->right=curr;
		prev->down=temp;
		temp->up=prev;
	}

}
//inserts a node above the x node
void InsertViewPortUp (NODE **viewPort, NODE* temp){
	NODE * prev,*curr;
	prev=(*viewPort);
	curr=prev->right;
	if(temp->yPos==prev->yPos){
		temp->down=prev;
		prev->up=temp;
	}else{
		while(curr->yPos!=temp->yPos){
			curr=curr->right;
		}
		prev=curr->left->up;
		curr->up=temp;
		temp->down=curr;
		prev->right=temp;
		temp->left=prev;
	}

}
//gets the initial viewPort
void getViewPort(NODE ** headPtr, NODE** viewPort,int viewPortSize,int mapSize){
	NODE * x, *curr,*temp;
	x=getXNode(headPtr);
	curr=x;
	int k=viewPortSize/2;
	int i,j;
	i=0;
	while(i<k){
		if(curr->left!=NULL){
			curr=curr->left;
		}
		i++;
	}
	i=0;
	while(i<k){
		if(curr->up!=NULL){
			curr=curr->up;
		}
		i++;
	}
	for(i=0;i<viewPortSize;i++){
		for(j=0;j<viewPortSize;j++){
			if(curr->ThreatType==NONE){
				temp = CreateNode((curr->type.typeofNone->animalOrDisease[0]),curr->xPos,curr->yPos);
			}else if(curr->ThreatType==ANIMAL){
				temp = CreateNode((curr->type.typeofAnimal->animalOrDisease[0]),curr->xPos,curr->yPos);
			}else{
				temp = CreateNode((curr->type.typeofDisease->animalOrDisease[0]),curr->xPos,curr->yPos);
			}
			InsertViewPort (viewPort, temp);
			if(curr->right!=NULL){
				curr=curr->right;
			}else{
				while(j>0){
					curr=curr->left;
					j--;
				}
				break;
			}
		}
		if(curr->down!=NULL){
			curr=curr->down;
		}else{
			break;
		}
	}
}
//prints the viewPort
void printViewPort(NODE** viewPort,int viewPortSize){
	int i,j;
	NODE * prev,*curr;
	prev=(*viewPort);
	curr=prev->down;
	NODE *x=getXNode(viewPort);
	for(i=0;i<viewPortSize;i++){
		int k=viewPortSize/2;
		int diff=x->xPos-k;
		diff=diff*-1;
		if(diff>=0){
			while(i<=diff){
				j=0;
				while(j<viewPortSize){
					printf("%c",'O');
					fflush(stdout);
					j++;
				}
				printf("\n");
				fflush(stdout);
				i++;
			}
		}
		for(j=0;j<viewPortSize;j++){
			int k=viewPortSize/2;
			int diff=x->yPos-k;
			diff=diff*-1;
			if(diff>=0){
				while(j<=diff){
					printf("%c",'O');
					fflush(stdout);
					j++;
				}
			}
			if(prev->discovered==1 && prev->aliveOrDead==1){
				printf("%c",prev->type.typeofNone->animalOrDisease[0]);
				fflush(stdout);
			}else if(prev->discovered==0 && prev->aliveOrDead==1){
				printf("%c",'U');
				fflush(stdout);
			}else{
				printf("%c",'N');
				fflush(stdout);
			}
			if(prev->right!=NULL){
				prev=prev->right;
			}else{
				while(j<viewPortSize-1){
					printf("%c",'O');
					fflush(stdout);
					j++;
				}
				break;
			}
		}
		if(prev->down!=NULL){
			prev=curr;
			curr=curr->down;
		}else{
			printf("\n");
			fflush(stdout);
			while(i<viewPortSize-1){
				j=0;
				while(j<viewPortSize){
					printf("%c",'O');
					fflush(stdout);
					j++;
				}
				printf("\n");
				fflush(stdout);
				i++;
			}
			break;
		}
		printf("\n");
		fflush(stdout);
	}
}
//gets where the X node is
NODE * getXNode(NODE ** headPtr){
	NODE *prev, *curr;
	prev=(*headPtr);
	curr=prev->down;
	while(curr!=NULL){
		while(prev!=NULL){
			if(prev->type.typeofNone->animalOrDisease[0]=='X'){
				return prev;
			}
			prev=prev->right;
		}
		prev=curr;
		curr=curr->down;
	}
	while(prev!=NULL){
		if(prev->type.typeofNone->animalOrDisease[0]=='X'){
			return prev;
		}
		prev=prev->right;
	}
	return prev;
}
//inserts a node into the viewPort
void InsertViewPort(NODE ** headPtr, NODE * newdata){
	if ((*headPtr) == NULL){
		(*headPtr)=newdata;
	}else{
		NODE *prev, *curr, *temp;
		prev=(*headPtr);
		curr=prev->down;
		while(prev->xPos < newdata->xPos && curr!=NULL){
			prev=curr;
			curr=curr->down;
		}
		if(curr==NULL){
			if(newdata->yPos==prev->yPos){
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
				if(newdata->xPos!=(*headPtr)->xPos){
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
}
