#ifndef _FUNK_H
#define _FUNK_H

extern void interface(void);
extern void test_lcd(void);
extern void LED_init(void); 
extern void TIMER1_init(void);
extern void LED_an(int x);
extern void LED_aus(int x);
extern void Lauflicht(void);
extern void Display_LED(void);
extern void Display_LED_an(int x);
extern void Display_LED_aus(int x);
extern  __irq void TIMER1_IRQHandler (void);
extern int LED[8];
extern int x;
extern int n;

#endif