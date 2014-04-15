/*!\file p2p_demo_tx_node.h
 * \brief P2P demo code on top of the EZMacPRO stack.
 *
 *
 * \n This software must be used in accordance with the End User License
 * \n Agreement.
 *
 * \b COPYRIGHT
 * \n Copyright 2012 Silicon Laboratories, Inc.
 * \n http://www.silabs.com
 */

#ifndef  _P2P_DEMO_TX_NODE_H_
#define  _P2P_DEMO_TX_NODE_H_


                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

/*!
 * Application Firmware version.
 */
#define APP_FW_VERSION    "1.0.0b"


/*!
 * Addresses used in this demo.
 */
#define CUSTOMER_ID                         (0x01)
#define DEVICE_SELF_ID	                    (0x05)
#define SERVER_ID							(0x01)


/*!
 * States of the Star demo.
 */
#define DEMO_BOOT                           (0x10)
#define DEMO_TX                             (0x20)
#define DEMO_TX_WF_ACK                      (0x40)


#define STARTUP_TIMEOUT                     (3)         // sec


/*!
 * LFT timeout macros used exclusively with LFTMR registers.
 */
#define LFTMR2_TIMEOUT_MSEC(n)              (0x44)      // Internal Time Base, WTR=0x04
#define LFTMR1_TIMEOUT_MSEC(n)              (U8)(((U16)((n*32.768F)/64)&0xFF00)>>8) // WTR=0x04 assumed
#define LFTMR0_TIMEOUT_MSEC(n)              (U8)((U16)((n*32.768F)/64)&0x00FF)      // WTR=0x04 assumed
#define LFTMR2_TIMEOUT_SEC(n)               (0x44)      // WTR=0x04
#define LFTMR1_TIMEOUT_SEC(n)               (U8)(((U16)((n*1000*32.768F)/64)&0xFF00)>>8) // WTR=0x04 assumed
#define LFTMR0_TIMEOUT_SEC(n)               (U8)((U16)((n*1000*32.768F)/64)&0x00FF)      // WTR=0x04 assumed


                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

extern SEGMENT_VARIABLE(abRfPayload[64], U8, BUFFER_MSPACE);
extern SEGMENT_VARIABLE(bPacketLength, U8, APPLICATION_MSPACE);
extern SEGMENT_VARIABLE(wPacketCounter, U8, APPLICATION_MSPACE);
extern SEGMENT_VARIABLE(DEMO_SR, U8, APPLICATION_MSPACE);


                /* ======================================= *
                 *  F U N C T I O N   P R O T O T Y P E S  *
                 * ======================================= */

/*!
 * Local function prototypes.
 */
void StateMachine_Init(void);
void StateMachine(void);

void vP2P_demo_TxInit(void);
void vP2P_demo_SendPacketGoToSleep(void);
U8 bitSet_XOR(int len, U8 * buffer);

#pragma pack(push, 1)
typedef struct EZMacProPayload
{
   U8    SID;
   U16   SEQUENSE_NO;
} EZMacProPayloadStruct;

typedef struct{
	U8 STX[2];
	U8 sensor_sequence;
	U8 sensor_ID[2];
	U8 rf_dest[2];
	U8 sos_state;
	U8 hel_state;
	U8 repeater_ID[2];
	U8 repeater_rssi;
	U8 XOR;
	U8 ETX[2];
}EZMacProPayload_SensorNode;

#pragma pack(pop) 

#endif //_P2P_DEMO_TX_NODE_H_
