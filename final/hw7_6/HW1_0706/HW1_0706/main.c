/*
 * HW1_0706.c
 *
 * Created: 3/8/2020 7:22:57 AM
 * Author : theethawat
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

unsinged char bin_to_7seg(unsigned char input){
	static char lookuptable[] = {
		0x3F,
		0x06,
		0x5B,
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x07,
		0x7F,
		0x6F,
		0x77,
		0x7C,
		0x39,
		0x5E,
		0x79,
		0x71
	};
	return lookuptable[input];
}

unsigned char checkIfReset(unsigned char value){
	unsigned char check_button;
	check_button = PIND;
	check_button &= 0b00000100;
	check_button = check_button >> 2;
	if(check_button == 0){
		check_button = PIND;
		check_button &= 0b00000100;
		check_button = check_button >> 2;
		if(check_button == 0){
			return 0;
		}
		else{
			return value;
		}
	}
}
unsigned char runCounter(unsigned char number,unsigned char modeStatus){
	// Mode Up
	if(modeStatus == 0){
		if(number < 16){
			number ++;
			return number;
		}
		else{
			return 0;
		}
	}
	else if(modeStatus ==1){
		if(number > 0){
			number --;
			return number;
		}
		else{
			return 15;
		}
	}
}

char checkToggle(unsigned char modeStatus){
	unsigned char swInput;
	swInput = PIND;
	swInput = swInput & 0b00000010;
	swInput = swInput >> 1;
	if (swInput == 0)
	{
		//bounce delay
		_delay_ms(10);
		swInput = PIND;
		swInput &= 0b00000010;
		swInput = swInput >> 1;
		if(swInput == 0){
			//Toggle
			if (modeStatus == 1)
			{
				return 0;
			} 
			else
			{
				return 1;
			}

}

int main(void)
{
    /* Replace with your application code */
	// 0 is input 1 is output
	
	//PORTC AS all Output
	DDRC = 0xFF;
	
	//PORTD 0 is Output 1,2 as Input
	DDRD =  0x01; //0b00000001
    unsigned char out_value = 1;
	unsigned char out_led,temp,modeStatus;
	modeStatus = 0;
	while (1) 
    {
		temp = 0x01;
		out_led = bin_to_7seg(out_value);
		PORTC = out_led;
		PORTD = temp;
		_delay_ms(1000);
		out_value  = runCounter(out_value,modeStatus);
		modeStatus = checkToggle(modeStatus);
		out_value = checkIfReset(out_value);
    }
}
