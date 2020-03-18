
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define inc 0
#define dec 1
#define reset 2

enum my_digits{ones,tens,hundreds,thousands};
int16_t delay_count;
uint8_t mode = inc;
unsigned char TB7SEG[] = {
	0b00111111 , 0b00000110 , 0b01011011 , 0b01001111,
	0b01100110 , 0b01101101 , 0b01111101 , 0b00000111,
	0b01111111 , 0b01101111 , 0b01110111 , 0b01111100,
	0b00111001 , 0b01011110 , 0b01111001 , 0b01110001 };

void display_7seg(uint8_t a){
	unsigned char tmp;
	tmp = TB7SEG[a];
	PORTC = tmp;
	tmp = tmp >>6;
	tmp &= 0x01;
	if (tmp)
	PORTD |= 0b00000001;
	else
	PORTD &= 0b11111110;
	
}

void bias_transistor(enum my_digits a){
	unsigned char tmp1,tmp2;
	tmp1 = PORTD;
	tmp1 &=0x01;
	switch(a){
		case ones:tmp2 = 0x80;break;
		case tens:tmp2 = 0x40;break;
		case hundreds:tmp2 = 0x20;break;
		default:	tmp2 = 0x10;break;
	}
	if(tmp1)
		PORTD = tmp2 | tmp1;
	else
		PORTD = tmp2;
}

uint8_t read_switch(){
	uint8_t sw;
	sw = PIND >> 1;
	sw &= 0x03;
	if (sw != 3)	{
		_delay_ms(10);
		sw = PIND >> 1;
		sw &= 0x03;
		if(sw != 3){
			if(sw == 1){
				if (mode == inc)
				mode = dec;
				else
				mode = inc;
			}
			
			else if (sw == 2){
				mode = reset;
			}
			do {
				sw = PIND >> 1;
				sw &= 0x03;
			} while (sw != 3);
		}
		return mode;
	}
	else
		return mode;
	
	return mode;
}

int main(void)
{
    signed int count = 0;
	unsigned int count_for_1sec = 0;
	unsigned char tmp;
	enum my_digits state = ones;
	uint8_t sw;
	DDRC = 0b00111111;
	DDRD = 0b11111001;
    while (1) {
		_delay_ms(5);
		count_for_1sec+=10;
		if (count_for_1sec==40){
			count_for_1sec=0;
			sw = read_switch();
			if (sw == inc){
				count++;
				if (count>9999){
					count = 0;
				}
			}
			else if (sw == dec){
				count--;
				if (count < 0){
					count=9999;
				}
				
			}
			else if (sw == reset)
			count=0;
		}
		
		bias_transistor(state);
		switch(state){
			
		case ones:
		tmp = count%10;
		display_7seg(tmp);
		_delay_ms(10);
		state = tens;
		break;
		
		case tens:
		tmp = (count/10)%10;
		display_7seg(tmp);
		_delay_ms(10);
		state = hundreds;
		break; 
		
		case hundreds:
		tmp = (count/100)%10;
		display_7seg(tmp);
		_delay_ms(10);
		state = thousands;
		break;
		
		case thousands:
		tmp = (count/1000)%10;
		display_7seg(tmp);
		_delay_ms(10);
		state = ones;
		break;
		
		}
    }
}

