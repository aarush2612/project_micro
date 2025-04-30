#include <at89c5131.h>
#include "lcd.h"																				//Driver for interfacing lcd 
#include "mcp3008.h"		//Driver for interfacing ADC ic MCP3008
#include <math.h>


char adc_ip_data_ascii[6]={0,0,0,0,0,'\0'};							//string array for saving ascii of input sample	

void main(void)
{
	unsigned int i=0,counter = 0;
	int s = 0, c = 0;
	unsigned int x=0;
	
	spi_init();																					
	adc_init();
  lcd_init();
	msdelay(1000);
	
	while(counter<400){
	  x = adc(4);
	  switch(counter%4) {
			case 0:
				s+=x;
			  counter++;
			  break;
			
			case 1:
				c+=x;
			  counter++;
			  break;
			
			case 2:
				s-=x;
			  counter++;
			  break;
			
			case 3:
				counter++;
				c-=x;
			  break;
		}
		//459.5us from x = adc(4) to c-=x;
		sixtyusdelay(159);
	}
	c = (int) (c*4.8875)/200;// 4.8875 is because of 5/1023
	s = (int) (s*4.8875)/200;
	//lcd_cmd(0x80);
	
	s = s*s+c*c;
	int_to_string(s, adc_ip_data_ascii);
	lcd_write_string("Amplitude: ");
	lcd_write_string(adc_ip_data_ascii);
	while(1);
}