/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F67J60
        Version           :  2.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB             :  MPLAB X 5.20

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LEDA aliases
#define LEDA_TRIS                 TRISAbits.TRISA0
#define LEDA_LAT                  LATAbits.LATA0
#define LEDA_PORT                 PORTAbits.RA0
#define LEDA_ANS                  anselRA0bits.anselRA0
#define LEDA_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LEDA_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LEDA_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LEDA_GetValue()           PORTAbits.RA0
#define LEDA_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LEDA_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LEDA_SetAnalogMode()      do { anselRA0bits.anselRA0 = 1; } while(0)
#define LEDA_SetDigitalMode()     do { anselRA0bits.anselRA0 = 0; } while(0)

// get/set LEDB aliases
#define LEDB_TRIS                 TRISAbits.TRISA1
#define LEDB_LAT                  LATAbits.LATA1
#define LEDB_PORT                 PORTAbits.RA1
#define LEDB_ANS                  anselRA1bits.anselRA1
#define LEDB_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define LEDB_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define LEDB_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define LEDB_GetValue()           PORTAbits.RA1
#define LEDB_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define LEDB_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define LEDB_SetAnalogMode()      do { anselRA1bits.anselRA1 = 1; } while(0)
#define LEDB_SetDigitalMode()     do { anselRA1bits.anselRA1 = 0; } while(0)

// get/set READER_B_DATA aliases
#define READER_B_DATA_TRIS                 TRISBbits.TRISB0
#define READER_B_DATA_LAT                  LATBbits.LATB0
#define READER_B_DATA_PORT                 PORTBbits.RB0
#define READER_B_DATA_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define READER_B_DATA_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define READER_B_DATA_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define READER_B_DATA_GetValue()           PORTBbits.RB0
#define READER_B_DATA_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define READER_B_DATA_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)

// get/set MSG aliases
#define MSG_TRIS                 TRISBbits.TRISB3
#define MSG_LAT                  LATBbits.LATB3
#define MSG_PORT                 PORTBbits.RB3
#define MSG_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define MSG_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define MSG_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define MSG_GetValue()           PORTBbits.RB3
#define MSG_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define MSG_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)

// get/set SPIFLASH_CS aliases
#define SPIFLASH_CS_TRIS                 TRISCbits.TRISC2
#define SPIFLASH_CS_LAT                  LATCbits.LATC2
#define SPIFLASH_CS_PORT                 PORTCbits.RC2
#define SPIFLASH_CS_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SPIFLASH_CS_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SPIFLASH_CS_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SPIFLASH_CS_GetValue()           PORTCbits.RC2
#define SPIFLASH_CS_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SPIFLASH_CS_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)

// get/set SCK1 aliases
#define SCK1_TRIS                 TRISCbits.TRISC3
#define SCK1_LAT                  LATCbits.LATC3
#define SCK1_PORT                 PORTCbits.RC3
#define SCK1_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SCK1_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SCK1_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SCK1_GetValue()           PORTCbits.RC3
#define SCK1_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SCK1_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)

// get/set SDI1 aliases
#define SDI1_TRIS                 TRISCbits.TRISC4
#define SDI1_LAT                  LATCbits.LATC4
#define SDI1_PORT                 PORTCbits.RC4
#define SDI1_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SDI1_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SDI1_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SDI1_GetValue()           PORTCbits.RC4
#define SDI1_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SDI1_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set SDO1 aliases
#define SDO1_TRIS                 TRISCbits.TRISC5
#define SDO1_LAT                  LATCbits.LATC5
#define SDO1_PORT                 PORTCbits.RC5
#define SDO1_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SDO1_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SDO1_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SDO1_GetValue()           PORTCbits.RC5
#define SDO1_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SDO1_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)

// get/set SCLc aliases
#define SCLc_TRIS                 TRISCbits.TRISC6
#define SCLc_LAT                  LATCbits.LATC6
#define SCLc_PORT                 PORTCbits.RC6
#define SCLc_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define SCLc_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define SCLc_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define SCLc_GetValue()           PORTCbits.RC6
#define SCLc_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define SCLc_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)

// get/set SDAc aliases
#define SDAc_TRIS                 TRISCbits.TRISC7
#define SDAc_LAT                  LATCbits.LATC7
#define SDAc_PORT                 PORTCbits.RC7
#define SDAc_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define SDAc_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define SDAc_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define SDAc_GetValue()           PORTCbits.RC7
#define SDAc_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define SDAc_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)

// get/set ROTATE_A2B aliases
#define ROTATE_A2B_TRIS                 TRISDbits.TRISD0
#define ROTATE_A2B_LAT                  LATDbits.LATD0
#define ROTATE_A2B_PORT                 PORTDbits.RD0
#define ROTATE_A2B_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define ROTATE_A2B_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define ROTATE_A2B_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define ROTATE_A2B_GetValue()           PORTDbits.RD0
#define ROTATE_A2B_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define ROTATE_A2B_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)

// get/set ROTATE_B2A aliases
#define ROTATE_B2A_TRIS                 TRISDbits.TRISD1
#define ROTATE_B2A_LAT                  LATDbits.LATD1
#define ROTATE_B2A_PORT                 PORTDbits.RD1
#define ROTATE_B2A_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define ROTATE_B2A_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define ROTATE_B2A_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define ROTATE_B2A_GetValue()           PORTDbits.RD1
#define ROTATE_B2A_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define ROTATE_B2A_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)

// get/set BUZZER aliases
#define BUZZER_TRIS                 TRISDbits.TRISD2
#define BUZZER_LAT                  LATDbits.LATD2
#define BUZZER_PORT                 PORTDbits.RD2
#define BUZZER_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define BUZZER_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define BUZZER_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define BUZZER_GetValue()           PORTDbits.RD2
#define BUZZER_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define BUZZER_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)

// get/set READER_A_DATA aliases
#define READER_A_DATA_TRIS                 TRISEbits.TRISE0
#define READER_A_DATA_LAT                  LATEbits.LATE0
#define READER_A_DATA_PORT                 PORTEbits.RE0
#define READER_A_DATA_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define READER_A_DATA_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define READER_A_DATA_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define READER_A_DATA_GetValue()           PORTEbits.RE0
#define READER_A_DATA_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define READER_A_DATA_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)

// get/set EXIT aliases
#define EXIT_TRIS                 TRISEbits.TRISE1
#define EXIT_LAT                  LATEbits.LATE1
#define EXIT_PORT                 PORTEbits.RE1
#define EXIT_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define EXIT_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define EXIT_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define EXIT_GetValue()           PORTEbits.RE1
#define EXIT_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define EXIT_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)

// get/set O1 aliases
#define O1_TRIS                 TRISEbits.TRISE2
#define O1_LAT                  LATEbits.LATE2
#define O1_PORT                 PORTEbits.RE2
#define O1_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define O1_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define O1_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define O1_GetValue()           PORTEbits.RE2
#define O1_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define O1_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)

// get/set O2 aliases
#define O2_TRIS                 TRISEbits.TRISE3
#define O2_LAT                  LATEbits.LATE3
#define O2_PORT                 PORTEbits.RE3
#define O2_SetHigh()            do { LATEbits.LATE3 = 1; } while(0)
#define O2_SetLow()             do { LATEbits.LATE3 = 0; } while(0)
#define O2_Toggle()             do { LATEbits.LATE3 = ~LATEbits.LATE3; } while(0)
#define O2_GetValue()           PORTEbits.RE3
#define O2_SetDigitalInput()    do { TRISEbits.TRISE3 = 1; } while(0)
#define O2_SetDigitalOutput()   do { TRISEbits.TRISE3 = 0; } while(0)

// get/set O3 aliases
#define O3_TRIS                 TRISEbits.TRISE4
#define O3_LAT                  LATEbits.LATE4
#define O3_PORT                 PORTEbits.RE4
#define O3_SetHigh()            do { LATEbits.LATE4 = 1; } while(0)
#define O3_SetLow()             do { LATEbits.LATE4 = 0; } while(0)
#define O3_Toggle()             do { LATEbits.LATE4 = ~LATEbits.LATE4; } while(0)
#define O3_GetValue()           PORTEbits.RE4
#define O3_SetDigitalInput()    do { TRISEbits.TRISE4 = 1; } while(0)
#define O3_SetDigitalOutput()   do { TRISEbits.TRISE4 = 0; } while(0)

// get/set AUX_OUT aliases
#define AUX_OUT_TRIS                 TRISEbits.TRISE5
#define AUX_OUT_LAT                  LATEbits.LATE5
#define AUX_OUT_PORT                 PORTEbits.RE5
#define AUX_OUT_SetHigh()            do { LATEbits.LATE5 = 1; } while(0)
#define AUX_OUT_SetLow()             do { LATEbits.LATE5 = 0; } while(0)
#define AUX_OUT_Toggle()             do { LATEbits.LATE5 = ~LATEbits.LATE5; } while(0)
#define AUX_OUT_GetValue()           PORTEbits.RE5
#define AUX_OUT_SetDigitalInput()    do { TRISEbits.TRISE5 = 1; } while(0)
#define AUX_OUT_SetDigitalOutput()   do { TRISEbits.TRISE5 = 0; } while(0)

// get/set ALARM aliases
#define ALARM_TRIS                 TRISGbits.TRISG4
#define ALARM_LAT                  LATGbits.LATG4
#define ALARM_PORT                 PORTGbits.RG4
#define ALARM_SetHigh()            do { LATGbits.LATG4 = 1; } while(0)
#define ALARM_SetLow()             do { LATGbits.LATG4 = 0; } while(0)
#define ALARM_Toggle()             do { LATGbits.LATG4 = ~LATGbits.LATG4; } while(0)
#define ALARM_GetValue()           PORTGbits.RG4
#define ALARM_SetDigitalInput()    do { TRISGbits.TRISG4 = 1; } while(0)
#define ALARM_SetDigitalOutput()   do { TRISGbits.TRISG4 = 0; } while(0)


/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/