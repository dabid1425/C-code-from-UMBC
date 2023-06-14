/*
 * UpdatingView.h
 *
 *  Created on: Oct 29, 2014
 *      Author: dan
 */

#ifndef UPDATINGVIEW_H_
#define UPDATINGVIEW_H_
#include "LinkedList.h"
void swap(NODE * x, NODE* prev);
NODE * findPrev(NODE** viewPort,int whichWay, NODE * x);
void movingUP(NODE** viewPort,NODE** headPtr,int viewPortSize);
void movingRight(NODE** viewPort,NODE** headPtr,int viewPortSize);
void movingDown(NODE** viewPort,NODE** headPtr,int viewPortSize);
void movingLeft(NODE** viewPort,NODE** headPtr,int viewPortSize);
#endif /* UPDATINGVIEW_H_ */
