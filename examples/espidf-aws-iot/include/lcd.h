#ifndef LCD_H_
#define LCD_H_


#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"driver/gpio.h"
#include"sdkconfig.h"
#include<math.h>
#include<string.h>


/*

This code is belongs to 16x2 LCD interfacing with ESP32 by esp-idf programming using VS code.


LCD Pins		ESP32  GPIO Pins

VSS-------------------------GND
VDD-------------------------+5V
V0--------------------------GND
RS---------------------------5
RW---------------------------2
E----------------------------4
D0 --------------------------5
D1--------------------------16
D2--------------------------17
D3--------------------------21
D4---------------------------3
D5---------------------------1
D6--------------------------22
D7--------------------------23
*/


int lcd_pins[11] = {5,16,17,21,3,1,22,23,15,2,4};
void lcd_cmd(unsigned char cmd);
void lcd_init();
void lcd_decode(unsigned char info);
void lcd_data(unsigned char data);
void lcd_string(unsigned char *p);

void lcd_init()
{
	//initialize pins
	for(int i=0;i<11;i++)
	{
		gpio_pad_select_gpio(lcd_pins[i]);
		gpio_set_direction(lcd_pins[i],GPIO_MODE_OUTPUT);
	}
	
	lcd_cmd(0x38); // configure lcd in 8 bit mode
	lcd_cmd(0x01); // clear display screen
	lcd_cmd(0x0E); // display cursure on and display on
	lcd_cmd(0x80); // set cursure to 1'st line 1'st position
	
}

void lcd_decode(unsigned char info)
{
	unsigned char temp;
	for(int i=0;i<8;i++)
	{
		temp =pow(2,i);
		gpio_set_level(lcd_pins[i],(info&temp));
	}
}

void lcd_cmd(unsigned char cmd)
{
	lcd_decode(cmd);
	gpio_set_level(lcd_pins[8],0);
	gpio_set_level(lcd_pins[9],0);
	gpio_set_level(lcd_pins[10],1);
	vTaskDelay(10/portTICK_PERIOD_MS);
	gpio_set_level(lcd_pins[10],0);
	vTaskDelay(10/portTICK_PERIOD_MS);
}

void lcd_data(unsigned char data)
{
	lcd_decode(data);
	gpio_set_level(lcd_pins[8],1);
	gpio_set_level(lcd_pins[9],0);
	gpio_set_level(lcd_pins[10],1);
	vTaskDelay(10/portTICK_PERIOD_MS);
	gpio_set_level(lcd_pins[10],0);
	vTaskDelay(10/portTICK_PERIOD_MS);


}

void lcd_string(unsigned char *p)
{
	while(*p !='\0')
	{
		lcd_data(*p);
		p=p+1;
	}
}

#endif