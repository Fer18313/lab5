/*
 * File:   main.c
 * Author: Fernando Sandoval
 *
 * Created on August 12, 2021, 5:59 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 8000000 

// LIBRARIES
#include <xc.h>
#include <stdint.h>

// VARIABLES
uint8_t c;
uint8_t unit = 0;
uint8_t dzn = 0;
uint8_t var = 0;
uint8_t temp_0 = 0;
uint8_t dozns = 0;
uint8_t hundreds = 0;
uint8_t dozns_0 = 0;
uint8_t unit_0 = 0;
uint8_t temp = 0;
uint8_t upload = 1;
uint8_t download = 0;
uint8_t carry = 0;

//FUNCTION PROTOTYPES
void initCONFIG(void);

// MAIN CODE
void main(void) {
    initCONFIG();
    while(1){
        c = PORTD;
        hundreds = c/100; 
        dozns_0 = c%100;
        dozns = dozns_0/10;
        unit = c%10;
        if (PIR1bits.TXIF){
                    __delay_ms(20);
                    TXREG = hundreds + 48; 
                    __delay_ms(20);
                    TXREG = dozns + 48; 
                    __delay_ms(20);
                    TXREG = unit + 48;
                    __delay_ms(20);
                    TXREG = 0x0D;
                    __delay_ms(20);
            }
        PIR1bits.TXIF = 0; 
        }
    return;
}

void __interrupt() isr(void){
    if(INTCONbits.RBIF){
        if(PORTBbits.RB0 ==0){
            PORTD++;
        }
        if(PORTBbits.RB1==0){
            PORTD--;
        }
        INTCONbits.RBIF=0;
    }
    if(PIR1bits.RCIF==1){
        if(RCREG==0x0D){
            PORTA=var;
            temp =0;
            var=0;
            upload=1;
            download=0;
            carry=0;
        }
   if (RCREG !=  0x0D){
        temp_0 = RCREG;
            if(temp_0==48){
                temp = 0;
            }else if(temp_0==49){
                temp = 1;
            }else if(temp_0==50){
                temp = 2;
            }else if(temp_0==51){
                temp = 3;
            }else if(temp_0==52){
                temp = 4;
            }else if(temp_0==53){
                temp = 5;
            }else if(temp_0==54){
                temp = 6;
            }else if(temp_0==55){
                temp = 7;
            }else if(temp_0==56){
                temp = 8;
            }else if(temp_0==57){
                temp = 9;
            }
        if (upload == 1){
            var = temp;
            unit_0 = temp;
            upload = 0;
            download = 1;
        }
        else if (download == 1){
            var = (unit_0*10)+temp;
            dzn = temp;
            download = 0;
            carry = 1;
        }
        else if (carry == 1){
            var = (unit_0*100)+(dzn*10)+temp;
            download = 0;
            carry = 1;
        }     
        }}
}


void initCONFIG(void){
    OSCCONbits.IRCF2 = 1; 
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    ANSELH = 0;
    ANSEL = 0;
    TRISA = 0;
    TRISB = 0b00000011; 
    TRISC = 0x80;
    PORTA = 0; 
    PORTB = 0b00000011; 
    PORTC = 0;
    PORTD = 0;
    TRISD = 0b00000000;
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    INTCONbits.GIE = 1;  
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.RBIE = 1; 
    IOCB = 0b00000011;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;  
    TXSTAbits.TXEN = 1;
    INTCONbits.RBIF = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1; 
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 207;
    SPBRGH = 0;
    return;
}