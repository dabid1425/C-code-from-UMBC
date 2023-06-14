/*
 * viewPort.h
 *
 *  Created on: Oct 28, 2014
 *      Author: dan
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
void InsertViewPortRight(NODE **viewPort, NODE* temp);
void InsertViewPortLeft(NODE **viewPort, NODE* temp);
void InsertViewPortUp(NODE **viewPort, NODE *temp);
void InsertViewPort(NODE ** headPtr, NODE * newdata);
NODE * getXNode(NODE ** headPtr);
void printViewPort(NODE** viewPort,int viewPortSize);
void getViewPort(NODE ** headPtr, NODE** viewPort,int viewPortSize,int mapSize);

#endif /* VIEWPORT_H_ */
