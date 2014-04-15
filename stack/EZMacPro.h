/*!\file EZMacPro.h
 * \brief EZMacPRO stack definitions.
 *
 * \n EZMacPRO version: 3.0.1r
 *
 * \n This software must be used in accordance with the End User License
 * \n Agreement.
 *
 * \b COPYRIGHT
 * \n Copyright 2012 Silicon Laboratories, Inc.
 * \n http://www.silabs.com
 */

#ifndef _EZMACPRO_H_
#define _EZMACPRO_H_


                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

/*!
 * Fixed timeouts.
 */
// XTAL timeout 1.5ms
#define TIMEOUT_XTAL_START    TIMEOUT_US(1500L)
// LBT ETSI timeout 500us
#define TIMEOUT_LBTI_ETSI     TIMEOUT_US(500L)
//fixed timeout 5ms
#define LBT_FIXED_TIME_5000US TIMEOUT_US(5000L)
//fixed timeout 4.5ms
#define LBT_FIXED_TIME_4500US TIMEOUT_US(4500L)
//fixed timeout 1ms
#define LBT_FIXED_TIME_1000US TIMEOUT_US(1000L)
// PLL settling time 200us
#define PLL_SETTLING_TIME     TIMEOUT_US(200L)
//the SW make up the ACK packet(in 4 Mhz ~1.6ms)
//the ACK packet timeout 5ms
#define MAKE_UP_THE_ACK_PACKET   TIMEOUT_US(5000L)
#define MAX_LBT_WAITING_TIME TIMEOUT_US(12700L)
//------------------------------------------------------------------------------------------------
// EZMacProReg.name.MSR states
//------------------------------------------------------------------------------------------------
#define EZMAC_PRO_SLEEP     0x00
#define EZMAC_PRO_IDLE      0x40
#define EZMAC_PRO_WAKE_UP   0x80
#define WAKE_UP_ERROR       0x8F
//------------------------------------------------------------------------------------------------
// EZMacProReg.name.MSR state bit definitions
//------------------------------------------------------------------------------------------------
#define TX_STATE_BIT        0x10
#define RX_STATE_BIT        0x20
//------------------------------------------------------------------------------------------------
// EZMacProReg.name.MSR TX enumerations
// use with TX_STATE_BIT
//------------------------------------------------------------------------------------------------
#ifndef RECEIVER_ONLY_OPERATION
enum
{
#ifdef TRANSCEIVER_OPERATION
   TX_STATE_LBT_START_LISTEN,
	TX_STATE_LBT_LISTEN,
	TX_STATE_LBT_RANDOM_LISTEN,
#endif//TRANSCEIVER_OPERATION
   TX_STATE_WAIT_FOR_TX,
#ifdef EXTENDED_PACKET_FORMAT
#ifdef TRANSCEIVER_OPERATION
    TX_STATE_WAIT_FOR_ACK,
//    TX_ERROR_NO_ACK,
#endif//TRANSCEIVER_OPERATION
#endif//EXTENDED_PACKET_FORMAT
#ifdef TRANSCEIVER_OPERATION
   TX_ERROR_CHANNEL_BUSY,
#endif//TRANSCEIVER_OPERATION
   TX_ERROR_STATE
};
#endif
//------------------------------------------------------------------------------------------------
// EZMacProReg.name.MSR RX enumerations
// use with RX_STATE_BIT
//------------------------------------------------------------------------------------------------
#ifndef TRANSMITTER_ONLY_OPERATION
enum
{
   RX_STATE_FREQUENCY_SEARCH,
#ifdef ANTENNA_DIVERSITY_ENABLED
   RX_STATE_CHANGE_ANTENNA,
#endif//ANTENNA_DIVERSITY_ENABLED
#ifdef MORE_CHANNEL_IS_USED
   RX_STATE_WAIT_FOR_PREAMBLE,
#endif//MORE_CHANNEL_IS_USED
   RX_STATE_WAIT_FOR_SYNC,
   RX_STATE_WAIT_FOR_PACKET,
#ifdef EXTENDED_PACKET_FORMAT
   RX_STATE_WAIT_FOR_SEND_ACK,
#endif//EXTENDED_PACKET_FORMAT
#ifdef PACKET_FORWARDING_SUPPORTED
#ifdef TRANSCEIVER_OPERATION
   RX_STATE_FORWARDING_LBT_START_LISTEN,
   RX_STATE_FORWARDING_LBT_LISTEN,
	RX_STATE_FORWARDING_LBT_RANDOM_LISTEN,
#endif//TRANSCEIVER_OPERATION
   RX_STATE_FORWARDING_WAIT_FOR_TX,
   RX_ERROR_FORWARDING_WAIT_FOR_TX,
#endif//PACKET_FORWARDING_SUPPORTED
   RX_ERROR_STATE
};
#endif



//------------------------------------------------------------------------------------------------
// SECR register error codes
//------------------------------------------------------------------------------------------------
#define EZMAC_PRO_ERROR_BAD_CRC        0x01
#define EZMAC_PRO_ERROR_BAD_ADDR       0x02
#define EZMAC_PRO_ERROR_BAD_CID        0x04
#define EZMAC_PRO_ERROR_CHANNEL_BUSY   0x08
//------------------------------------------------------------------------------------------------
// MAC register definition
//------------------------------------------------------------------------------------------------
#define EZ_LASTREG      ADCTSV
//------------------------------------------------------------------------------------------------
// OTHER definitions
//------------------------------------------------------------------------------------------------
//sync word length
#define SYNC_WORD_LENGTH 2
//LBT definitions
#define LBT_FIXED_NUMBER		      10
#define LBT_FIXED_BUSY_NUMBER 	   2


//------------------------------------------------------------------------------------------------
// Debug Trap defined only for debug
//------------------------------------------------------------------------------------------------
#define DEBUG_TRAP()   while(1)       // debug

//================================================================================================
//
// MAC return parameter enumeration
//
//================================================================================================
typedef enum
{
   MAC_OK = 0,
   NAME_ERROR,
   VALUE_ERROR,
   STATE_ERROR,
   WAKEUP_ERROR,
   LBT_ERROR,
   PACKET_SENT,
   ACK_RX_ERROR,
   INCONSISTENT_SETTING,
	CHIPTYPE_ERROR
}MacParams;
//================================================================================================
//
// MAC register  enumeration
//
//================================================================================================
typedef enum
{
// mandatory elements listed first
   MCR = 0,                      // Master Control Register
   SECR,                         // State & Error Counter Control Register
   TCR,                          // Transmit Control Register
   RCR,                          // Receiver Control Register
	FR0,									// Frequency Register 0
	FR1,									// Frequency Register 1
	FR2,									// Frequency Register 2
	FR3,									// Frequency Register 3
#ifdef MORE_CHANNEL_IS_USED
	FR4,
	FR5,
	FR6,
	FR7,
	FR8,
	FR9,
	FR10,
	FR11,
	FR12,
	FR13,
	FR14,
	FR15,
	FR16,
	FR17,
	FR18,
	FR19,
	FR20,
	FR21,
	FR22,
	FR23,
	FR24,
	FR25,
	FR26,
	FR27,
	FR28,
	FR29,
	FR30,
	FR31,
	FR32,
	FR33,
	FR34,
	FR35,
	FR36,
	FR37,
	FR38,
	FR39,
	FR40,
	FR41,
	FR42,
	FR43,
	FR44,
	FR45,
	FR46,
	FR47,
	FR48,
	FR49,
#endif//MORE_CHANNEL_IS_USED
	FSR,									// Frequency Select Register
#ifdef FOUR_CHANNEL_IS_USED
   EC0,                          // Error Counter of Frequency 0
   EC1,                          // Error Counter of Frequency 1
   EC2,                          // Error Counter of Frequency 2
   EC3,                          // Error Counter of Frequency 3
#endif//FOUR_CHANNEL_IS_USED
   PFCR,                         // Packet Filter Control Register
   SFLT,                         // Sender ID Filter
   SMSK,                         // Sender ID Filter Mask
   MCA_MCM,                      // Multicast Address / Multicast Mask
   MPL,                          // Maximum Packet Length
   MSR,                          // MAC Status Register
   RSR,                          // Receive Status Register
	RFSR,									// Received Frequency Status Register
   RSSI,                         // Received Signal Strength Indicator
   SCID ,                        // Self Customer ID
   SFID,                         // Self ID
   RCTRL,                        // Received Control Byte
   RCID,                         // Received Customer ID
   RSID,                         // Received Sender ID
   DID,                          // Destination ID
   PLEN                          // Payload Length
#ifdef TRANSCEIVER_OPERATION
 	,LBTIR,                       // Listen Before Talk  Interval Register
   LBTLR                         // Listen Before Talk Limit Register
#endif//TRANSCEIVER_OPERATION
 	,LFTMR0,                      // Low Frequency Timer Setting Register 0
   LFTMR1,                       // Low Frequency Timer Setting Register 1
   LFTMR2,                       // Low Frequency Timer Setting Register 2
 	LBDR,                         // Low Battery Detect Register
   ADCTSR,                       // ADC and Temperature Sensor Register
   ADCTSV,                       // ADC/Temperature Value Register
	DTR									// Device Type Register
}MacRegs;

//------------------------------------------------------------------------------------------------
// EZMacProReg structure (must match enumeration order and compiler options
//------------------------------------------------------------------------------------------------
typedef struct EZMacProRegStruct
{
   // mandatory elements listed first
   U8    MCR;                          // Master Control Register
   U8    SECR;                         // State & Error Counter Control Register
   U8    TCR;                          // Transmit Control Register
   U8    RCR;                          // Receiver Control Register
   U8    FR0;                          // Frequency Register 0
   U8    FR1;                          // Frequency Register 1
   U8    FR2;                          // Frequency Register 2
   U8    FR3;                          // Frequency Register 3
#ifdef MORE_CHANNEL_IS_USED
	U8		FR4;
	U8		FR5;
	U8		FR6;
	U8		FR7;
	U8		FR8;
	U8		FR9;
	U8		FR10;
	U8		FR11;
	U8		FR12;
	U8		FR13;
	U8		FR14;
	U8		FR15;
	U8		FR16;
	U8		FR17;
	U8		FR18;
	U8		FR19;
	U8		FR20;
	U8		FR21;
	U8		FR22;
	U8		FR23;
	U8		FR24;
	U8		FR25;
	U8		FR26;
	U8		FR27;
	U8		FR28;
	U8		FR29;
	U8		FR30;
	U8		FR31;
	U8		FR32;
	U8		FR33;
	U8		FR34;
	U8		FR35;
	U8		FR36;
	U8		FR37;
	U8		FR38;
	U8		FR39;
	U8		FR40;
	U8		FR41;
	U8		FR42;
	U8		FR43;
	U8		FR44;
	U8		FR45;
	U8		FR46;
	U8		FR47;
	U8		FR48;
	U8		FR49;
#endif//MORE_CHANNEL_IS_USED
	U8 	FSR;									// Frequency Select Register
#ifdef FOUR_CHANNEL_IS_USED
   U8    EC0;                          // Error Counter of Frequency 0
   U8    EC1;                          // Error Counter of Frequency 1
   U8    EC2;                          // Error Counter of Frequency 2
   U8    EC3;                          // Error Counter of Frequency 3
#endif//FOUR_CHANNEL_IS_USED
   U8    PFCR;                         // Packet Filter Control Register
   U8    SFLT;                         // Sender ID Filter
   U8    SMSK;                         // Sender ID Filter Mask
   U8    MCA_MCM;                      // Multicast Address / Multicast Mask
   U8    MPL;                          // Maximum Packet Length
   U8    MSR;                          // MAC Status Register
   U8    RSR;                          // Receive Status Register
	U8 	RFSR;									// Received Frequency Status Register
   U8    RSSI;                         // Received Signal Strength Indicator
   U8    SCID ;                        // Self Customer ID
   U8    SFID;                         // Self ID
   U8    RCTRL;                        // Received Control Byte
   U8    RCID;                         // Received Customer ID
   U8    RSID;                         // Received Sender ID
   U8    DID;                          // Destination ID
   U8    PLEN;                         // Payload Length
#ifdef TRANSCEIVER_OPERATION
   U8    LBTIR;                        // Listen Before Talk  Interval Register
   U8    LBTLR;                        // Listen Before Talk Limit Register
#endif//TRANSCEIVER_OPERATION
   U8    LFTMR0;                       // Low Frequency Timer Setting Register 0
   U8    LFTMR1;                       // Low Frequency Timer Setting Register 1
   U8    LFTMR2;                       // Low Frequency Timer Setting Register 2
   U8    LBDR;                         // Low Battery Detect Register
   U8    ADCTSR;                       // ADC and Temperature Sensor Register
   U8    ADCTSV;                       // ADC/Temperature Value Register
	U8		DTR;									// Device Type Register

} EZMacProRegStruct;
//------------------------------------------------------------------------------------------------
// Union used for arrat and structure access
//------------------------------------------------------------------------------------------------
typedef union
{
    EZMacProRegStruct   name;
    U8                  array[DTR + 1];
} EZMacProUnion;
//------------------------------------------------------------------------------------------------
// packet forward typedef
//------------------------------------------------------------------------------------------------
typedef struct ForwardedPacketTableEntry
{
   U8 sid;
   U8 seq;
   U8 chan;
} ForwardedPacketTableEntry;


                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

extern SEGMENT_VARIABLE(EZMacProReg, EZMacProUnion, REGISTER_MSPACE);
extern BIT fHeaderErrorOccurred;
extern SEGMENT_VARIABLE(RxBuffer[RECEIVED_BUFFER_SIZE], U8 , BUFFER_MSPACE);
extern SEGMENT_VARIABLE(ForwardedPacketTable[FORWARDED_PACKET_TABLE_SIZE], ForwardedPacketTableEntry, FORWARDED_PACKET_TABLE_MSPACE);
extern SEGMENT_VARIABLE(AckBufSize, U8 , EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(AckBuffer[ACK_BUFFER_SIZE], U8 , BUFFER_MSPACE);
extern SEGMENT_VARIABLE(EZMacProLBT_Retrys, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(BusyLBT, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(EZMacProSequenceNumber, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(EZMacProCurrentChannel, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(EZMacProRSSIvalue, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(EZMacProReceiveStatus, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(RX_Freq_dev, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TX_Freq_dev, U8, EZMAC_PRO_GLOBAL_MSPACE);
// dynamic timeouts- depend on data rate and MAC settings
extern SEGMENT_VARIABLE(TimeoutSyncWord, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TimeoutRX_Packet, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TimeoutTX_Packet, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TimeoutACK, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TimeoutChannelSearch, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(EZMacProRandomNumber, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(TimeoutLBTI, U32, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(Selected_Antenna, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(SelectedChannel, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(maxChannelNumber, U8, EZMAC_PRO_GLOBAL_MSPACE);
extern SEGMENT_VARIABLE(PreamRegValue, U8, EZMAC_PRO_GLOBAL_MSPACE);


                /* ======================================= *
                 *  F U N C T I O N   P R O T O T Y P E S  *
                 * ======================================= */

MacParams EZMacPRO_Init(void);
MacParams EZMacPRO_Wake_Up(void);
MacParams EZMacPRO_Sleep(void);
MacParams EZMacPRO_Idle(void);
MacParams EZMacPRO_Transmit(void);
MacParams EZMacPRO_Receive(void);
MacParams EZMacPRO_Idle(void);
MacParams EZMacPRO_Reg_Write(MacRegs name, U8);
MacParams EZMacPRO_Reg_Read (MacRegs name, U8 *value);
MacParams EZMacPRO_TxBuf_Write(U8, VARIABLE_SEGMENT_POINTER(payload, U8, BUFFER_MSPACE));
MacParams EZMacPRO_RxBuf_Read(VARIABLE_SEGMENT_POINTER(length, U8, BUFFER_MSPACE), VARIABLE_SEGMENT_POINTER(payload, U8, BUFFER_MSPACE));
MacParams EZMacPRO_Ack_Write(U8 length, VARIABLE_SEGMENT_POINTER(payload, U8, BUFFER_MSPACE));

void SetRfParameters(U8);
void macSpecialRegisterSettings(U8);
void macUpdateDynamicTimeouts (U8, U8);
#ifdef PACKET_FORWARDING_SUPPORTED
void initForwardedPacketTable (void);
#endif //PACKET_FORWARDING_SUPPORTED
void macSetEnable2(U8);
void macSetFunction1(U8);
#ifdef TRANSCEIVER_OPERATION
void macUpdateLBTI (U8);
#endif//TRANSCEIVER_OPERATION






#endif //_EZMACPRO_H_
