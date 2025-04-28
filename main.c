#include <at89c5131.h>
#include "lcd.h"																				//Driver for interfacing lcd 
#include "mcp3008.h"																		//Driver for interfacing ADC ic MCP3008


char adc_ip_data_ascii[6]={0,0,0,0,0,'\0'};							//string array for saving ascii of input sample
char adc_ip_data_ascii1[6]={0,0,0,0,0,'\0'};	
code unsigned char display_msg1[]="Volt.: ";						//Display msg on 1st line of lcd
code unsigned char display_msg2[]=" mV";//Display msg on 2nd line of lcd
unsigned int adc_data_prev = 0;
unsigned int count = 0;
unsigned int arr[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned int i = 0;
unsigned int flag = 0;

void main(void)
{
	unsigned int adc_data_curr=0,temp_data=0,adc_data=0,highest=0;
	unsigned int x=0,y=0;
	
	spi_init();																					
	adc_init();
  lcd_init();
	msdelay(100);
	
	while(1){
	  x = adc(4);
	  adc_data_curr = (int) (x*4.8875); 		//Converting received 10 bits value to milli volt (3.3*1000*i/p /1023)
		if(adc_data_curr > adc_data_prev){
			adc_data = adc_data_curr - adc_data_prev;
		}
		else if(adc_data_prev > adc_data_prev){
			adc_data = adc_data_prev - adc_data_curr;
		}
		
	  int_to_string(adc_data, adc_ip_data_ascii);
		lcd_cmd(0x80);
		lcd_write_string("Volt: ");
	  lcd_write_string(adc_ip_data_ascii);
		adc_data_prev = adc_data_curr;
		if(highest<adc_data){
			  if(count<4){
					count++;
				}
			  if(adc_data-highest<=0.1*adc_data && count==3 && i<10){
					arr[i++] = adc_data;
				}
				highest = adc_data;
			  count+=1;
		}
		else if(highest>adc_data){
			if(highest - adc_data<=0.1*highest && i<10){
				arr[i++] = adc_data;
			}

		}
		
		lcd_cmd(0xc0);
		int_to_string(highest, adc_ip_data_ascii);
		lcd_write_string("Amplitude: ");
		lcd_write_string(adc_ip_data_ascii);
		if(i == 10){
			count = (arr[0] + arr[1] + arr[2] + arr[3] + arr[4] + arr[5] + arr[6] + arr[7] + arr[8] + arr[9] + arr[10])/10;
			lcd_cmd(0x80);
			int_to_string(count, adc_ip_data_ascii);
			lcd_write_string("Final: ");
			lcd_write_string(adc_ip_data_ascii);
			while(1);
		}
	  fiveusdelay(10);
	}
}