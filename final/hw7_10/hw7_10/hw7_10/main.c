#define F_CPU 8000000UL
#define inc 0
#define reset 1
#define nothing 2
#include <avr/io.h>
#include <util/delay.h>

enum my_digits{ones,tens,hundreds,thousands};

unsigned char TB7SEG[] = {
	0x3F,0x06,0x5B,0x4F,
	0x66,0x6D,0x7D,0x07,
	0x7F,0x6F,0x77,0x7C,
	0x39,0x5E,0x79,0x71 };

void disp7seg(unsigned char a){
	unsigned char tmp;
	tmp = TB7SEG[a];
	PORTC = tmp;
	tmp = tmp >>6;
	tmp &= 0x01;
	if (tmp)
		PORTD |= 0x01;
	else
		PORTD &= 0xfe;
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

unsigned char read_switch(){
	unsigned char sw;
	unsigned char mode = nothing ;
	sw = PIND >> 1;
	sw &= 0x03;
	if (sw != 3)	{
		_delay_ms(10);
		sw = PIND >> 1;
		sw &= 0x03;
		if(sw != 3){
			if(sw == 1){
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
void initPort(void)
{
	DDRC = 0x3f; //0x3f 0b00111111
 	DDRD = 0xf9; //0xf9 0b11111001
}
int main(void)
{
	signed int count = 0;
	unsigned int countForSec = 0;
	unsigned char tmp;
	enum my_digits state = ones;
	unsigned char sw;
	initPort();
	while (1) {
		_delay_ms(5);
		countForSec+=10;
		if (countForSec==40){
			countForSec=0;
			sw = read_switch();
			if (sw == inc){
				count++;
				if (count>10){
					count = 0;
				}
			}
			else if (sw == reset)
			count=0;
		}
		
		bias_transistor(state);
		switch(state){
			
			case ones:
				tmp = count%10;
				disp7seg(tmp);
				_delay_ms(10);
				state = tens;
				break;
			
			case tens:
				tmp = (count/10)%10;
				disp7seg(tmp);
				_delay_ms(10);
				state = hundreds;
				break;
			
			case hundreds:
				tmp = (count/100)%10;
				disp7seg(tmp);
				_delay_ms(10);
				state = thousands;
				break;
			
			case thousands:
				tmp = (count/1000)%10;
				disp7seg(tmp);
				_delay_ms(10);
				state = ones;
				break;			
		}
	}
}