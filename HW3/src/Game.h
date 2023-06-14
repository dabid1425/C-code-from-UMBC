/*
 * Game.h
 *
 *  Created on: Oct 28, 2014
 *      Author: dan
 */

#ifndef GAME_H_
#define GAME_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "UpdatingView.h"
void play(NODE ** headPtr,NODE ** viewPort,int foodCount,int viewPortSize,int mapSize);
int moving(NODE ** viewPort,int whichWay,int foodCount, NODE ** headPtr, int viewPortSize,int mapSize);
void makeDiscoverable(NODE ** headPtr, int row,int column,NODE ** viewPort);
int notAnimal(NODE * pos,int foodCount);
int animal(NODE * pos,int foodCount);
void whatAnimal(NODE * pos);
#endif /* GAME_H_ */
