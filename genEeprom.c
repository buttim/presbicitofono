//!gcc -Wall -o "%name%.exe" "%file%"
//!genEeprom
#include <stdio.h>
#include <stdlib.h>
#include "pitches.h"
#include "brahmsLullaby.h"
#include "elephantWalk.h"
#include "FeroVecio.h"
#include "furElise.h"
#include "godfather.h"
#include "odeToJoy.h"
#include "monferrina.h"
#include "takeOnMe.h"
#include "starWars.h"
#include "princeIgor.h"
#include "pinkPanther.h"

#define FOSC	11059200U

/*******************************************
EEPROM format (words)
	- number of songs
	- table of offset for each song
	- pairs note/duration for the songs

note=0 means rest
duration=0 means end
********************************************/

#define DURATA 15000
	
const short *songs[]={
	monferrina,
	feroVecio,
	//following melodies by Robson Couto (https://github.com/robsoncouto/arduino-songs)
	godfather,
	princeIgor,
	odeToJoy,
	furElise,
	elephantWalk,
	pinkPanther,
	brahmsLullaby,
	takeOnMe
};

double durata[sizeof songs/sizeof(*songs)];
int quanteNote[sizeof songs/sizeof(*songs)];

unsigned short timerVal(int freq) {
	return 0x10000-(FOSC/2/freq);
}

int main(int argc,char *argv[]) {
	const short *p;
	unsigned short t=0;
	FILE *f=fopen("eeprom.bin","wb");
	
	if (f==NULL) {
		fprintf(stderr,"cannot write 'eeprom.bin'\n");
		return 1;
	}
	t=sizeof songs/sizeof(*songs);
	fwrite(&t,sizeof t,1,f);
	t=sizeof(short)+2*sizeof songs/sizeof(*songs);
	for (int j=0;j<sizeof songs/sizeof(*songs);j++)  {
		fwrite(&t,sizeof t,1,f);
		for (p=songs[j];p[1]!=0;p+=2) {
			quanteNote[j]++;
			t+=4;
			float d=1./abs(p[1]);
			if (p[1]<0) d=d*3/2;
			durata[j]+=d;
		}
		t+=4;
	}
	
	for (int j=0;j<sizeof songs/sizeof(*songs);j++) {
		int tempo=(DURATA-quanteNote[j]*50)/durata[j];
		for (p=songs[j];p[1]!=0;p+=2) {
			t=0;
			if (*p!=0) 
				t=timerVal(p[0]);
			fwrite(&t,sizeof t,1,f);
			t=abs(p[1]);
			t=tempo/t;
			if (p[1]<0) t+=t/2;
			fwrite(&t,sizeof t,1,f);
		}
		t=0;
		fwrite(&t,sizeof(short),1,f);
		fwrite(&t,sizeof(short),1,f);
	}
	printf("%d bytes written to 'eeprom.bin'\n",(int)ftell(f));
	fclose(f);
	return 0;
}
