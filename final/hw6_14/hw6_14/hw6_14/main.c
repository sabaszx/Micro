/*
 * hw6_14.c
 *
 * Created: 6/3/2563 15:56:07
 * Author : Chayanon Sukkhat
 */ 
#define F_CPU 8000000
#define LED10 PORTC
#define LED01 PORTD
#include <util/delay.h>
#include <avr/io.h>

unsigned char bcdTo7seg(unsigned char disp){
	switch(disp)
	{
		case 0:return ~0b00111111;
		case 1:return ~0b00000110;
		case 2:return ~0b01011011;
		case 3:return ~0b01001111;
		case 4:return ~0b01100110;
		case 5:return ~0b01101101;
		case 6:return ~0b01111101;
		case 7:return ~0b00000111;
		case 8:return ~0b01111111;
		case 9:return ~0b01101111;
		default:return 0xff;
	}
	
}

int main(void)
{
	unsigned char c10, c01;
	unsigned char sw1,sw2,sw3;
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xff;
	DDRD = 0xff;
	c10 = 0;
	c01 = 0;
	while (1)
	{
		sw2 = PINB;
		sw3 = sw2 >> 7;
		sw2 &= 0x1f;
		sw1 = PINA;
		sw1 &= 0x1f;
		if(sw3)
		{
			//count upper
			c01++;
			if ((c01 > 9)|| (c01 > sw2))
			{
				c01 = 0;
				c10++;
				if((c10>9) || (c10 > sw1))
				{
					c10=0;
				}
			}
		}
		else
		{
			//count lower
			if(c01 <=0 ){
				//borrow
				if (c10 <= 0)
				{
					if(sw1 > 9){
						c10 = 9;
					}
					else
					{
						c10 = sw1;	
					}
					
					if (sw2 > 9)
					{
						c01 = 9;
					}
					else{
						c01 = sw2;
					}
				}
				else{
					c10--;
					if (c01 > 9)
					{
						c01 = 9;
					}
					else{
						c01 = sw2;
					}
					
				}
			
			}
			//not borrow
			else{
				c01--;
			}
		}
		_delay_ms(1000);
		LED01 = bcdTo7seg(c01);
		LED10 = bcdTo7seg(c10);
	}
}
