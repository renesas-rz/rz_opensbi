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
 * File Name    : cpg_iodefine.h
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
#ifndef CPG_IODEFINE_H
#define CPG_IODEFINE_H

#define CPG_BASE_ADDR   (0x11010000uL)                          /* Base address of CPG */
#define CPG_CLKON_SCIF      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x584uL))   /* Clock on/off register for SCIF  */
#define CPG_CLKMON_SCIF     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x704uL))   /* Clock monitor register for SCIF  */
#define CPG_RST_SCIF        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x884uL))   /* Reset on/off register for SCIF */
#define CPG_RSTMON_SCIF     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0xA04uL))   /* Reset monitor register for SCIF */

/*** CPG_CLKON ***/
#define CPG_CLKON_WEN_CH0       (0x00010000uL)                  /* Write-enable  to this register */

/*** Bit define of CPG_CLKON_SCIF/CPG_CLKMON_SCIF register ***/
#define CPG_CLKON_SCIF0_ON      (0x00000001uL)                  /* CLK0_ON */
#define CPG_CLKON_SCIF0_OFF     (0x00000000uL)

#define CPG_RST_SCIF0_OFF       (0x00000001uL)                  /* Reset off for SCIF0 */
#define CPG_RST_SCIF0_WEN       (0x00010000uL)                  /* Write-enable  UNIT0_RSTB bit for SCIF0 */
#define CPG_RSTMON_SCIF0_MSK    (0x00000001uL)
#define CPG_RSTMON_SCIF0_OFF    (0x00000000uL)                  /* Monitor reset off for SCIF0 */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* CPG_IODEFINE_H */
