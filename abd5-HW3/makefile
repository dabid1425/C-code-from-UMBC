CC=gcc
CFLAGS=-g -Wall
TARGET = HW3.out

default:HW3

HW3: Game.o LinkedList.o UpdatingView.o viewPort.o HW3.o
	$(CC) $(CFLAGS) -o $(TARGET) Game.o LinkedList.o UpdatingView.o viewPort.o HW3.o 

HW3.o: HW3.c Game.h LinkedList.h viewPort.h UpdatingView.h
	$(CC) $(CFLAGS) -c HW3.c  

Game.o: Game.c Game.h LinkedList.h viewPort.h UpdatingView.h 
	$(CC) $(CFLAGS) -c Game.c
LinkedList.o: LinkedList.c LinkedList.h viewPort.h
	$(CC) $(CFLAGS) -c LinkedList.c 

UpdatingView.o: UpdatingView.c LinkedList.h viewPort.h UpdatingView.h
	$(CC) $(CFLAGS) -c UpdatingView.c

viewPort.o: viewPort.c LinkedList.h viewPort.h 
	$(CC) $(CFLAGS) -c viewPort.c

clean:
	$(RM) $(TARGET) *.o *~