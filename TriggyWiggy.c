// Triggy Wiggy
//
// USB MIDI to gate/trig.
//
// targeted for RPi.
// 2019 Daniel Skaborn

#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>

#define MIDIDEVICE	"/dev/snd/midiC1D0"

int MIDIin_d;
//int MIDIout_d;

pthread_t MIDIrcv;
void *cmd_MIDI_rcv(void *arg);

int MIDIin(unsigned char *data) {
	return read(MIDIin_d, data, 1 );
}
void MIDIout(unsigned char outbuf) {
	write(MIDIout_d, &outbuf, 1);
	return;
}

unsigned char noteToPin(unsigned char note) {
	unsigned char pin = 0;
	switch(note) {
		case 38:
			pin = 1;
			break;
		case 40:
			pin = 2;
			break;
		case 37:
			pin = 3;
			break;
		case 48:
			pin = 4;
			break;
		case 47:
			pin = 5;
			break;
		case 43:
			pin = 6;
			break;
		case 51:
			pin = 7;
			break;
		case 49:
			pin = 8;
			break;
		case 46:
			pin = 9;
			break;
		case 42:
			pin = 10;
			break;
		case 44:
			pin = 11;
			break;
		case 83:
			pin = 12;
			break;
		case 36:
			pin = 13;
			break;
		case 57:
			pin = 14;
			break;
		default:
			pint = 0;
	}
	return pin;
}

void pinOff(unsigned char note) {
	unsigned char pin;
	pin = noteToPin(note);

// todo GPIO pin off...

	return;
}

void pinOn(unsigned char note) {
	unsigned char pin;
	pin = noteToPin(note);

// todo GPIO pin on...

	return;
}

void *cmd_MIDI_rcv(void *arg) {
	unsigned char d=0;
	unsigned char cc=0;
	int rcvstate=0;
	int tickMidiClock=0;
	int p;
	MIDIin_d  = open(MIDIDEVICE,O_RDONLY);



	while(1) {
		if (MIDIin(&d)){
		switch(rcvstate) {
			case 0:
				if ((d & 0xF0)==(0x90)) rcvstate = 1; // note off
				if ((d & 0xF0)==(0x80)) rcvstate = 2; // note on
//				if ((d & 0xF0)==(0xB0)) rcvstate = 4; //CC msg
				break;
			case 1:
				PinOff(d);
				rcvstate = 0;
				break;
			case 2:
				tempnote = d;
				rcvstate = 3;
				break;
			case 3:
				if (d==0)
					pinOff(tempnote);
				else
					pinOn(tempnote);
				rcvstate = 0;
				break;
/*			case 4: // midi CC control no
				cc = d;
				rcvstate = 5;
				break;
				
			case 5: // midi CC control data
				rcvstate = 0;
				break;*/
			default:
				rcvstate = 0;
				break;
			}
		}
	}
}

int main(void) {
	unsigned char rec_ch = 0;
	int i;

	printf("\nTriggyWiggy\n\n");

//	MIDIout_d = open(MIDIDEVICE,O_WRONLY);

	pthread_create(&MIDIrcv, NULL, cmd_MIDI_rcv, NULL);
	
	while(1) {
		sleep(1);
	}
}
