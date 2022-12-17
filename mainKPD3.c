#include "STD_TYPES.h"

#include "LCD_interface.h"

#include "DIO_interface.h"

#include "KPD_interface.h"

#include <avr/delay.h>

u8 CompareArray(u8 Local_u8A1[], u8 Local_u8A2[]);
void DashBoardScreen1(void);
void DashBoardScreen2(void);


void ResetCode(void);
#define RedLed 				DIO_u8_PORTB,DIO_u8_PIN0
#define GreenLed 			DIO_u8_PORTB,DIO_u8_PIN1
#define MCW 				DIO_u8_PORTB,DIO_u8_PIN2
#define MCCW 				DIO_u8_PORTB,DIO_u8_PIN3

u8 Yindex = 12;
u8 Xindex = 0;
u8 Local_u8Counter = 0;
u8 Local_u8Counter2 = 0;
u8 Global_u8Res = 2;
u8 Global_u8Pass[4] = {'1','2','3','4'}; // pass of the user
u8 Global_u8EnteredPass[4]; // Entered pass by the user



int main (void)
{
	DIO_u8Init();
	LCD_voidInit();

	u8 Local_u8KeyVal;
	u8 Local_u8Flag = 0;
	LCD_voidWriteString("Password : "); // write "Password" on LCD
	Xindex = 1;
	Yindex = 12;

	while(1)
	{
		KPD_u8GetKeyValue(&Local_u8KeyVal); // get key val
		if (Local_u8Flag == 1)
		{
			DashBoardScreen1();
			_delay_ms(30);
		}
		else if (Local_u8Flag == 2)
		{
			DashBoardScreen2();
			_delay_ms(30);
		}
		if (Local_u8KeyVal != KPD_u8_KEY_NOT_PRESSED)
		{
			if (Local_u8Flag == 1)
			{
				Local_u8Counter = 0;
				if (Local_u8KeyVal == '1')
				{
					DashBoardScreen2();
					_delay_ms(30);
					Local_u8Flag = 2;
				}
				else if (Local_u8KeyVal == '2')
				{
					ResetCode();
					Local_u8Flag = 0;
				}
				else
				{
					LCD_voidClear();
					LCD_u8GoToXY(0,0);
					LCD_voidWriteString("Wrong input");
					_delay_ms(2000);
					ResetCode();
					Local_u8Flag = 0;
				}
			}
				else if (Local_u8Flag == 2)
				{
					Local_u8Counter = 0;
					if (Local_u8KeyVal == '1')
					{
						DIO_u8SetPinValue(MCW,DIO_u8_HIGH);
						DIO_u8SetPinValue(MCCW,DIO_u8_LOW);
					}
					else if (Local_u8KeyVal == '2')
					{
						DIO_u8SetPinValue(MCW,DIO_u8_LOW);
						DIO_u8SetPinValue(MCCW,DIO_u8_HIGH);
					}
					else if (Local_u8KeyVal == '3')
					{
						DIO_u8SetPinValue(MCW,DIO_u8_HIGH);
						DIO_u8SetPinValue(MCCW,DIO_u8_HIGH);
					}
					else if (Local_u8KeyVal == '4')
					{
						DashBoardScreen1();
						_delay_ms(1000);
						Local_u8Flag = 1;
					}
				}
				else if (Local_u8Flag == 0)
				{
					LCD_u8GoToXY(Xindex,Yindex); // go to the end of the lcd -4
					Yindex++;
					LCD_voidWriteChar('*'); // make the password look like astriscs
					Global_u8EnteredPass[Local_u8Counter] = Local_u8KeyVal; // save the entered pass in an array
					Local_u8Counter++; // counter to monitor the entered value
					if (Yindex > 15 || Local_u8Counter > 3) // if the user exceeded the text box
					{
						Local_u8Counter = 0; // reset the counter
						Yindex = 1;
						Xindex = 12; // reset the cursor place
						_delay_ms(1000);
						LCD_voidClear();
						Global_u8Res = CompareArray(Global_u8EnteredPass,Global_u8Pass); // call the function to compare the arrays
					}
				}
		}
		else if (Global_u8Res == 0) // if the comparison returned 0
		{
			LCD_voidClear();
			LCD_voidWriteString("Wrong Pass"); // write wrong pass
			DIO_u8SetPinValue(RedLed,DIO_u8_HIGH); // turn red led on
			_delay_ms(2000);
			ResetCode();
			Global_u8Res = 2; // random value for Result as a resetting value
			Local_u8Counter2 ++; // count up by 1
		}
		else if (Local_u8Counter2 > 2) // if the user exceeded 3 trials
		{
			Local_u8Counter2 = 0; // reset the wrong trials' counter
			LCD_voidClear();
			LCD_voidWriteString("Access Denied"); // write access denided on the LCD
			DIO_u8SetPinValue(RedLed,DIO_u8_HIGH); // turn red led on
			_delay_ms(2000);
			ResetCode();// reset the code
		}
		else if (Global_u8Res == 1) // if the user entered the right value
		{
			LCD_u8GoToXY(0,0); // go to 0,0
			LCD_voidWriteString("Welcome To"); // write welcome Mohamed
			LCD_u8GoToXY(1,0);
			LCD_voidWriteString("Motor DashBoard");
			DIO_u8SetPinValue(GreenLed,DIO_u8_HIGH); // turn green led on
			_delay_ms(3000);
			Global_u8Res = 2;
			Local_u8Flag = 1;
		}
	}
	return 0;
}

// function for comparing arrays
u8 CompareArray(u8 Local_u8A1[], u8 Local_u8A2[])
{
	u8 Local_u8Res = 0;
	for (u8 Local_u8Counter = 0; Local_u8Counter < 4; Local_u8Counter ++)
	{
		if (Local_u8A1[Local_u8Counter] == Local_u8A2[Local_u8Counter])
		{
			Local_u8Res = 1; // return 1 if the elements are equal
		}
		else
		{
			Local_u8Res = 0; // return 0 if the elements are not equal
			break;
		}
	}

	return Local_u8Res;
}
// function for resetting the code
void ResetCode(void)
{

	LCD_voidClear();
	LCD_u8GoToXY(0,0);
	LCD_voidWriteString("Password : "); // write "Password" on LCD
	DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN0,DIO_u8_LOW);
	DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN1,DIO_u8_LOW);
	Xindex = 1;
	Yindex = 12;
}

void DashBoardScreen1(void)
{
	LCD_voidClear();
	LCD_u8GoToXY(0,0);
	LCD_voidWriteString("1- Dc Motor");
	LCD_u8GoToXY(1,0);
	LCD_voidWriteString("2- Exit");
	_delay_ms(1000);
}

void DashBoardScreen2(void)
{
	LCD_voidClear();
	LCD_u8GoToXY(0,0);
	LCD_voidWriteString("1- CW");
	LCD_u8GoToXY(1,0);
	LCD_voidWriteString("2- CCW");
	LCD_u8GoToXY(0,7);
	LCD_voidWriteString("3- Stop");
	LCD_u8GoToXY(1,7);
	LCD_voidWriteString("4- Return");
	_delay_ms(1000);
}


