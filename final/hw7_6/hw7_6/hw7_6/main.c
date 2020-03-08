/*
 * hw7_6.c
 *
 * Created: 7/3/2563 0:59:58
 * Author : lenovo
 */ 
#define  F_CPU 8000000UL
#define INC 0
#define DEC 1
#define NOTHING 2
#include <util/delay.h>
#include <avr/io.h>

unsigned char tb7seg[] = {0b00111111, 0b00000110,0b01011011, 0b01001111,
	0b01100110, 0b01101101,0b01111101, 0b00000111,
	0b01111111, 0b01101111,0b01110111, 0b01111100,
	0b00111001, 0b01011110,0b01111001, 0b01110001}; //declare as global variable
	
void initDirection(void)
{
	DDRC = 0x3f;
	DDRD = 0xf9;
}	
void dispLed(unsigned char a)
{
	unsigned char tmp;
	tmp = tb7seg[a];
	PORTC = tmp;
	tmp = tmp >> 6;
	tmp &= 0x01;
	if (tmp)
		PORTD |= 0x01;
	else
		PORTD &= 0xfe;
}
/*------logic status---- as below
sw: 12   | details
	-----------------
	00 --> press both
	01 --> press sw2
	10 --> press sw1
	11 --> press neither
*/
unsigned char readSW(void)
{
	unsigned char sw,returnTocaller;
	sw = PIND >> 1;
	sw &= 0x03; //0000 0011 ===> used only 2 bits
	if(sw != 3)
	{
		if (sw == 1) //if press only sw2
		{
			returnTocaller = DEC; //store status that press sw2 to decrement
		}
		else if(sw == 2) //if press only sw1
		{
			returnTocaller = INC; //store status that press sw2 to increment
		}
		else //in case of press neither
			returnTocaller = NOTHING;
		do
		{
			sw = PIND >> 1; //shift right 1
			sw &= 0x03; //0000 0011 use only 2 bits
		} while (sw != 3);
	}
	else
		return NOTHING;
	return NOTHING;	
}

void counterUpper(unsigned char rec)
{
	
}

void counterLower(unsigned char rec)
{
	
}

int main(void)
{
	initDirection()
	unsigned char sw;
	while(1)
	{
		sw = readSW();
				
	}
}