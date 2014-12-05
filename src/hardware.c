#include "Common.h"
#include "hardware.h"

// There are specific files, radiohardware.h and
// radiohardware.c that specifically deal with the
// wireless radio
#include "radiohardware.h"

// For certain functions called in the configure... routines
#include "cm_ir.h"
#include "cm_radio.h"
#include "MRF24J40.h"

void configureAccelerometer() {

    AD1CON1bits.AD12B = 1;              // 12-bit one-channel higher acc. mode
    AD1CON2bits.VCFG = 0;               // AVdd and AVss as + and - ref
    AD1CON3bits.ADCS = 0;               // T_AD = T_CY (sounds about right probably)

    AD1PCFGL = AD1PCFGL & (~(1 << pADC_PIN_NUM));      // Disable digital function on pin 'analogport'; make it analog

    AD1CHS0bits.CH0SA = pADC_PIN_NUM;     // Sample on analog pin 'analogport'
    AD1CON2bits.CHPS = 0;               // Convert "channel 0"

    AD1CON1bits.FORM = 0;               // Read buffer in integer form

    AD1CON1bits.ASAM = 0;               // Don't auto sample, manually set the SAMP bit to sample
    AD1CON1bits.SSRC = 0b111;           // Auto convert (conversion is ended when the module decides)

    AD1CON1bits.ADON = 1;               // Enable module

    configureTimer1_1600();

}

void configureTimer1_1600() {
    T1CONbits.TON = 0;          // Disable Timer
    T1CONbits.TCS = 0;          // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;        // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11;     // Select 1:256 Prescaler
    TMR1 = 0x00;                // Clear timer register
    PR1 = 9;                    // Load the period value
    IPC0bits.T1IP = 0x01;       // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;          // Enable Timer1 interrupt
    T1CONbits.TON = 1;          // Start Timer
}

void configureTimer1_60() {
    T1CONbits.TON = 0;          // Disable Timer
    T1CONbits.TCS = 0;          // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;        // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11;     // Select 1:256 Prescaler
    TMR1 = 0x00;                // Clear timer register
    PR1 = 256;                    // Load the period value
    IPC0bits.T1IP = 0x01;       // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;          // Enable Timer1 interrupt
    T1CONbits.TON = 1;          // Start Timer
}

void configureTimer3_60() {
    T3CONbits.TON = 0;          // Disable Timer
    T3CONbits.TCS = 0;          // Select internal instruction cycle clock
    T3CONbits.TGATE = 0;        // Disable Gated Timer mode
    T3CONbits.TCKPS = 0b11;     // Select 1:256 Prescaler
    TMR3 = 0x00;                // Clear timer register
    PR3 = 256;                    // Load the period value
    IPC2bits.T3IP = 0x01;       // Set Timer1 Interrupt Priority Level
    IFS0bits.T3IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T3IE = 1;          // Enable Timer1 interrupt
    T3CONbits.TON = 1;          // Start Timer
}

void configureIRReceive() {

    T2CONbits.TON = 0;          // Disable Timer
    T2CONbits.TCKPS = 0b00;     // Select 1:1 Prescaler
    TMR2 = 0x00;                // Clear timer register
    T2CONbits.TON = 1;          // Start Timer

    pIRRX_PIN_TRIS = 1;                 // Set the chosen pin as input
    RPINR7bits.IC1R = pIRRX_PIN_NUM;    // Set input IC1 to the chosen pin

    IC1CONbits.ICM = 0b00;      // Disable Input Capture 1 module
    IC1CONbits.ICTMR = 1;       // Select Timer2 as the IC1 Time base
    IC1CONbits.ICI = 0b00;      // Interrupt on every capture event
    //IC1CONbits.ICM = 0b001;     // Re-enable / Generate capture event every edge
    IC1CONbits.ICM = 0b010;     // Every falling edge

    // Enable Capture Interrupt And Timer2
    IPC0bits.IC1IP = 1;         // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0;         // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1;         // Enable IC1 interrupt

    initIRReceive();

}

void configureIRSend() {

    pLED_PIN_TRIS = 0;
    pLED_PIN_PORT = 0;

//    T4CONbits.TON = 0;          // Disable Timer
//    T4CONbits.TCKPS = 0b00;     // Select 1:1 Prescaler
//    TMR4 = 0x00;                // Clear timer register
//    PR4 = 33;                 // Load the period value
//    IPC6bits.T4IP = 0x01;       // Set Timer3 Interrupt Priority Level
//    IFS1bits.T4IF = 0;          // Clear Timer3 Interrupt Flag
//    IEC1bits.T4IE = 0;          // Don't Enable Timer3 interrupt yet
//    T4CONbits.TON = 1;          // Start Timer


}

void configureLightMCU_SPI() {

    pLIGHTMCU_SDO_TRIS = 0;
    pLIGHTMCU_SCK_TRIS = 0;

    pLIGHTMCU_SDO_PORT = 0;
    pLIGHTMCU_SCK_PORT = 0;

    //configureTimer3_60();

}

int configureRadio(int short_addr, long long long_addr) {

    RadioHW_Init();

    int radiostatus = RadioInit();
    RadioInitP2P();

    // Set the short address, long address, and pan ID
    // (I think they can be pretty much anything except the reserved
    // MAC addresses (0's and F's)
    RadioSetAddress(short_addr, long_addr, 0x0F00);

    // Between 11 and 26
    RadioSetChannel(11);

    return radiostatus;

}

void configureUART1() {

    pUART_TX_RPOR = 0b00011;   // Assign output U1TX to pin RP8
    RPINR18bits.U1RXR = pUART_RX_NUM;      // Assign input U1RX to pin RP6


    U1MODE = 0x00;

    // U1BRG = (FCY / (16*baud)) - 1; baud = 9600

    U1BRG = 23;

    // Enable UART1 module, and enable transmission on it

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void configureAudio() {

    pAUDIO_CLK_TRIS = 0;
    pAUDIO_DATA_TRIS = 0;
    pAUDIO_RESET_TRIS = 0;

}

void configureCloudLighting() {

    pCLIGHTS_XLAT_TRIS = 0;
    pCLIGHTS_SCK_TRIS = 0;
    pCLIGHTS_SDO_TRIS = 0;
    pCLIGHTS_BLANK_TRIS = 0;
    pCLIGHTS_VPRG_TRIS = 0;

    pCLIGHTS_GSCLK_RPOR = 0b10010;  // Tie output OC1 to GSCLK pin
    //pCLIGHTS_GSCLK_TRIS = 0;

    pCLIGHTS_SCK_PORT = 0;
    pCLIGHTS_DCPRG_PORT = 0;
    pCLIGHTS_VPRG_PORT = 0;
    pCLIGHTS_XLAT_PORT = 0;
    pCLIGHTS_BLANK_PORT = 0;

    T3CONbits.TON = 0;          // Disable Timer
    T3CONbits.TCKPS = 0b00;     // Select 1:1 Prescaler
    TMR3 = 0x00;                // Clear timer register
    PR3 = 1;                 // Load the period value
    T3CONbits.TON = 1;          // Start Timer

    T2CONbits.TON = 0;          // Disable Timer
    T2CONbits.TCKPS = 0b00;     // 1:1
    TMR2 = 0x00;                // Clear timer register
    PR2 = 4100;                    // Load the period value
    IPC1bits.T2IP = 0x01;       // Set Timer2 Interrupt Priority Level
    IFS0bits.T2IF = 0;          // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;          // Enable Timer2 interrupt

    OC1CONbits.OCM = 0b000;     // Disable Output Compare Module
    OC1CONbits.OCTSEL = 1;      // Select Timer 3 as output compare time base

    OC1RS = 1;                  // Load the Compare Register Value for falling edge gsclk output
    OC1CONbits.OCM = 0b110;     // Select the Output Compare PWM mode

    T2CONbits.TON = 1;          // Start Timer

    configureTimer1_60();

}