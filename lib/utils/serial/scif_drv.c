/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2021 Renesas Electronics Corporation
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
#include <sbi_utils/serial/scif_drv.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_timer.h>
#include <sbi/riscv_io.h>

/*
 Macro definitions
 */
#define SCIF_SMR_0_OFFSET    0x000U    /* Serial mode register */
#define SCIF_BRR_0_OFFSET    0x002U    /* Bit rate/Modulation duty register */
#define SCIF_SCR_0_OFFSET    0x004U    /* Serial Control register */
#define SCIF_FTDR_0_OFFSET   0x006U    /* Transmit FIFO data register */
#define SCIF_FSR_0_OFFSET    0x008U    /* Serial Status register */
#define SCIF_FRDR_0_OFFSET   0x00AU    /* Receive FIFO data register */
#define SCIF_FCR_0_OFFSET    0x00CU    /* FIFO control register */
#define SCIF_FDR_0_OFFSET    0x00EU    /* FIFO data count register */
#define SCIF_SPTR_0_OFFSET   0x010U    /* Serial port register */
#define SCIF_LSR_0_OFFSET    0x012U    /* Line status register */
#define SCIF_SEMR_0_OFFSET   0x014U    /* Serial Extended mode register */
#define SCIF_FTCR_0_OFFSET   0x016U    /* FIFO trigger control register */


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

#define SCIF_SIZE(offset) ((offset==SCIF_BRR_0_OFFSET)|| \
							(offset==SCIF_FTDR_0_OFFSET)|| \
							(offset==SCIF_FRDR_0_OFFSET)|| \
							(offset==SCIF_SEMR_0_OFFSET))

#define SCBRR_VALUE(clk,baudrate) (clk/(64/2*baudrate)-1)

/*
 Local Typedef definitions
 */

/*
 Exported global variables
 */
static volatile void *scif_base;
static u32 scif_clk;
static u32 scif_baudrate;

/*
 Private (static) variables and functions
 */
static void scif_wait(unsigned long);
static struct sbi_console_device scif_console = {
    .name         = "scif",
    .console_putc = scif_put_char,
};

static u32 get_reg(u32 offset)
{
	if(SCIF_SIZE(offset)){
		return readb(scif_base + offset);
	}else{
		return readw(scif_base + offset);
	}
}

static void set_reg(u32 offset, u32 val)
{
	if(SCIF_SIZE(offset)){
		return writeb(val,scif_base + offset);
	}else{
		return writew(val,scif_base + offset);
	}
}

/*
 * Function Name: scif_init
 * Description  : Initialize SCIF driver.
 * Arguments    : clk : BSS clock.
                  baudrate : baudrate.
 * Return Value : 0
 */
int scif_init(unsigned long base,unsigned long clk, unsigned long baudrate)
{
	volatile uint16_t data16;
	
	scif_base = (volatile void *)base;
	scif_clk = clk;
	scif_baudrate = baudrate;
	
	set_reg(SCIF_SCR_0_OFFSET,SCIF_SCR_RCV_TRN_DIS);    /* Disable receive & transmit */
	set_reg(SCIF_FCR_0_OFFSET,SCIF_FCR_RST_ASSRT_TFRF); /* Reset assert tx-FIFO & rx-FIFO */

	data16 = get_reg(SCIF_FSR_0_OFFSET);  /* Dummy read          */
	set_reg(SCIF_FSR_0_OFFSET,0x0000U); /* Clear all error bit */

	data16 = get_reg(SCIF_LSR_0_OFFSET);  /* Dummy read          */
	set_reg(SCIF_LSR_0_OFFSET,0x0000U); /* Clear ORER bit      */

    
	set_reg(SCIF_SCR_0_OFFSET,0x0000U); /* Select internal clock, SC_CLK pin unused for output pin */
    
	set_reg(SCIF_SMR_0_OFFSET,0x0000U); /* Set asynchronous, 8bit data, no-parity, 1 stop and Po/1 */

	data16 = get_reg(SCIF_SEMR_0_OFFSET);
	set_reg(SCIF_SEMR_0_OFFSET,data16 & (~SCIF_SEMR_MDDRS)); /* Select to access BRR */
	set_reg(SCIF_BRR_0_OFFSET, SCBRR_VALUE(scif_clk,scif_baudrate));

	scif_wait(baudrate);     /* wait */

	/* FTCR is left at initial value, because this interrupt isn't used. */

	set_reg(SCIF_FCR_0_OFFSET,SCIF_FCR_RST_NGATE_TFRF); /* Reset negate tx-FIFO, rx-FIFO. */

	set_reg(SCIF_SCR_0_OFFSET,SCIF_SCR_RCV_TRN_EN); /* Enable receive & transmit w/SC_CLK=no output */

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

	while (!(SCIF_FSR_TXD_CHK & get_reg(SCIF_FSR_0_OFFSET)))
	{
		/* No Operation */
	}

	set_reg(SCIF_FTDR_0_OFFSET,outChar);
	reg = get_reg(SCIF_FSR_0_OFFSET);
	reg &= (~SCIF_FSR_TXD_CHK); /* Clear TEND and TDFE flag */
	set_reg(SCIF_FSR_0_OFFSET,reg);
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

