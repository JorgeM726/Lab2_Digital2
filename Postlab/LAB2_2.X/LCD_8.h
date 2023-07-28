

#ifndef LCD_H
#define	LCD_H

#ifndef _XTAL_FREQ
#endif

#ifndef EN
#define EN RC5
#endif

#ifndef RS
#define RS RC4
#endif

#ifndef D0
#define D0 RD0
#endif

#ifndef D1
#define D1 RD1
#endif

#ifndef D2
#define D2 RD2
#endif

#ifndef D3
#define D3 RD3
#endif

#ifndef D4
#define D4 RD4
#endif

#ifndef D5
#define D5 RD5
#endif

#ifndef D6
#define D6 RD6
#endif

#ifndef D7
#define D7 RD7
#endif

#include <xc.h>   


void Lcd_Cmd(char a);

void Lcd_Clear(void);

void Lcd_Set(char a, char b);

void Lcd_Init(void);

void Lcd_Write(char a);

void Lcd_Write_Str(char *a);

void Lcd_ShiftR(void);

void Lcd_ShiftL(void);

#endif	

