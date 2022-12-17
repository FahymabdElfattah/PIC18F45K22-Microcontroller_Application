#include <xc.h>
#include <pic18f45k22.h> //PIC18F45K22
#include <stdio.h>
#include <stdlib.h>


#pragma config FOSC = HSMP, PLLCFG = OFF, PRICLKEN = ON, FCMEN = OFF, IESO = OFF     // INTIO67
#pragma config PWRTEN = OFF, BOREN = SBORDIS, BORV = 190
#pragma config WDTEN = OFF, WDTPS = 32768
#pragma config CCP2MX = PORTB3, PBADEN = OFF, CCP3MX = PORTB5, HFOFST = ON, T3CMX = PORTC0, P2BMX = PORTD2, MCLRE = EXTMCLR
#pragma config STVREN = ON, LVP = OFF, XINST = OFF, DEBUG = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF
#pragma config EBTRB = OFF

#define _XTAL_FREQ  8000000

void init_port();
void init_interrupt();
void init_INT0();
void init_INT1();
void init_INT2();
void main(void) {
    init_port();
    init_interrupt();
    init_INT0();
    init_INT1();
    init_INT2();
    while(1){
        LATAbits.LATA4 = 1 ;
        __delay_ms(1000);
        LATAbits.LATA4 = 0;
        __delay_ms(1000);
    }
    return ;
}
void init_port(){
    TRISBbits.RB0 = 1 ;
    TRISBbits.RB1 = 1 ;
    TRISBbits.RB2 = 1 ;
    
    TRISD = 0x00 ;
    LATD = 0 ;
    /*
    ANSELA = 0x00; // RA5, RA3, RA2, RA1 et RA0 en E/S numérique
    LATA = 0x00;  // Initialise les LATCH de données du PORTA
    PORTA = 0x00;  // Initialise les LATCH de données du PORTA
    TRISA = 0x0F; // RA<3:0> en entrée et RA<5:4> en sortie
     */
}
void init_interrupt(){
    ADCON1bits.PVCFG = 0b1111;
    RCONbits.IPEN = 1 ;
    INTCONbits.GIE = 1 ;
    INTCONbits.PEIE = 1 ;
}
void init_INT0(){
    INTCONbits.INT0E = 1 ;
    INTCONbits.INT0F = 0 ;
    INTCON2bits.INTEDG0 = 1;
}
void init_INT1(){
    INTCON3bits.INT1IP = 0;
    INTCON3bits.INT1E = 1;
    INTCON3bits.INT1IF = 0 ;
    INTCON2bits.INTEDG1 = 1 ;
}
void init_INT2(){
    INTCON3bits.INT2IP = 0;
    INTCON3bits.INT2E = 1;
    INTCON3bits.INT2IF = 0 ;
    INTCON2bits.INTEDG2 = 1 ;
}
void __interrupt(high_priority)ISR_H(){
    if(INTCONbits.INT0E == 1 && INTCONbits.INT0F == 1 ){
        LATDbits.LATD0 = 1 ;
        INTCONbits.INT0F = 0 ;
    }
}
void __interrupt(low_priority)ISR_L(){
    if(INTCON3bits.INT1E == 1 && INTCON3bits.INT1IF == 1){
        LATDbits.LATD1 = 1 ;
        INTCON3bits.INT1IF = 0 ;
    }
     if(INTCON3bits.INT2E == 1 && INTCON3bits.INT2IF == 1){
        LATDbits.LATD2 = 1 ;
        INTCON3bits.INT2IF = 0 ;
    }
}
/*
ANSELx: Registre de configuration du mode analogique ou numérique 0=>N 1=>A
TRISx: Configurer chaque broche d?un PORT x en entrée ou en sortie: 0=>O 1=>I
PORTx: Registre de lecture de l?état des broches du PORTx 
LATx: Registre de données utilisé uniquement pour les sorties
*/

