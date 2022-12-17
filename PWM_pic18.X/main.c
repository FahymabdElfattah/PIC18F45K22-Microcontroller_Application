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


#define Fosc  8000000UL
#define Freq 15000UL
#define _XTAL_FREQ  8000000UL

uint16_t c ;

void init_port();
void init_PWM(uint32_t Frequency);
void duty_PWM(uint8_t duty);
            
void main(void) {
    init_port();
    //init_PWM(300);
    //duty_PWM(50);
    TRISCbits.RC2 = 0;
    T2CONbits.T2CKPS = 0b00 ;
    T2CONbits.TMR2ON = 0 ;
    CCP1CONbits.CCP1M = 0b1100;
    PR2 =52;
    //PR2 =132;
    T2CONbits.TMR2ON = 1 ;
    
    //CCPR1L = 255 >> 2;
    //CCP1CONbits.DC1B = (0x03) & 255 ; 
    
    c = 105 ;
    CCPR1L = (0xFF) & (c >> 2);
    CCP1CONbits.DC1B = (0x03) & c ;
    while(1){
        __delay_ms(300);
        c = 105 ;
        CCP1CONbits.CCP1M = 0b1100;
        CCPR1L = (0xFF) & (c >> 2);
        __delay_ms(200);
        CCP1CONbits.CCP1M = 0b0000;
        LATCbits.LATC2 = 0 ;
        
       /* for(c = 0; c <= 533; c++){
            //c = 0 ; // PWM is OFF
            //c = 533 ;
            //c = 266 ;
            CCPR1L = (0xFF) & (c >> 2);
            CCP1CONbits.DC1B = (0x03) & c ;
            __delay_ms(5);
        } */
        
        /*
         LATAbits.LATA4 = 1 ;
        __delay_ms(1000);
        LATAbits.LATA4 = 0;
        __delay_ms(1000);
        */
        
    }
    return ;
}
void init_port(){
   // ANSELA = 0x00; // RA5, RA3, RA2, RA1 et RA0 en E/S numérique
    //LATA = 0x00;  // Initialise les LATCH de données du PORTA
    //PORTA = 0x00;  // Initialise les LATCH de données du PORTA
    //TRISA = 0x0F; // RA<3:0> en entrée et RA<5:4> en sortie
}
void init_PWM(uint32_t Frequency){
    TRISCbits.RC2 = 0;
    T2CONbits.T2CKPS = 0b00 ;
    T2CONbits.TMR2ON = 0 ;
    CCP1CONbits.CCP1M = 0b1100;
    PR2 =(uint8_t)((Fosc)/(Frequency*4*1)-1) ;
    T2CONbits.TMR2ON = 1 ;
}
void duty_PWM(uint8_t duty){ // 0% --> 100%
    uint16_t MAXduty = 0xFFFF & ((Fosc/Freq));
    uint16_t MINduty = 0 ;
    uint16_t i = 0xFFFF & (((MAXduty-MINduty)*duty)/(100-0));
    CCPR1L = (0xFF) & (i >> 2);
    CCP1CONbits.DC1B = (0x03) & i ;    
}


/*
ANSELx: Registre de configuration du mode analogique ou numérique 0=>N 1=>A
TRISx: Configurer chaque broche d?un PORT x en entrée ou en sortie: 0=>O 1=>I
PORTx: Registre de lecture de l?état des broches du PORTx 
LATx: Registre de données utilisé uniquement pour les sorties
*/

/*
 * Fosc = 8 MHz   Tosc = 1/Fosc
 * 
 * PWM period = [(PR2)+1].4.Tosc.(TMR2 Prescale value)
 * PWM frequency = Fosc /{[(PR2)+1].4.(TMR2 Prescale value)}
 * PR2 de 8 bits ==> (0 -> 255)
 * TMR2 Prescale value 1,2,...,128
 * ------------------------------------------------------------
 * PR2 = (FOSC / {4.(PWM frequency).(TMR2 Prescale value)})-1
 * ----------------------------------------------------------------------
 * PWM Duty cycle Freq = X(%).PWM frequency    X (0 -> 100)
 * ----------------------------------------------------------------------
 * (CCPRXL:CCPXCON)= FOSC / {(PWM Duty cycle Freq).(TMR2 Prescale value)}
 * -----------------------------------------------------------------------
 * CCPRXL:CCPXCON : Register of 10 bits !!!!!! ==>  0 -> 1023
 * 
 *    -> 100%
 * X  -> 50%
 *    -> 0%
 * --------------------------------------------------------------------------
 * Pulse Width = CCPRxL:CCPxCON<5:4>.TOSC.(TMRx Prescale Value)
 * --------------------------------------------------------------------------
 * Resolution of 10 bits  (CCPRXL:CCPXCON) = 2^n
 * n = log2((CCPRXL:CCPXCON))
*/
