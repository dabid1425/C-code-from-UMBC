/*
 * HW2.c
 *
 * Created: 10/5/2013 2:50:53 PM
 *  Author: dan
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "U0_UART.h"
#include <stdbool.h>
#include <util/delay.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#define PORTB5_SPEAKER_MASK 0b00100000
#define USER_LINE_MAX 32
#define NUMBER_OF_SONGS 4
#define MAX_SONG_LENGTH 64
#define FREQ_A4_HZ 440
#define FREQ_B4_HZ 493.88
#define FREQ_C4_HZ 261.63
#define FREQ_D4_HZ 293.66
#define FREQ_E4_HZ 329.63
#define FREQ_F4_HZ 349.23
#define FREQ_G4_HZ 392
#define HALFPERIOD_G4_US  1276 //delay in microseconds
#define HALFPERIOD_A4_US  1136//delay in microseconds
#define HALFPERIOD_B4_US  1012//delay in microseconds
#define HALFPERIOD_C4_US  1911//delay in microseconds
#define HALFPERIOD_D4_US  1703//delay in microseconds
#define HALFPERIOD_E4_US  1517//delay in microseconds
#define HALFPERIOD_F4_US  1432//delay in microseconds
#define NOTE_A 0b00000000
#define NOTE_B 0b00000001
#define NOTE_C 0b00000010
#define NOTE_D 0b00000011
#define NOTE_E 0b00000100
#define NOTE_F 0b00000101
#define NOTE_G 0b00000110
#define NOTE_R 0b00000111
extern FILE uart_stream;
char letterASCII;
int track=0;
uint8_t duration;
char songTitles [NUMBER_OF_SONGS][USER_LINE_MAX] ={"Hot cross buns","Scale","Hello hello","Hello hello there"};
const char mainMenu[4][32] PROGMEM = {"----Main Menu---\n","1. List Songs \n","2. Play Song\n","3. Create Song\n" };
const char playMenu[3][32] PROGMEM= {"----Play Song Menu---\n","1. Play by number\n","2. Search By Title\n"};    

//packs the note in binary it checks which note it is and shifts the note over 5 and then adds the 

uint8_t PackNote(char letterASCII, uint8_t duration){
	//packs the letter and note 
	uint8_t packedNote = 0;
	if(letterASCII=='A' || letterASCII=='a'){
		packedNote=(NOTE_A<<5)+duration;
	}else if(letterASCII=='B' || letterASCII=='b'){
		packedNote=(NOTE_B<<5)+duration;
	}else if(letterASCII=='C' || letterASCII=='c'){
		packedNote=(NOTE_C<<5)+duration;
	}else if(letterASCII=='D' || letterASCII=='d'){
		packedNote=(NOTE_D<<5)+duration;
	}else if(letterASCII=='E' || letterASCII=='e'){
		packedNote=(NOTE_E<<5)+duration;
	}else if(letterASCII=='F' || letterASCII=='f'){
		packedNote=(NOTE_F<<5)+duration;
	}else if(letterASCII=='G' || letterASCII=='g'){
		packedNote=(NOTE_G<<5)+duration;
	}else if(letterASCII=='R' || letterASCII=='r'){
		packedNote=(NOTE_R<<5)+duration;
	}
	return packedNote;

}
//unpacks the note duration by bitwise anding the packed note with 0b00011111
uint8_t UnpackNoteDuration(uint8_t packedNote){
	return packedNote&0b00011111;
}
//unpacks the note letter by shifting the packed value over 5 and then adding A to it if it is equal to H return an R
uint8_t UnpackNoteLetterASCII(uint8_t packedNote){
	if(((packedNote>>5)+'A')=='H'){
		return 'R';
	}
	return (packedNote>>5)+'A';

}
//plays letter for some amount of quarter notes
void PlayNote(uint8_t letterASCII, uint8_t duration){
	int numIterations=0;
	int i=0;
	if(letterASCII=='A'){
		numIterations=(FREQ_A4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_A4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_A4_US);
			i++;
		}
	}else if(letterASCII=='B'){
		numIterations=(FREQ_B4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_B4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_B4_US);
			i++;
		}
	}else if(letterASCII=='C'){
		numIterations=(FREQ_C4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_C4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_C4_US);
			i++;
		}
	}else if(letterASCII=='D'){
		numIterations=(FREQ_D4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_D4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_D4_US);
			i++;
		}
	}else if(letterASCII=='E'){
		numIterations=(FREQ_E4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_E4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_E4_US);
			i++;
		}
	}else if(letterASCII=='F'){
		numIterations=(FREQ_F4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_F4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_F4_US);
			i++;
		}
	}else if(letterASCII=='G'){
		numIterations=(FREQ_G4_HZ/4)*duration;
		while(i<numIterations){
			PORTB &= ~(1<<5);//from discussion
			_delay_us(HALFPERIOD_G4_US);
			PORTB |= (1<<5);//from discussion
			_delay_us(HALFPERIOD_G4_US);
			i++;
		}
	}else if(letterASCII=='R'){
		numIterations=duration;
		for(int i=0;i<numIterations;i++){
			_delay_ms(1000);
		}
	}
}


//unpacks each packed
void PlaySong(uint8_t song[]){
	uint8_t stop=0;
	for(int track=0;track<strlen((char*)song);track++){
		if(stop==0){
			letterASCII=UnpackNoteLetterASCII(song[track]);
			duration=UnpackNoteDuration(song[track])-256;
			if(duration!=0){
				PlayNote(letterASCII,duration);
			}else{
				stop=1;
			}
		}

	}
	printf(PSTR("\n"));
}
//packs the notes and the duration 
void StoreSong(uint8_t song[],const char songString[]){
	int loopCounter=0;
	int i=0;
	for(int i=0;i<MAX_SONG_LENGTH;i++){
		song[i]=0;
	}
	int length= strlen(songString);
	while(loopCounter<length){
		char letterASCII=toupper(songString[loopCounter]);
		uint8_t duration=songString[loopCounter+1]-48;
		if(letterASCII==82 && duration==0){
			break;
		}else{
			if((songString[loopCounter+2]<=57) && (songString[loopCounter+2]>=48) && duration!=0 ){
				duration=duration*10;
				duration=duration+(songString[loopCounter+2]-48);
				if(duration>31){
					break;
				}
				song[i]=PackNote(letterASCII,duration);
				loopCounter=loopCounter+3;
				i++;
			}else{
				if(duration>31){
					break;
				}			
				song[i]=PackNote(letterASCII,duration);
				i++;
				loopCounter=loopCounter+2;
			}
		}
	}
	song[i]=PackNote('R',0);
}

void StripEOL(char string[],int n){
	string[n]=0;
}
//displays the main menu
uint8_t DisplayMenu(){
	printf_P(mainMenu[0]);
	printf_P(mainMenu[1]);
	printf_P(mainMenu[2]);
	printf_P(mainMenu[3]);
	uint8_t number;
	number=UARTGetch(stdin);
	number -=48;
	return number;
}
//lists all the songs
void ListSongs(char songTitles[NUMBER_OF_SONGS][USER_LINE_MAX]){
	for(int i=0;i<4;i++){
		printf("%d",i+1);
		printf(". ");
		printf(songTitles[i]);
		printf(PSTR("\n"));
	}
}

//try's to find the closest match to the of all the titles in the code and returns the score 
uint8_t MatchScore(const char countQueryString [],const char database[]){
	uint8_t score=0;
	char newData[USER_LINE_MAX]={0};
	char newDummy[USER_LINE_MAX]={0};
	char reset[USER_LINE_MAX]={0};
	int k=0;
	int saved=strlen(database);
	int b=0;
	while(k<=saved){
		newDummy[b]=tolower(database[k]);
		if(newDummy[b]==32  ||  newDummy[b]==0){
			newDummy[b]=32;
			b=-1;
			int saveLength=strlen(newData);
			if(saveLength==0){
				strcat(newData,newDummy);
			}else{
				if(strstr(newData,newDummy)==NULL){
					strcat(newData,newDummy);
				}
			}
			strncpy(newDummy,reset,USER_LINE_MAX);
		}
		k++;
		b++;
	}
	k=0;
	b=0;
	saved=strlen(countQueryString);
	while(k<=saved){
		newDummy[b]=tolower(countQueryString[k]);
		if(newDummy[b]==32  ||  newDummy[b]==0){
			newDummy[b]=32;
			b=-1;
			int a=0;
			int i=0;
			char newChar[USER_LINE_MAX]={0};
			while(i<=strlen(newData)){
				newChar[a]=newData[i];
				if(newChar[a]==32  ||  newChar[a]==0){
					a=-1;
					if(strcmp(newChar,newDummy)==0){
						score++;
					}
					strncpy(newChar,reset,USER_LINE_MAX);
				}
				a++;
				i++;
			}		
			strncpy(newDummy,reset,USER_LINE_MAX);
				
		}
		k++;
		b++;
	}
	return score;
}
int main(void){
	//song array 4 by 64 that contains all the notes 
	uint8_t song[NUMBER_OF_SONGS][MAX_SONG_LENGTH] =
		{{(NOTE_B<<5)+2 ,(NOTE_R<<5)+2,(NOTE_A<<5)+2,(NOTE_G<<5 )+2 , (NOTE_R<<5)},
			{(NOTE_A<<5)+2,(NOTE_B<<5)+2,(NOTE_C<<5)+2,(NOTE_D<<5)+2,(NOTE_E<<5)+2,(NOTE_F<<5)+2,(NOTE_G<<5)+2,NOTE_R<<5}
				,{(NOTE_C<<5)+2,(NOTE_B<<5)+2,NOTE_R<<5},{(NOTE_D<<5)+2,NOTE_R<<5}};
	DDRB = PORTB5_SPEAKER_MASK;//sets pin 5 to output
	PORTB = PORTB5_SPEAKER_MASK;
	stderr = stdout = stdin= &uart_stream;
	UARTInit();//set UART
	uint8_t trackToPlay;
	uint8_t choice;
	//keeps running 
	while(1){
		//ask user for input its a number so code does not need user to hit enter 
		choice=DisplayMenu();
		printf(PSTR("\n"));
		//if choice was 1 the code will print all the songs in the code
		if(choice==1){
			ListSongs(songTitles);
		}else if(choice==2){
			//code will print the options the user has to either hit 1 or 2
			printf_P(playMenu[0]);
			printf_P(playMenu[1]);
			printf_P(playMenu[2]);
			uint8_t number;
			number=UARTGetch(stdin);
			number -=48;
			printf(PSTR("\n"));
			if(number==1){
				//if the user hits one the code will ask the user which song to play enter 1 through 4
				printf(PSTR("Which song \n"));
				ListSongs(songTitles);
				trackToPlay=UARTGetch(stdin);
				trackToPlay -=48;
				if(trackToPlay==1){
					PlaySong(song[0]);
				}else if(trackToPlay==2){
					PlaySong(song[1]);
				}else if(trackToPlay==3){
					PlaySong(song[2]);
				}else if(trackToPlay==4){
					PlaySong(song[3]);
				}else{
					printf(PSTR("That wasn't a choice\n"));
				}
			}else if(number==2){
				//user has to enter the title they are looking for and the code will then search for the closest match 
				int counter=0;
				int score=0;
				int whichOne=-1;
				int oldScore=0;
				char songsNames[MAX_SONG_LENGTH]={0};
				printf_P(PSTR("Enter title of song: \n"));
				songsNames[0]=(char)UARTGetch(stdin);
				counter++;
				while(songsNames[counter-1]!='\n' && songsNames[counter-1]!='\r' && counter<USER_LINE_MAX){
					songsNames[counter]=(char)UARTGetch(stdin);
					if(songsNames[counter]!=8){
						counter++;
						}else{
						counter--;
					}
				}
				StripEOL(songsNames,counter-1);
				for(int i=0;i<4;i++){
					score=MatchScore(songsNames,songTitles[i]);
					if(score!=0){
						if(oldScore<score){
							oldScore=score;
							whichOne=i;
						}
					}
				}
				if(whichOne!=-1){
					printf(PSTR("\n"));
					PlaySong(song[whichOne]);
				}
			}
		}else if(choice==3){
			//here the code enters a new song and the notes 
			printf(PSTR("---Which song would you like to replace (Enter 1-4)---- \n"));
			ListSongs(songTitles);
			trackToPlay=UARTGetch(stdin);
			trackToPlay -=48;
			printf(PSTR("\n"));
			if(trackToPlay>4 || trackToPlay<0){
				printf(PSTR("That was not a choice\n"));
			}else{
				trackToPlay=trackToPlay-1;
				int counter=0;
				char songsTitle[USER_LINE_MAX]={0};
				printf(PSTR("What is the title of the song?\n"));
				songsTitle[0]=(char)UARTGetch(stdin);
				counter++;
				while(songsTitle[counter-1]!='\n' && songsTitle[counter-1]!='\r' && counter<USER_LINE_MAX){
					songsTitle[counter]=(char)UARTGetch(stdin);
					if(songsTitle[counter]!=8){
						counter++;
					}else{
						counter--;
					}
				}
				printf(PSTR("\n"));
				StripEOL(songsTitle,counter-1);
				int i;
				for(i=0;i<USER_LINE_MAX;i++){
					songTitles[trackToPlay][i]=songsTitle[i];
				}
				counter=0;
				char songs[MAX_SONG_LENGTH]={0};
				printf_P(PSTR("Enter song: \n"));
				songs[0]=(char)UARTGetch(stdin);
				counter++;
				while(songs[counter-1]!='\n' && songs[counter-1]!='\r' && counter<62){
					songs[counter]=(char)UARTGetch(stdin);
					if(songs[counter]!=8){
						counter++;
					}else{
						counter--;
					}
				}
				printf(PSTR("\n"));
				StripEOL(songs,counter-1);
				StoreSong(song[trackToPlay],songs);
			}
		}else{
			printf_P(PSTR("That wasn't a choice\n"));
		}
	}
}
