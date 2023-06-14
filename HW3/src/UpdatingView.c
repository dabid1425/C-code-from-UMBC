/*
 * UpdatingView.c
 *
 *  Created on: Oct 29, 2014
 *      Author: dan
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "viewPort.h"
#include "UpdatingView.h"
//swaps the x node with the previous node just switches the char array
void swap(NODE * x, NODE* prev){
	char dummy[1];
	dummy[0]=x->type.typeofNone->animalOrDisease[0];
	x->type.typeofNone->animalOrDisease[0]=prev->type.typeofNone->animalOrDisease[0];
	prev->type.typeofNone->animalOrDisease[0]=dummy[0];
	if(x->type.typeofNone->prev[0]=='R'){
		x->type.typeofNone->animalOrDisease[0]='R';
	}else if(x->type.typeofNone->prev[0]=='F' ||x->type.typeofNone->prev[0]=='D' ||
			x->type.typeofNone->prev[0]=='C'){
		x->type.typeofNone->animalOrDisease[0]=x->type.typeofNone->prev[0];
	}
}
//finds the value next to the x node if whichWay=0 it looks one up if whichWay=1 it looks one down whichWay=2 it looks one left whichWay=3 it looks one right
NODE * findPrev(NODE** viewPort,int whichWay, NODE * x){
	NODE *prev,*curr;
	prev=(*viewPort);
	curr=prev->down;
	if(whichWay==0){
		while(prev->xPos!=x->xPos-1){
			prev=curr;
			curr=curr->down;
		}
		curr=prev;
		curr=curr->right;
		while(prev->yPos!=x->yPos){
			prev=curr;
			curr=curr->right;
		}
	}else if(whichWay==1){
		while(prev->xPos!=x->xPos+1){
			prev=curr;
			curr=curr->down;
		}
		curr=prev;
		curr=curr->right;
		while(prev->yPos!=x->yPos){
			prev=curr;
			curr=curr->right;
		}
	}else if(whichWay==2){
		while(prev->xPos!=x->xPos){
			prev=curr;
			curr=curr->down;
		}
		curr=prev->right;
		while(prev->yPos+1!=x->yPos){
			prev=curr;
			curr=curr->right;
		}
	}else if(whichWay==3){
		while(prev->xPos!=x->xPos){
			prev=curr;
			curr=curr->down;
		}
		curr=prev->right;
		while(prev->yPos!=x->yPos+1){
			prev=curr;
			curr=curr->right;
		}
	}

	return prev;
}
//moves the x node one up and will insert a row and delete a row when needed
void movingUP(NODE** viewPort,NODE** headPtr,int viewPortSize){
	NODE *x=getXNode(viewPort);
	NODE* prev=findPrev(viewPort,0,x);
	swap(x,prev);
	prev=findPrev(headPtr,0,x);
	int k=viewPortSize/2;
	int i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->up;
		}
		i++;
	}
	if(prev!=NULL){
		InsertRow(viewPort,headPtr,prev->xPos,0);
		prev=(*viewPort);
		prev=prev->up;
		(*viewPort)=prev;
	}
	prev=x;
	i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->down;
		}
		i++;
	}
	if(prev!=NULL){
		DeleteRow(viewPort,prev->xPos,0);
	}

}
//moves the x node one over to the right and will insert a column and delete a column when needed
void movingRight(NODE** viewPort,NODE** headPtr,int viewPortSize){
	NODE *x=getXNode(viewPort);
	NODE* prev=findPrev(viewPort,3,x);
	swap(x,prev);
	prev=findPrev(headPtr,3,x);
	int k=viewPortSize/2;
	int i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->right;
		}
		i++;
	}
	if(prev!=NULL){
		InsertColumn(viewPort,headPtr,prev->yPos,1);
	}
	prev=x;
	i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->left;
		}
		i++;
	}
	if(prev!=NULL){
		x=(*viewPort);
		x=x->right;
		DeleteColumn(viewPort,prev->yPos,0);
		(*viewPort)=x;
	}
}
//moves the x node one down and will insert a row and delete a row when needed
void movingDown(NODE** viewPort,NODE** headPtr,int viewPortSize){
	NODE *x=getXNode(viewPort);
	NODE* prev=findPrev(viewPort,1,x);
	swap(x,prev);
	prev=findPrev(headPtr,1,x);
	int k=viewPortSize/2;
	int i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->down;
		}
		i++;
	}
	if(prev!=NULL){
		InsertRow(viewPort,headPtr,prev->xPos,1);
	}
	prev=x;
	i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->up;
		}
		i++;
	}
	if(prev!=NULL){
		x=(*viewPort);
		x=x->down;
		DeleteRow(viewPort,prev->xPos,1);
		(*viewPort)=x;
	}


}
//moves the x node one over to the left and will insert a column and delete a column when needed
void movingLeft(NODE** viewPort,NODE** headPtr,int viewPortSize){
	NODE *x=getXNode(viewPort);
	NODE* prev=findPrev(viewPort,2,x);
	swap(x,prev);
	prev=findPrev(headPtr,2,x);
	int k=viewPortSize/2;
	int i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->left;
		}
		i++;
	}
	if(prev!=NULL){
		InsertColumn(viewPort,headPtr,prev->yPos,0);
		prev=(*viewPort);
		prev=prev->left;
		(*viewPort)=prev;
	}

	prev=x;
	i=0;
	while(i<k){
		if(prev!=NULL){
			prev=prev->right;
		}
		i++;
	}
	if(prev!=NULL){
		DeleteColumn(viewPort,prev->yPos,1);
	}
}
