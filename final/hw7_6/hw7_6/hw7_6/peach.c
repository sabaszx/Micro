#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char LOOKUPTB[] = { 0b00111111,
		0b00000110,
		0b01011011,
		0b01001111,
		0b01100110,
		0b01101101,
		0b01111101,
		0b00000111,
		0b01111111,
		0b01101111,
		0b01110111,
		0b01111100,
		0b00111001,
		0b01011110,
		0b01111001,
	0b01110001 };
	/* Replace with your application code */
	
	unsigned char sw1, sw2, result, display, state;
	DDRC = 0xff;
	DDRD = 0x01;
	result = 0;
	state = 0;
	while (1)
	{
		sw1 = PIND;
		sw1 &= 0x02;
		sw2 = PIND;
		sw2 &= 0x04;
		
		if (sw2 == 0) {
			state = ~state;
			_delay_ms(500);
		}
		if(state == 0){
			if(result == 15){
				result =0;
			}
			else{
				result++;
			}
		}
		else{
			if(result == 0){
				result=15;
			}
			else{
				result--;
			}
		}
		if (sw1 == 0){
			result = 0;
		}
		
		display = LOOKUPTB[result];
		PORTC = display;
		display &=0x40;
		display = display >>6;
		PORTD = display;
		_delay_ms(500);
	}
}

