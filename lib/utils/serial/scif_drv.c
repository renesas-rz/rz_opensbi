/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 */

/*
 * File Name    : scif_drv.c
 * Version      : 1.0
 * Description  : SCIF driver.
 */
/*
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 */

/*
 Includes   <System Includes> , "Project Includes"
 */
#include <stdint.h>
#include "scif_iodefine.h"
#include <sbi_utils/serial/scif_drv.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_timer.h>

/*
 Macro definitions
 */

/*
 Local Typedef definitions
 */

/*
 Exported global variables
 */

/*
 Private (static) variables and functions
 */
static void scif_wait(unsigned long);
static struct sbi_console_device scif_console = {
    .name         = "scif",
    .console_putc = scif_put_char,
};

/*
 * Function Name: scif_init
 * Description  : Initialize SCIF driver.
 * Arguments    : clk : BSS clock.
                  baudrate : baudrate.
 * Return Value : 0
 */
int scif_init(unsigned long clk, unsigned long baudrate)
{
	volatile uint16_t data16;

	SCIF_SCR_0 = SCIF_SCR_RCV_TRN_DIS;    /* Disable receive & transmit */
	SCIF_FCR_0 = SCIF_FCR_RST_ASSRT_TFRF; /* Reset assert tx-FIFO & rx-FIFO */

	data16 = SCIF_FSR_0;  /* Dummy read          */
	SCIF_FSR_0 = 0x0000U; /* Clear all error bit */

	data16 = SCIF_LSR_0;  /* Dummy read          */
	SCIF_LSR_0 = 0x0000U; /* Clear ORER bit      */

    
	SCIF_SCR_0 = 0x0000U; /* Select internal clock, SC_CLK pin unused for output pin */
    
	SCIF_SMR_0 = 0x0000U; /* Set asynchronous, 8bit data, no-parity, 1 stop and Po/1 */

	data16 = SCIF_SEMR_0;
	SCIF_SEMR_0 = data16 & (~SCIF_SEMR_MDDRS); /* Select to access BRR */
	SCIF_BRR_0 = SCBRR_VALUE(clk,baudrate);
	

	SCIF_SEMR_0 = (data16 | SCIF_SEMR_BRME | SCIF_SEMR_MDDRS); /* Select to access MDDR */
	SCIF_BRR_0 = 0xffU; /*  = 256*(115200*64/2*(17+1))/(100*10^6) = 0xff */

	scif_wait(baudrate);     /* wait */

	/* FTCR is left at initial value, because this interrupt isn't used. */

	SCIF_FCR_0 = SCIF_FCR_RST_NGATE_TFRF; /* Reset negate tx-FIFO, rx-FIFO. */

	SCIF_SCR_0 = SCIF_SCR_RCV_TRN_EN; /* Enable receive & transmit w/SC_CLK=no output */

	sbi_console_set_device(&scif_console);

	return 0;
}
/*
 * End of function scif_init
 */

/*
 * Function Name: scif_put_char
 * Description  : Put character via SCIF.
 * Arguments    : outChar : output character.
 * Return Value : 0: OK, -1: NG
 */
void scif_put_char(char outChar)
{
	uint16_t reg;

	while (!(SCIF_FSR_TXD_CHK & SCIF_FSR_0))
	{
		/* No Operation */
	}

	SCIF_FTDR_0 = outChar;
	reg = SCIF_FSR_0;
	reg &= (~SCIF_FSR_TXD_CHK); /* Clear TEND and TDFE flag */
	SCIF_FSR_0 = reg;
}
/*
 * End of function scif_put_char
 */

/*
 * Function Name: scif_wait
 * Description  : wait for timeout of specified period.
 * Arguments    : boudrate.
 * Return Value : none.
 */
static void scif_wait(unsigned long boudrate)
{
	unsigned long utime;
	
	utime = 1000000 / boudrate;
	utime += 1;
	
	sbi_timer_udelay(utime);
}
/*
 * End of function scif_wait
 */

/* End of File */

