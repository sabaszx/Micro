/*
 * HW1_0706.c
 *
 * Created: 3/8/2020 7:22:57 AM
 * Author : theethawat
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

char bin_to_7seg(uint8_t input);
uint8_t run_number(uint8_t number,unsigned char mode_updown);
uint8_t check_for_reset(uint8_t data_value);
char check_for_toggle(unsigned char mode_updown);


int main(void)
{
    /* Replace with your application code */
	// 0 is input 1 is output
	
	//PORTC AS all Output
	DDRC = 0xFF;
	
	//PORTD 0 is Output 1,2 as Input
	DDRD =  0x01; //0b00000001
    uint8_t out_value = 1;
	unsigned char out_led,temp,mode_updown;
	mode_updown = 0;
	while (1) 
    {
		temp = 0x01;
		out_led = bin_to_7seg(out_value);
		PORTC = out_led;
		PORTD = temp;
		_delay_ms(1000);
		out_value  = run_number(out_value,mode_updown);
		mode_updown = check_for_toggle(mode_updown);
		out_value = check_for_reset(out_value);
    }
}

char check_for_toggle(unsigned char mode_updown){
	unsigned char switch_input;
	switch_input = PIND;
	switch_input = switch_input & 0b00000010;
	switch_input = switch_input >> 1;
	if (switch_input == 0)
	{
		//bounce delay
		_delay_ms(10);
		switch_input = PIND;
		switch_input &= 0b00000010;
		switch_input = switch_input >> 1;
		if(switch_input == 0){
			//Toggle
			if (mode_updown == 1)
			{
				return 0;
			} 
			else
			{
				return 1;
			}

}

uint8_t check_for_reset(uint8_t data_value){
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
			return data_value;
			}
	}
}

uint8_t run_number(uint8_t number,unsigned char mode_updown){
	// Mode Up
	if(mode_updown == 0){
		if(number < 16){
			number ++;
			return number;
		}
		else{
			return 0;
		}
	}
	else if(mode_updown ==1){
		if(number > 0){
			number --;
			return number;
		}
		else{
			return 15;
		}
	}
}

char bin_to_7seg(uint8_t input){
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

