/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 */
 /*
 * File Name    : scif_iodefine.h
 * Version      : 1.0
 * Description  : iodefine header file.
 */
 /*
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 */

 /*
 Includes   <System Includes> , "Project Includes"
 */
#include <stdint.h>

 /*
 Macro definitions
 */
#ifndef INC_IODEFINE_SCIF_IODEFINE_H_
#define INC_IODEFINE_SCIF_IODEFINE_H_

#define _SCIF_BASE_     (0x01004B800UL)

#define SCIF_SMR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x000U ))    /* Serial mode register */
#define SCIF_BRR_0      (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x002U ))    /* Bit rate/Modulation duty register */
#define SCIF_SCR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x004U ))    /* Serial Control register */
#define SCIF_FTDR_0     (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x006U ))    /* Transmit FIFO data register */
#define SCIF_FSR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x008U ))    /* Serial Status register */
#define SCIF_FRDR_0     (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x00AU ))    /* Receive FIFO data register */
#define SCIF_FCR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x00CU ))    /* FIFO control register */
#define SCIF_FDR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x00EU ))    /* FIFO data count register */
#define SCIF_SPTR_0     (*(volatile uint16_t *)( _SCIF_BASE_ + 0x010U ))    /* Serial port register */
#define SCIF_LSR_0      (*(volatile uint16_t *)( _SCIF_BASE_ + 0x012U ))    /* Line status register */
#define SCIF_SEMR_0     (*(volatile  uint8_t *)( _SCIF_BASE_ + 0x014U ))    /* Serial Extended mode register */
#define SCIF_FTCR_0     (*(volatile uint16_t *)( _SCIF_BASE_ + 0x016U ))    /* FIFO trigger control register */


#define SCIF_RFRST    (0x0002U)    /* Reset assert receive-FIFO (bit[1])   */
#define SCIF_TFRST    (0x0004U)    /* Reset assert transmit-FIFO(bit[2])   */

#define SCIF_FCR_RST_ASSRT_TFRF (SCIF_RFRST | SCIF_TFRST)   /* Reset assert tx-FIFO & rx-FIFO   */
#define SCIF_FCR_RST_NGATE_TFRF (0x0000U)                   /* Reset negate tx-FIFO & rx-FIFO   */


#define SCIF_RE       (0x0010U)    /* Enable receive (bit[4])  */
#define SCIF_TE       (0x0020U)    /* Enable transmit(bit[5])  */
#define SCIF_SCR_RCV_TRN_EN     (SCIF_RE | SCIF_TE)         /* Enable receive & transmit w/SC_CLK=no output */
#define SCIF_SCR_RCV_TRN_DIS    (0x0000U)                   /* Disable receive & transmit    */

#define SCIF_FSR_ER   (0x0080U)    /* Receive error flag */
#define SCIF_FSR_TEND (0x0040U)    /* Detect break flag */
#define SCIF_FSR_TDFE (0x0020U)    /* Detect break flag */
#define SCIF_FSR_BRK  (0x0010U)    /* Detect break flag */
#define SCIF_FSR_RDF  (0x0002U)    /* Receive FIFO data full flag */
#define SCIF_FSR_DR   (0x0001U)    /* Receive data ready flag */

#define SCIF_FSR_RXD_CHK    (SCIF_FSR_ER | SCIF_FSR_BRK | SCIF_FSR_DR)
#define SCIF_FSR_TXD_CHK    (SCIF_FSR_TEND | SCIF_FSR_TDFE)

#define SCIF_LSR_ORER       (0x0001U)    /* Overrun error flag */


#define SCIF_SPTR_SPB2DT    (0x0001U)    /* if SCR.TE setting, don't care */
#define SCIF_SPTR_SPB2IO    (0x0002U)    /* if SCR.TE setting, don't care */

#define SCIF_SEMR_BRME      (0x20U)      /* bit-rate modulation enable */
#define SCIF_SEMR_MDDRS     (0x10U)      /* MDDR access enable */


/*** Refer gpio_iodefine.h ***/
#define PFC_PFS_SCK0        (0x01U)      /* Select SCK0 function w/o interrupt   */
#define PFC_PFS_RXD0        (0x01U)      /* Select RXD0 function w/o interrupt   */
#define PFC_PFS_TXD0        (0x01U)      /* Select TXD0 function w/o interrupt   */

 /*
 Global Typedef definitions
 */

 /*
 External global variables
 */

 /*
 Exported global functions
 */

#endif /* INC_IODEFINE_SCIF_IODEFINE_H_ */
