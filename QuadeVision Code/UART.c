#include "msp.h"
#include "project.h"
/*
 * UART.c
 *
 *  Created on: Nov 2, 2020
 *      Author: rylan
 *      Directly adapted from the source link in main.c
 */

void config_UART(){
    /*
     * we modified the TI driver to use UART at a baud rate of 115200. This outputs to a serial monitor
     *
     * */

    /*         UART                      */ //Adapted from source link in main.c which got their code from -> MSP432P401 Demo - eUSCI_A0 UART echo at 9600 baud using BRCLK = 12MHz by Ti
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses


    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
//    // Baud Rate calculation
//    // 12000000/(16*9600) = 78.125
//    // Fractional portion = 0.125
//    // User's Guide Table 21-4: UCBRSx = 0x10
//    // UCBRFx = int ( (78.125-78)*16) = 2
//    EUSCI_A0->BRW = 78;//78;                     // 12000000/16/9600
//    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |//OLD BAUD RATE SETUP
//            EUSCI_A_MCTLW_OS16;
    // Baud Rate calculation for 115200 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 12000000/(16*115200) = 6.510
    // Fractional portion = 0.510
    // User's Guide Table 21-4: UCBRSx = 0xAA
    // UCBRFx = int ( (0.510)*16) = 8
    EUSCI_A0->BRW = 6;                     // 12000000/16/115200
    EUSCI_A0->MCTLW = (8 << EUSCI_A_MCTLW_BRF_OFS) |//was2
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Disable USCI_A0 RX interrupt

}

void uart_out(const char *str){ //Adapted from source in main.c
    if (str != '\0') { //IF the buffer is not empty

        while (*str != '\0') { //runs through all of the characters in the buffer
            /* Wait for the transmit buffer to be ready */
            while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

            /* Transmit data */
            EUSCI_A0->TXBUF  = *str;

            /* If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
                EUSCI_A0->TXBUF = '\r'; //Goes to the next line
            }

            str++; //Move to the next character in the buffer.
        }
    }
}





