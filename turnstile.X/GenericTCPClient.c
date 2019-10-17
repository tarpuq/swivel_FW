/*********************************************************************
 *
 *  Generic TCP Client Example Application
 *  Module for Microchip TCP/IP Stack
 *   -Implements an example HTTP client and should be used as a basis 
 *	  for creating new TCP client applications
 *	 -Reference: None.  Hopefully AN833 in the future.
 *
 *********************************************************************
 * FileName:        GenericTCPClient.c
 * Dependencies:    TCP, DNS, ARP, Tick
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     8/01/06	Original
 ********************************************************************/
#define __GENERICTCPCLIENT_C

#include <stdint.h>

#include "TCPIPConfig.h"

#if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)

#include "TCPIP Stack/TCPIP.h"
#include "mcc_generated_files/pin_manager.h"

static uint8_t txBufferLen;
static uint8_t txBuffer[10];
static uint8_t rxBufferLen;
BYTE rxBuffer[10];

static uint8_t rxDataReady = 0;
static uint8_t transmit = 0;

uint8_t isServerDataReady()
{
    if (rxDataReady == 1)
    {
        rxDataReady = 0;
        return 1;
    }
    return 0;
}

uint8_t getFrame(uint8_t *buffer)
{
    memcpy(buffer, rxBuffer, rxBufferLen);
    return rxBufferLen;
}

void setFrame(uint8_t *buffer, uint8_t len)
{
    memcpy(txBuffer, buffer, len);
    txBufferLen = len;
    transmit = 1;
}

/*****************************************************************************
  Function:
    void GenericTCPClient(void)

  Summary:
    Implements a simple HTTP client (over TCP).

  Description:
    This function implements a simple HTTP client, which operates over TCP.  
    The function is called periodically by the stack, and waits for BUTTON1 
    to be pressed.  When the button is pressed, the application opens a TCP
    connection to an Internet search engine, performs a search for the word
    "Microchip" on "microchip.com", and prints the resulting HTML page to
    the UART.
	
    This example can be used as a model for many TCP and HTTP client
    applications.

  Precondition:
    TCP is initialized.

  Parameters:
    None

  Returns:
    None
 ***************************************************************************/
void GenericTCPClient(void)
{
    BYTE i;
    WORD w;
    static DWORD Timer;
    static TCP_SOCKET MySocket = INVALID_SOCKET;

    static enum _GenericTCPExampleState
    {
        SM_OPEN = 0,
        SM_SOCKET_OBTAINED,
#if defined(STACK_USE_SSL_CLIENT)
        SM_START_SSL,
#endif
        SM_PROCESS_RESPONSE,
        SM_DISCONNECT,
        SM_WAIT_REQUEST,
        SM_IDLE
    } GenericTCPExampleState = SM_WAIT_REQUEST;

    switch (GenericTCPExampleState)
    {
    case SM_OPEN:
        // Connect a socket to the remote TCP server
        MySocket = TCPOpen(AppConfig.ServerIP.Val, TCP_OPEN_IP_ADDRESS, AppConfig.ServerPort, TCP_PURPOSE_GENERIC_TCP_CLIENT);

        // Abort operation if no TCP socket of type TCP_PURPOSE_GENERIC_TCP_CLIENT is available
        // If this ever happens, you need to go add one to TCPIPConfig.h
        if (MySocket == INVALID_SOCKET)
            break;

#if defined(STACK_USE_UART)
        putrsUART((ROM char*) "\r\n\r\nConnecting using Microchip TCP API...\r\n");
#endif

        GenericTCPExampleState++;
        Timer = TickGet();
        break;

    case SM_SOCKET_OBTAINED:
        // Wait for the remote server to accept our connection request
        if (!TCPIsConnected(MySocket))
        {
            // Time out if too much time is spent in this state
            if (TickGet() - Timer > 5 * TICK_SECOND)
            {
                // Close the socket so it can be used by other modules
                TCPDisconnect(MySocket);
                MySocket = INVALID_SOCKET;
                GenericTCPExampleState--;
            }
            break;
        }

        Timer = TickGet();

#if defined (STACK_USE_SSL_CLIENT)
        if (!TCPStartSSLClient(MySocket, (void *) "thishost"))
            break;
        GenericTCPExampleState++;
        break;

    case SM_START_SSL:
        if (TCPSSLIsHandshaking(MySocket))
        {
            if (TickGet() - Timer > 10 * TICK_SECOND)
            {
                // Close the socket so it can be used by other modules
                TCPDisconnect(MySocket);
                MySocket = INVALID_SOCKET;
                GenericTCPExampleState = SM_OPEN;
            }
            break;
        }
#endif

        // Make certain the socket can be written to
        if (TCPIsPutReady(MySocket) < 125u)
            break;

        //  Frame process
        TCPPutArray(MySocket, txBuffer, txBufferLen);

        // Send the packet
        TCPFlush(MySocket);
        GenericTCPExampleState++;
        break;

    case SM_PROCESS_RESPONSE:
        // Check to see if the remote node has disconnected from us or sent us any application data
        // If application data is available, write it to the UART
        if (!TCPIsConnected(MySocket))
        {
            GenericTCPExampleState = SM_DISCONNECT;
            // Do not break;  We might still have data in the TCP RX FIFO waiting for us
        }

        // Get count of RX bytes waiting
        w = TCPIsGetReady(MySocket);
        rxBufferLen = w;

        // Obtian and print the server reply
        i = sizeof (rxBuffer) - 1;
        rxBuffer[i] = '\0';
        while (w)
        {
            if (w < i)
            {
                i = w;
                rxBuffer[i] = '\0';
            }
            w -= TCPGetArray(MySocket, rxBuffer, i);
#if defined(STACK_USE_UART)
            putsUART((char*) rxBuffer);
#endif

            rxDataReady = 1;

            // putsUART is a blocking call which will slow down the rest of the stack 
            // if we shovel the whole TCP RX FIFO into the serial port all at once.  
            // Therefore, let's break out after only one chunk most of the time.  The 
            // only exception is when the remote node disconncets from us and we need to 
            // use up all the data before changing states.
            if (GenericTCPExampleState == SM_PROCESS_RESPONSE)
            {
                GenericTCPExampleState = SM_DISCONNECT;
                break;
            }
        }
        break;

    case SM_DISCONNECT:
        // Close the socket so it can be used by other modules
        // For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
        TCPDisconnect(MySocket);
        MySocket = INVALID_SOCKET;
        GenericTCPExampleState = SM_WAIT_REQUEST;
        break;
    case SM_WAIT_REQUEST:
        if (transmit)
        {
            transmit = 0;
            GenericTCPExampleState = SM_OPEN;
        }
        break;
    case SM_IDLE:
        break;
    }
}

#endif	//#if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)
