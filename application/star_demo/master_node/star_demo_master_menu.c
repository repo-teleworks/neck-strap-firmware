/*!\file star_demo_master_menu.c
 * \brief Star demo code on top of the EZMacPRO stack.
 *
 *
 * \n This software must be used in accordance with the End User License
 * \n Agreement.
 *
 * \b COPYRIGHT
 * \n Copyright 2012 Silicon Laboratories, Inc.
 * \n http://www.silabs.com
 */

#include "..\..\common.h"
#include "star_demo_master_node.h"
#include "star_demo_master_menu.h"


SEGMENT_VARIABLE(lcd_data[22], U8, TEST_CODE_MSPACE);


#ifdef DOG_LCD_ENABLED
/*!
 * Display the actual menu on the LCD.
 */
void PerformMenu(MENU_ITEM menu_item)
{
    U8 bCnt;

    switch (menu_item)
    {
        case sMenu_StartupScreen:
            // Display SiLabs startup screen
            LcdClearDisplay();
            LcdSetPictureCursor(LCD_LINE_1, 20);
            LcdDrawPicture(silabs87x40);
            memcpy(lcd_data, "Network Application  ", 21);
            LcdWriteLine(LCD_LINE_7, lcd_data);
            sprintf(lcd_data, "  Star Demo v       ");
            memcpy(&lcd_data[13], APP_FW_VERSION, 6);
            LcdWriteLine(LCD_LINE_8, lcd_data);
            break;

        case sMenu_MainScreen:
            //show main screen
            LcdClearDisplay();
            LcdSetPictureCursor(1, 1);
            LcdDrawPicture(ant_slave1_11x8);
            LcdSetPictureCursor(1, 29);
            LcdDrawPicture(ant_slave2_13x8);
            LcdSetPictureCursor(1, 59);
            LcdDrawPicture(ant_slave3_13x8);
            LcdSetPictureCursor(1, 89);
            LcdDrawPicture(ant_slave4_13x8);
            memcpy(lcd_data, "---------------------", 21);
            LcdWriteLine(LCD_LINE_2, lcd_data);
            LcdWriteLine(LCD_LINE_7, lcd_data);
            memcpy(lcd_data, "Master - Boot        ", 21);
            LcdWriteLine(LCD_LINE_8, lcd_data);
            break;

        case sMenu_Associate_State:
            memcpy(lcd_data, "Master - Associate   ", 21);
            LcdWriteLine(LCD_LINE_8, lcd_data);
            break;

        case sMenu_StatusUpdate_State:
            memcpy(lcd_data, "Master - Slave query ", 21);
            LcdWriteLine(LCD_LINE_8, lcd_data);
            break;

        case sMenu_RefreshScreen:
            /* Loop over Association table. */
            for (bCnt = 0; bCnt < MAX_NMBR_OF_SLAVES; bCnt++)
            {   /* Update Rssi on the LCD. */
                LcdShowRssi(bCnt, SlaveInfoTable[bCnt].rssi);
                /* Associated node info - temperature and voltage units. */
                if(SlaveInfoTable[bCnt].associated == ASSOCIATED)
                {   /* Update slave status. */
                    SprintfSlaveInfo(&SlaveInfoTable[bCnt], lcd_data);
                }
                else
                {   /* Update slave status - not associated. */
                    SprintfSlaveNotAssoc(lcd_data);
                }
                LcdWriteLine(LCD_LINE_3 + bCnt, lcd_data);
            }
            break;

        case sMenu_SleepState:
            memcpy(lcd_data, "Master - Sleep       ", 21);
            LcdWriteLine(LCD_LINE_8, lcd_data);
            break;

        default:
            break;

    }
    return;
}
#endif //DOG_LCD_ENABLED


#ifdef DOG_LCD_ENABLED
//------------------------------------------------------------------------------------------------
// void TurnOnLedShowRssi(U8 node, U8 rssi)
//
// turn on the LED and print RSSI info for the given slave
//
// Return Value : None
// Parameters   : node - slave address
//                rssi - rssi of the last received slave node
//
//-----------------------------------------------------------------------------------------------
void LcdShowRssi(U8 node, U8 rssi)
{
    U8 rssi_address;

    switch (node)
    {
        case 0:
            rssi_address = 13;
            break;

        case 1:
            rssi_address = 43;
            break;

        case 2:
            rssi_address = 73;
            break;

        case 3:
            rssi_address = 103;
            break;

        default:
            rssi_address = 0;
            break;
    }
    //clear the previous RSSI
    PrintRssi(1, rssi_address, 0);
    //draw the bargraph
    PrintRssi(1, rssi_address, rssi);
}
#endif //DOG_LCD_ENABLED


#ifdef DOG_LCD_ENABLED
//------------------------------------------------------------------------------------------------
// void PrintRssi(U8 row, U8 column, U8 rssi)
//
// print RSSI information to the LCD
//
// Return Value : None
// Parameters   : row - nmbr of row on the LCD
//                column - start position
//                rssi - rssi of the last received packet
//
//-----------------------------------------------------------------------------------------------
void PrintRssi(U8 row, U8 column, U8 rssi)
{
    //set picture position
    LcdSetPictureCursor(row, column);
    //draw the picture
    if (rssi == 0)
    { //no packet were received
        LcdDrawPicture(rssi_clear_14x8);
        return;
    }
    if (rssi < 55)
    { //rssi1
        LcdDrawPicture(rssi12x8);
        return;
    }
    if (rssi < 65)
    { //rssi2
        LcdDrawPicture(rssi24x8);
        return;
    }
    if (rssi < 75)
    { //rssi3
        LcdDrawPicture(rssi36x8);
        return;
    }
    if (rssi < 85)
    { //rssi4
        LcdDrawPicture(rssi48x8);
        return;
    }
    if (rssi < 95)
    { //rssi5
        LcdDrawPicture(rssi510x8);
        return;
    }
    if (rssi < 105)
    { //rssi6
        LcdDrawPicture(rssi612x8);
        return;
    }
    //rssi7
    LcdDrawPicture(rssi714x8);
}
#endif //DOG_LCD_ENABLED


void SprintfSlaveInfo(SlaveInfoTable_t * slave, U8 * s)
{
    U8 tmp1;
    U8 tmp2;

    /* Valid values in Slave Info table. */
    if(slave->temperature != TEMP_INVALID_VALUE && slave->voltage != VOLTAGE_INVALID_VALUE)
    {
        tmp1 = slave->temperature;
//        tmp2 = (((slave->voltage & 0x1F) - 1) >> 1) + 17;
        tmp2 = (((slave->voltage & 0x1F)) >> 1) + 17;
      #ifdef __C51__  //if compiled with Keil
        sprintf(s, "S(%02bu): %s%2bu.%1bu C  %2bu.%1buV",
              slave->address.sfid,
              ((tmp1 < TEMP_ZERO_VALUE) ? "-" : "+"),
              ((tmp1 < TEMP_ZERO_VALUE) ? ((TEMP_ZERO_VALUE - tmp1)>>1) : ((tmp1 - TEMP_ZERO_VALUE)>>1)),
              ((tmp1 & 0x01) ? 5 : 0),
              (tmp2/10),
              (tmp2%10));
      #endif //__C51__
      #ifdef SDCC     //if compiled with SDCC
        sprintf(s, "S(%01u%01u): %s%1u%1u.%1u C      %1u%1uV",
              (slave->address.sfid)/10, (slave->address.sfid)%10,
              ((tmp1 < TEMP_ZERO_VALUE) ? "-" : "+"),
              ((tmp1 < TEMP_ZERO_VALUE) ? ((TEMP_ZERO_VALUE - tmp1)>>1)/10 : ((tmp1 - TEMP_ZERO_VALUE)>>1)/10),
              ((tmp1 < TEMP_ZERO_VALUE) ? ((TEMP_ZERO_VALUE - tmp1)>>1)%10 : ((tmp1 - TEMP_ZERO_VALUE)>>1)%10),
              ((tmp1 & 0x01) ? 5 : 0),
              (tmp2/10),
              (tmp2%10));
      #endif //SDCC
    }
    else
    {
      #ifdef __C51__  //if compiled with Keil
        sprintf(s, "S(%02bu): ----- C  ----V", slave->address.sfid);
      #endif //__C51__
      #ifdef SDCC     //if compiled with SDCC
        sprintf(s, "S(%01u%01u): ----- C  ----V", slave->address.sfid);
      #endif //SDCC
    }
    s[12] = 0x7F;   /* Degree sign on the LCD. */
}


void SprintfSlaveNotAssoc(U8 * s)
{
#ifdef __C51__  //if compiled with Keil
  sprintf(s, "S(--): not associated");
#endif //__C51__
#ifdef SDCC     //if compiled with SDCC
  sprintf(s, "S(--): not associated");
#endif //SDCC
}




