/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : gpio_iodefine.h
 * Version      : 1.0
 * Description  : iodefine header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef GPIO_IODEFINE_H
#define GPIO_IODEFINE_H

#define USE_SCIF0_RTS_CST       /* Used SCIF0_RTS and SCIF0_CST pin */
/* #define USE_SD0_CD_SD0_WP */ /* Used SD0_CD and SD0_WP pin */

#define _GPIO_BASE_  (0x11030000uL)

#define GPIO_PMC16  (*(volatile uint8_t  *)(_GPIO_BASE_ + 0x0216uL))    /* PMC for SCIF0 w/P06_4, P13_3 ,P13_2, P13_1 and P06_0  */
#define GPIO_PMC18  (*(volatile uint8_t  *)(_GPIO_BASE_ + 0x0218uL))    /* PMC for SCIF0 w/P08_4, P08_3 ,P08_2, P08_1 and P08_0  */
#define GPIO_PMC36  (*(volatile uint8_t  *)(_GPIO_BASE_ + 0x0236uL))    /* PMC for SCIF0 w/P38_1 and P38_0  */
#define GPIO_PFC16  (*(volatile uint32_t *)(_GPIO_BASE_ + 0x0458uL))    /* PFC for SCIF0 w/P06_4, P06_3 ,P06_2, P06_1 and P06_0  */
#define GPIO_PFC18  (*(volatile uint32_t *)(_GPIO_BASE_ + 0x0460uL))    /* PFC for SCIF0 w/P08_4, P08_3 ,P08_2, P08_1 and P08_0  */
#define GPIO_PFC36  (*(volatile uint32_t *)(_GPIO_BASE_ + 0x04D8uL))    /* PFC for SCIF0 w/P38_1 and P38_0  */

#define GPIO_PWPR   (*(volatile uint32_t *)(_GPIO_BASE_ + 0x3014uL))    /* Write-protect register for PFC */

/*** Define bit pattern of PMC for SCIF0 ***/
#define GPIO_PMC36_SCIF0        (0x03u)             /* Bit[1:0]=B'11:  SCIF0_RXD and SCIF0_TXD */
#define GPIO_PMC16_SCIF0        (0x18u)             /* Bit[4:3]=B'11:  SCIF0_RXD and SCIF0_TXD */
#define GPIO_PMC18_SCIF0        (0x06u)             /* Bit[2:1]=B'11:  SCIF0_RXD and SCIF0_TXD */

/*** Define bit pattern of PFC for SCIF0 ***/
#define GPIO_PFC36_SCIF0        (0x00000011u)       /* Bit[6:4]=B'001: SCIF0_RXD, Bit[2:0]=B'001ÅF SCIF0_TXD */
#define GPIO_PFC16_SCIF0        (0x00066000u)       /* Bit[18:16]=B'110: SCIF0_TXD, Bit[14:12]=B'110: SCIF0_RXD */
#define GPIO_PFC18_SCIF0        (0x00000220u)       /* Bit[10:8]=B'010: SCIF0_TXD, Bit[6:4]=B'010: SCIF0_RXD */

/*** Define bit pattern of GPIO_PWPR ***/
#define GPIO_PWPR_B0WI_WEN      (0x00000000uL)      /* Write-enable to PFCWE bit */

#define GPIO_PWPR_PFCWE_WEN     (0x00000040uL)      /* Write-enable to PFC */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* GPIO_IODEFINE_H */
