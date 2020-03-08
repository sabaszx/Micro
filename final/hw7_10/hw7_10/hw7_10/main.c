/*
 * hw7_10.c
 *
 * Created: 8/3/2563 17:44:34
 * Author : lenovo
 */ 



#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


unsigned int count=0;
unsigned char segmentDigitDisply = 0;

unsigned char lookupTable[10] = {
	0x3F, //0
	0x06, //1
	0x5B, //2
	0x4F, //3
	0x66, //4
	0x6D, //5
	0x7D, //6
	0x07, //7
	0x7F, //8
	0x67  //9
};

void initial_mcu(){
	//       76543210
	DDRD = 0b11110001;
	DDRC = 0xFF;

}

void incrsCount(){
	if(count >= 9999){
		count=0;
		}else{
		count++;
	}
}

void decsCount(){
	if(count ==0 ){
		count = 9999;
		}else{
		count--;
	}
}

void redisply7segment(){
	unsigned char valueToDisplay = 0;
	unsigned char digitToDisplay;
	switch (segmentDigitDisply)
	{
		case 0:
		valueToDisplay = count%10;
		digitToDisplay = 1<<(7-segmentDigitDisply); //0b10000000
		segmentDigitDisply=1;
		break;
		case 1:
		valueToDisplay = (count%100)/10;
		digitToDisplay = 1<<(7-segmentDigitDisply); //0b01000000
		segmentDigitDisply=2;
		break;
		case 2:
		valueToDisplay = (count%1000)/100;
		digitToDisplay = 1<<(7-segmentDigitDisply); //0b00100000
		segmentDigitDisply=3;
		break;
		case 3:
		valueToDisplay = (count%10000)/1000;
		digitToDisplay = 1<<(7-segmentDigitDisply); //0b00010000
		segmentDigitDisply=0;
		break;
		default:
		segmentDigitDisply=0;
		break;
	}
	
	valueToDisplay = lookupTable[valueToDisplay];
	PORTC = valueToDisplay;
	PORTD = ((valueToDisplay>>6) & 0x01) || (digitToDisplay & 0x11110000);
}

int main(){
	initial_mcu();
	unsigned char isSW1press;
	unsigned char isSW2press;
	unsigned char delayCount=0;
	unsigned char direction=0;  // 0 is incr 1 is dec
	unsigned char isDirectionChange=0; // 0 mean unchange 1 mean has change
	
	while (1)
	{
		isSW1press = PIND | ~(0b00000010);
		isSW2press = PIND | ~(0b00000100);

		if(!isSW1press && !isDirectionChange){
			direction = !direction;
		}
		
		if(!isSW2press){
			count = 0;
			redisply7segment();
		}

		redisply7segment();

		_delay_ms(10);
		delayCount++;
		
		if(delayCount>=100){
			if(direction == 0){
				incrsCount();
				}else{
				decsCount();
			}
			redisply7segment();
			delayCount=0;
		}

	}
	
}

