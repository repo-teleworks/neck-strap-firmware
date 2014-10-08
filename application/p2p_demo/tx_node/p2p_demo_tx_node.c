/*!\file p2p_demo_tx_node.c
 * \brief P2P demo code on top of the EZMacPRO stack.
 *
 * \n P2P demo Transmitter node code description:
 * \n After power on reset the Transmitter node goes into Sleep state to wait
 * \n for the 1 second Low-Frequency Timer timeout. When the timer expires a
 * \n packet is transmitted with an ascending number in its payload. The content
 * \n of the last packet is always updated on the LCD before the radio is placed
 * \n into Sleep.
 * \n LED1 toggles when a packet transmission is acknowledged by the Receiver.
 *
 *
 * \n This software must be used in accordance with the End User License
 * \n Agreement.
 *
 * \b COPYRIGHT
 * \n Copyright 2012 Silicon Laboratories, Inc.
 * \n http://www.silabs.com
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */
#include "..\..\common.h"
#include "p2p_demo_tx_node.h"
#include "p2p_demo_tx_menu.h"


                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

SEGMENT_VARIABLE(abRfPayload[64], U8, BUFFER_MSPACE);
SEGMENT_VARIABLE(bPacketLength, U8, APPLICATION_MSPACE);
SEGMENT_VARIABLE(wPacketCounter, U8, APPLICATION_MSPACE);
SEGMENT_VARIABLE(DEMO_SR, U8, APPLICATION_MSPACE);
SEGMENT_VARIABLE(STATE_MAG_SW, U8, APPLICATION_MSPACE);
SEGMENT_VARIABLE(STATE_SOS_SW, U8, APPLICATION_MSPACE);
SEGMENT_VARIABLE(STATE_CHANGE_SOS, U8, APPLICATION_MSPACE);


#ifdef TRACE_ENABLED
  SEGMENT_VARIABLE(printBuf[40], U8, APPLICATION_MSPACE);
#endif //TRACE_ENABLED


                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

/*!
 * Entry point to the state machine.
 */
void StateMachine_Init(void)
{
    /* Disable Watchdog. */
    DISABLE_WATCHDOG();
    /* Init Demo State Machine. */
    DEMO_SR = DEMO_BOOT;
}


/*!
 * This is the State Machine of the Demo Application.
 */
void StateMachine(void)
{
	unsigned short timeout;
    switch (DEMO_SR)
    {
        case DEMO_BOOT:
            /* Initialise board. */
            BoardInit();
            /* Enable global interrupts. */
            ENABLE_GLOBAL_INTERRUPTS();
            /* Initialise EZMacPRO. */
            EZMacPRO_Init();
            /* Wait until device goes to Sleep. */
            // while (!fEZMacPRO_StateSleepEntered);
           	timeout = 50;
            while(!fEZMacPRO_StateSleepEntered){
            	if(1 > timeout){
            		ENABLE_WATCHDOG();
            	}
				else{
					delay_ms(10);
					timeout--;
				}
            }

            /* Clear State transition flags. */
            fEZMacPRO_StateWakeUpEntered = 0;
            fEZMacPRO_StateSleepEntered = 0;
            /* Point to point demo initialisation. */
            vP2P_demo_TxInit();

			STATE_MAG_SW = 0;
			STATE_MAG_SW = 0;
			STATE_CHANGE_SOS = 0;

            LED1 = ILLUMINATE;
            
#ifdef BOAD_VERSION_TELEWORKS    // teleworks
			LED2 = EXTINGUISH;
            LED_SOS = EXTINGUISH;
#endif
            /* Go to TX state. */
            DEMO_SR = DEMO_TX;
            break;

        case DEMO_TX:
            /* LFT expired, send next packet. */
            if (fEZMacPRO_LFTimerExpired)
            {   /* Clear flag. */
                fEZMacPRO_LFTimerExpired = 0;

#ifdef BOAD_VERSION_TELEWORKS    // teleworks
				if( 0 == MAG_SENSOR) {	// no wear chin strap
					STATE_MAG_SW = 1;
					LED2 = ILLUMINATE;
				}
				else{
					STATE_MAG_SW = 0;
					LED2 = EXTINGUISH;
				}

				if(1 == SW_SOS) {
					STATE_CHANGE_SOS =1;
				}

				if(1 == STATE_CHANGE_SOS && 0 == STATE_SOS_SW && 0 == SW_SOS) {		// SOS KEY
					STATE_CHANGE_SOS = 0;
					STATE_SOS_SW = 1;
					LED_SOS = ILLUMINATE;
				}
				else if(1 == STATE_CHANGE_SOS && 1 == STATE_SOS_SW && 0 == SW_SOS) {	// SOS KEY
					STATE_CHANGE_SOS = 0;
					STATE_SOS_SW = 0;
					LED_SOS = EXTINGUISH;
				}
#endif
                /* Send packet then place the radio to sleep. */
                vP2P_demo_SendPacketGoToSleep();
				LED1 = !LED1;

                /* Go to TX wait for acknowledgement state. */
                DEMO_SR = DEMO_TX_WF_ACK;
            }
            break;

        case DEMO_TX_WF_ACK:
            /* Auto-acknowledgement has arrived. */
            if (fEZMacPRO_PacketSent)
            { /* Clear flag. */
                fEZMacPRO_PacketSent = 0;
               // LED1 = !LED1;
                /* Go to TX state. */
                DEMO_SR = DEMO_TX;
            }

            /* Auto-acknowledgement has not arrived. */
            if (fEZMacPRO_AckTimeout)
            { /* Clear flag. */
                fEZMacPRO_AckTimeout = 0;
                /* Go to TX state. */
                DEMO_SR = DEMO_TX;
            }
            break;

        default:
            break;
    }
}



void vP2P_demo_TxInit(void)
{
	unsigned short timeout;
    /* Configure and start 2sec timeout for Silabs splash screen. */
    EZMacPRO_Reg_Write(LFTMR0, LFTMR0_TIMEOUT_SEC(STARTUP_TIMEOUT));
    EZMacPRO_Reg_Write(LFTMR1, LFTMR1_TIMEOUT_SEC(STARTUP_TIMEOUT));
    EZMacPRO_Reg_Write(LFTMR2, 0x80 | LFTMR2_TIMEOUT_SEC(STARTUP_TIMEOUT));
  #ifdef SDBC
    /* Show Silabs logo, demo name & version. */
    vPerformMenu(sMenu_StartupScreen);
  #endif //SDBC
    /* Wait here until LFT expires. */
    //while(!fEZMacPRO_LFTimerExpired);
	timeout = 200;
	while(!fEZMacPRO_LFTimerExpired){
	  if(1 > timeout){
		  ENABLE_WATCHDOG();
	  }
	  else{
		  delay_ms(10);
		  timeout--;
	  }
	}

    /* Clear flag. */
    fEZMacPRO_LFTimerExpired = 0;
    /* Disable LFT. */
    EZMacPRO_Reg_Write(LFTMR2, ~0x80 & LFTMR2_TIMEOUT_SEC(STARTUP_TIMEOUT));
    /* Init counter. */
    wPacketCounter = 0;
    /* Init packet content. */
    // memcpy(&abRfPayload[0], "      ", 6);
    memset(&abRfPayload[0], 0x00, 64);
    EZMacPRO_Reg_Write(MCR, 0xAC);              // Set data rate to 9.6kbps, DNPL = 1, rad = 1, used 1 channel
    EZMacPRO_Reg_Write(SECR, 0x20);             // State after receive is RX state and state after transmit is Sleep state
    EZMacPRO_Reg_Write(TCR, 0xB8);              // LBT Before Talk enabled, Output power: +20 dBm, no ACK, AFC disable
    EZMacPRO_Reg_Write(LBTLR, 0x78);            // RSSI threshold -60 dB
    EZMacPRO_Reg_Write(LBTIR, 0x8A);            // Time interval
    EZMacPRO_Reg_Write(FR0, 1);                 // set the used frequency channel
    EZMacPRO_Reg_Write(SCID, CUSTOMER_ID);      // set the customer ID to 0x01
    EZMacPRO_Reg_Write(SFID, DEVICE_SELF_ID);   // set the self ID
    EZMacPRO_Reg_Write(LFTMR0, 0x00);           // Set the interval to 5s
    EZMacPRO_Reg_Write(LFTMR1, 0x0A);           // Set the interval to 5s
    EZMacPRO_Reg_Write(LFTMR2, 0xC4);           // Enable LFT, use Internal Time Base
  #ifdef SDBC
    /* Show TX screen on LCD. */
    vPerformMenu(sMenu_TxScreen);
  #endif //SDBC
  #ifdef TRACE_ENABLED
    printf("[TX_NODE][DEMO_BOOT] Startup done.\n");
    printf("[TX_NODE][DEMO_TX] Went to sleep.\n");
  #endif //TRACE_ENABLED
}


void vP2P_demo_SendPacketGoToSleep()
{
	unsigned short timeout;
	//EZMacProPayloadStruct * PayloadStruct = &abRfPayload[0];
	EZMacProPayload_SensorNode * PayloadStruct = &abRfPayload[0];
    /* Set packet length. */
    // bPacketLength = 5;
    bPacketLength = sizeof(EZMacProPayload_SensorNode);
    /* Increase counter, set up packet content. */
    // sprintf(abRfPayload, "%1u", ++wPacketCounter);
    // PayloadStruct->SID = DEVICE_SELF_ID;
    // PayloadStruct->SEQUENSE_NO = ++wPacketCounter;
    
	PayloadStruct->STX[0]		= 0xBB;
	PayloadStruct->STX[1]		= 0xAA;
	PayloadStruct->sensor_sequence = ++wPacketCounter;
	PayloadStruct->sensor_ID[0]	= (U8)(DEVICE_SELF_ID);
	PayloadStruct->sensor_ID[1]	= 0x00;
	PayloadStruct->rf_dest[0]	= 0xFF;
	PayloadStruct->rf_dest[1]	= 0xFF;
	PayloadStruct->ETX[0]		= 0xCC;
	PayloadStruct->ETX[1]		= 0xDD;
	PayloadStruct->repeater_ID[0] = 0x00;
	PayloadStruct->repeater_ID[1] = 0x00;
	PayloadStruct->repeater_rssi = 0x00;

	if(1 == STATE_SOS_SW)
		PayloadStruct->sos_state = 1;
	else
		PayloadStruct->sos_state = 0;
		
	if(1 == STATE_MAG_SW)
		PayloadStruct->hel_state = 1;
	else
		PayloadStruct->hel_state = 0;

	PayloadStruct->XOR = bitSet_XOR(sizeof (EZMacProPayload_SensorNode) -3 , (U8 *)PayloadStruct);

    /* Set Destination ID. */
    //EZMacPRO_Reg_Write(DID, 0x01);
    EZMacPRO_Reg_Write(DID, SERVER_ID);
    /* Write the packet length and payload to the TX buffer. */
    EZMacPRO_TxBuf_Write(bPacketLength, &abRfPayload[0]);
    //EZMacPRO_TxBuf_Write(sizeof(EZMacProPayloadStruct), &abRfPayload[0]);
    /* Wake up from Sleep mode. */
    EZMacPRO_Wake_Up();
    /* Wait until device goes to Idle. */
    // while (!fEZMacPRO_StateIdleEntered);
	timeout = 50;
	while(!fEZMacPRO_StateIdleEntered){
	  if(1 > timeout){
		  ENABLE_WATCHDOG();
	  }
	  else{
		  delay_ms(10);
		  timeout--;
	  }
	}

    /* Clear State transition flags. */
    fEZMacPRO_StateWakeUpEntered = 0;
    fEZMacPRO_StateIdleEntered = 0;
  #ifdef SDBC
    /* Display Transmit on LCD. */
    vPerformMenu(sMenu_PacketTransmit);
  #endif //SDBC
  #ifdef TRACE_ENABLED
    printf("[TX_NODE][DEMO_SLEEP] Woke up.\n");
  #endif //TRACE_ENABLED
    /* Send the packet. */
    EZMacPRO_Transmit();
    /* Wait until device goes back to Sleep. */
    // while (!fEZMacPRO_StateSleepEntered);
	timeout = 50;
	while(!fEZMacPRO_StateSleepEntered){
	  if(1 > timeout){
		  ENABLE_WATCHDOG();
	  }
	  else{
		  delay_ms(10);
		  timeout--;
	  }
	}
	
    /* Clear State transition flag. */
    fEZMacPRO_StateSleepEntered = 0;
  #ifdef SDBC
    /* Update number of transmitted packets on LCD. */
    vPerformMenu(sMenu_TxRefreshCounter);
  #endif //SDBC
  #ifdef TRACE_ENABLED
    printf("[TX_NODE][DEMO_TX] Transmitted packet. Packet content:%5u\n", wPacketCounter);
  #endif //TRACE_ENABLED
  #ifdef SDBC
    /* Display Sleep on LCD. */
    vPerformMenu(sMenu_SleepMode);
  #endif //SDBC
  #ifdef TRACE_ENABLED
    printf("[TX_NODE][DEMO_SLEEP] Went to sleep.\n");
  #endif //TRACE_ENABLED
}

U8 bitSet_XOR(int len, U8 * buffer){

	U8 XOR=0;
	U16 i;
	for(i =0 ; i < len ; i++){
		XOR ^= buffer[i];
	}
	return XOR;
}

