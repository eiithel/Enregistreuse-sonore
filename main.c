/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>

#define USER_REG 0x90080000

#define DELS_MASK 0x0C
#define DEL3_MASK 0x08

//timer registers
#define TIMER1_CTL 0x01980000 //register CTL1
#define TIMER_PRD 0x01980004 // register PRD1
#define TIMER_CNT 0x01980008 // register CNT1
#define GPIO 0x01B00000

#define CTL_MASK 0x000000C0//HLD=1

//GPIO registers

#define GPEN_REG_GPIO 0x01B00000
#define GPDIR_REG_GPIO 0x01B00004
#define GPPOL_REG_GPIO 0x01B00000

//interrupt registers

//selector
#define MUXL 0x019C0004
#define MASK_GPIO4_SELECTOR 0x00000004


void configAndStartTimer1(void){

	*(unsigned int*)TIMER1_CTL &= ~0x00000080;//hld a zero

	*(unsigned int*)TIMER_PRD = 2812;

	*(unsigned int*)TIMER1_CTL |= 0x0301;//CP a 1 (Clock mode), func a 1 (TOUT = timer output pin)

	*(unsigned int*)TIMER1_CTL |= CTL_MASK;//hld et go a 1 demarrage timer

}

/*
 * Timer1 ==> INT15
 */
extern void enableInterrupts(void);
extern void timer1_int(void);
extern void c_int4(void);

void blink(){
	int i;
	*(unsigned int*)USER_REG |= DELS_MASK;//led2 et led3 on

	for(i=0;i<110000000;i++){
		//5secondes 220MHz*5
	}

	*(unsigned int*)USER_REG &= ~DEL3_MASK;//led3 eteinte

	for(i=0;i<110000000;i++){
	}
}

void changeSrcInterrupt(unsigned int mask){
	*(unsigned int*)MUXL |= mask;//par exemple 00100 pour MUXL[4:0] ==> 0x00000004
}

//routine d'interruption

interrupt void c_int4(void){

	blink();
}


int main(void) {

	int i;//faire les declarations de variable au debut des fonctions

	//autorisation generale des interruptions
	enableInterrupts();

	*(unsigned int*)USER_REG |= DELS_MASK;
	//changeSrcInterrupt(MASK_GPIO4_SELECTOR);
	//pas necessaire par defaut sur GPIO4

	*(unsigned int*)GPEN_REG_GPIO |= 0x00000010;
	*(unsigned int*)GPDIR_REG_GPIO |= 0xFFFFFFEF;
	*(unsigned int*)GPPOL_REG_GPIO |= 0x00000010;

	while(1){

		configAndStartTimer1();

	}

	return 0;
}
