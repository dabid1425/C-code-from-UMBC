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
#define USER_LINE_MAX 64
#define NUMBER_OF_SONGS 4
#define MAX_SONG_LENGTH 64
#define FREQ_A4_HZ 440
#define FREQ_B4_HZ 493.88
#define FREQ_C4_HZ 261.63
#define FREQ_D4_HZ 293.66
#define FREQ_E4_HZ 329.63
#define FREQ_F4_HZ 349.23
#define FREQ_G4_HZ 392
#define HALFPERIOD_G4_US 1276 //delay in microseconds
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
char songTitles [NUMBER_OF_SONGS][USER_LINE_MAX] ={"Hot cross buns","Hot potato pie","Twinkle twinkle","Three blind mice"};
const char mainMenu[4][32] PROGMEM = {"----Main Menu---\n","1. List Songs \n","2. Play Song\n","3. Create Song\n" };
const char playMenu[3][32] PROGMEM= {"----Play Song Menu---\n","1. Play by number\n","2. Search By Title\n"};    
void delayms(int seconds){
	for(int i = 0; i<seconds;i++)
		_delay_ms(1);
}
//packs the note in binary it checks which note it is and shifts the note over 5 and then adds the 

uint8_t PackNote(char letterASCII, uint8_t duration){
	uint8_t packedNote = 0;
	if(letterASCII=='A'||letterASCII=='a'){
		packedNote=(NOTE_A<<5)+duration;
	}else if(letterASCII=='B'||letterASCII=='b'){
		packedNote=(NOTE_B<<5)+duration;
	}else if(letterASCII=='C'||letterASCII=='c'){
		packedNote=(NOTE_C<<5)+duration;
	}else if(letterASCII=='D'||letterASCII=='d'){
		packedNote=(NOTE_D<<5)+duration;
	}else if(letterASCII=='E'||letterASCII=='e'){
		packedNote=(NOTE_E<<5)+duration;
	}else if(letterASCII=='F'||letterASCII=='f'){
		packedNote=(NOTE_F<<5)+duration;
	}else if(letterASCII=='G'||letterASCII=='g'){
		packedNote=(NOTE_G<<5)+duration;
	}else if(letterASCII=='R'||letterASCII=='r'){
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
	printf("*%c",letterASCII);
	printf("%d*",duration);
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

uint8_t DisplayMenu(){
	printf_P(mainMenu[0]);
	printf_P(mainMenu[1]);
	printf_P(mainMenu[2]);
	printf_P(mainMenu[3]);
	uint8_t number;
	number=UARTGetch(stdin);
	number -=48;
	printf(PSTR("\n"));
	return number;



}
void ListSongs(char songTitles[NUMBER_OF_SONGS][USER_LINE_MAX]){
	for(int i=0;i<4;i++){
		printf("%d",i+1);
		printf(". ");
		printf(songTitles[i]);
		printf(PSTR("\n"));
	}
}
uint8_t MatchScore(const char countQueryString [],const char database[]){
	uint8_t score=0;
	/*
	char newInput[USER_LINE_MAX]={0};
	char newData[USER_LINE_MAX]={0};
	char newDummy[USER_LINE_MAX]={0};
	int k=0;
	int saved=strlen(database);
	int b=0;
	while(k<saved){
		newDummy[b]=tolower(database[k]);
		if(newDummy[b]==32 || newDummy[b]==0){
			b=-1;
			int saveLength=strlen(newData);
			if(saveLength==0){
				strcat(newData,newDummy);
				}else{
				if(strstr(newData,newDummy)==NULL){
					strcat(newData,newDummy);
				}
			}
		}
		k++;
		b++;
	}
	k=0;
	b=0;
	saved=strlen(countQueryString);
	while(k<saved){
		newInput[b]=tolower(countQueryString[k]);
		if(newInput[b]==32 || newInput[b]==0){
			b=-1;
			if(strstr(newData,newInput)!=NULL){
				score++;
			}
		}
		k++;
		b++;
	}
	*/
	return score;
}
int main(void){
	uint8_t song[NUMBER_OF_SONGS][MAX_SONG_LENGTH] =
		{{(NOTE_B<<5)+2 ,(NOTE_A<<5) +2,(NOTE_G<<5 )+2 , (NOTE_R<<5)},{(NOTE_G<<5)+5,NOTE_R<<5,(NOTE_A<<5) +2},{(NOTE_C<<5)+2,NOTE_R<<5},{(NOTE_D<<5)+2,NOTE_R<<5}};
	DDRB = PORTB5_SPEAKER_MASK;//sets pin 5 to output
	PORTB = PORTB5_SPEAKER_MASK;
	stderr = stdout = stdin= &uart_stream;
	UARTInit();
	uint8_t trackToPlay;
	uint8_t choice;
	while(1){
		choice=DisplayMenu();
		if(choice==1){
			ListSongs(songTitles);
		}else if(choice==2){
			printf_P(playMenu[0]);
			printf_P(playMenu[1]);
			printf_P(playMenu[2]);
			uint8_t number;
			number=UARTGetch(stdin);
			number -=48;
			printf(PSTR("\n"));
			if(number==1){
				printf(PSTR("Which song \n"));
				ListSongs(songTitles);
				trackToPlay=UARTGetch(stdin);
				trackToPlay -=48;
				printf(PSTR("\n"));
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
				int counter=0;
				int score=0;
				int whichOne=-1;
				int oldScore=0;
				char songsNames[MAX_SONG_LENGTH]={0};
				printf_P(PSTR("Enter title of song: \n"));
				songsNames[0]=(char)UARTGetch(stdin);
				counter++;
				while(songsNames[counter-1]!='\n'&&songsNames[counter-1]!='\r'&&counter<64){
					songsNames[counter]=(char)UARTGetch(stdin);
					if(songsNames[counter]!=8){
						counter++;
					}
				}
				StripEOL(songsNames,counter-1);
				for(int i=0;i<4;i++){
					score=MatchScore(songsNames,songTitles[i]);
					if(oldScore<score){
						oldScore=score;
						whichOne=i;
					}
				}
				if(oldScore==-1){
					printf_P(PSTR("Song not found\n"));
				}else{
					printf(PSTR("\n"));
					PlaySong(song[whichOne]);
					printf(PSTR("\n"));
				}
			}
		}else if(choice==3){
			printf(PSTR("---Which song would you like to replace (Enter 1-4)---- \n"));
			ListSongs(songTitles);
			trackToPlay=UARTGetch(stdin);
			trackToPlay -=48;
			printf(PSTR("\n"));
			if(trackToPlay>4||trackToPlay<0){
				printf(PSTR("That was not a choice\n"));
			}else{
				trackToPlay=trackToPlay-1;
				int counter=0;
				char songsTitle[USER_LINE_MAX]={0};
				printf(PSTR("What is the title of the song?\n"));
				songsTitle[0]=(char)UARTGetch(stdin);
				counter++;
				while(songsTitle[counter-1]!='\n'&&songsTitle[counter-1]!='\r'&&counter<128){
					songsTitle[counter]=(char)UARTGetch(stdin);
					if(songsTitle[counter]!=8){
						counter++;
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
				while(songs[counter-1]!='\n'&&songs[counter-1]!='\r' && counter<62){
					songs[counter]=(char)UARTGetch(stdin);
					if(songs[counter]!=8){
						counter++;
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
