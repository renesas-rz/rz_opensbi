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
 * File Name    : scif_iodefine.h
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
#include <stdint.h>

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef INC_IODEFINE_SCIF_IODEFINE_H_
#define INC_IODEFINE_SCIF_IODEFINE_H_

/* SCIF */
#define _SCIF_BASE_     (0x01004B800UL)

#define SCIF_SMR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x000U ))    /* Serial mode register */
#define SCIF_BRR_0      (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x002U ))    /* Bit rate/Modulation duty register */
#define SCIF_SCR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x004U ))    /* Serial Control register */
#define SCIF_FTDR_0     (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x006U ))    /* Transmit FIFO data register */
#define SCIF_FSR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x008U ))    /* Serial Status register */
#define SCIF_FCR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x00CU ))    /* FIFO control register */
#define SCIF_LSR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x012U ))    /* Line status register */
#define SCIF_SEMR_0     (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x014U ))    /* Serial Extended mode register */


#define SCIF_RFRST              (0x0002U)                   /* Reset assert receive-FIFO (bit[1])   */
#define SCIF_TFRST              (0x0004U)                   /* Reset assert transmit-FIFO(bit[2])   */

#define SCIF_FCR_RST_ASSRT_TFRF (SCIF_RFRST | SCIF_TFRST)   /* Reset assert tx-FIFO & rx-FIFO   */
#define SCIF_FCR_RST_NGATE_TFRF (0x0000U)                   /* Reset negate tx-FIFO & rx-FIFO   */


#define SCIF_RE                 (0x0010U)                   /* Enable receive (bit[4])  */
#define SCIF_TE                 (0x0020U)                   /* Enable transmit(bit[5])  */
#define SCIF_SCR_RCV_TRN_EN     (SCIF_RE | SCIF_TE)         /* Enable receive & transmit w/SC_CLK=no output */
#define SCIF_SCR_RCV_TRN_DIS    (0x0000U)                   /* Disable receive & transmit    */

#define SCIF_FSR_ER             (0x0080U)                   /* Receive error flag */
#define SCIF_FSR_TEND           (0x0040U)                   /* Detect break flag */
#define SCIF_FSR_TDFE           (0x0020U)                   /* Detect break flag */
#define SCIF_FSR_BRK            (0x0010U)                   /* Detect break flag */
#define SCIF_FSR_DR             (0x0001U)                   /* Receive data ready flag */

#define SCIF_FSR_TXD_CHK        (SCIF_FSR_TEND | SCIF_FSR_TDFE)

#define SCIF_SEMR_BRME          (0x20U)                     /* bit-rate modulation enable */
#define SCIF_SEMR_MDDRS         (0x10U)                     /* MDDR access enable */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* INC_IODEFINE_SCIF_IODEFINE_H_ */
