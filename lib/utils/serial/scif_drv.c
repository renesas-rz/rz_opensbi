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
 * File Name    : scif_drv.c
 * Version      : 1.0
 * Description  : SCIF driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include <sbi_utils/serial/iodefine.h>
#include <sbi_utils/serial/scif_iodefine.h>
#include <sbi_utils/serial/gpio_iodefine.h>
#include <sbi_utils/serial/cpg_iodefine.h>
#include <sbi_utils/serial/scif_drv.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_timer.h>

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static void scif_poweron(void);
static void scif_init_pinfunction(void);
static struct sbi_console_device scif_console = {
    .name         = "scif",
    .console_putc = scif_put_char,
};

/**********************************************************************************************************************
 * Function Name: scif_init
 * Description  : Initialize SCIF driver.
 * Arguments    : none.
 * Return Value : 0
 *********************************************************************************************************************/
int scif_init(void)
{
    volatile uint16_t data16;

    scif_poweron();

    SCIF_SCR_0 = SCIF_SCR_RCV_TRN_DIS;      /* Disable receive & transmit */
    SCIF_FCR_0 = SCIF_FCR_RST_ASSRT_TFRF;   /* Reset assert tx-FIFO & rx-FIFO */

    data16 = SCIF_FSR_0;        /* Dummy read           */
    SCIF_FSR_0 = 0x0000U;       /* Clear all error bit  */

    data16 = SCIF_LSR_0;        /* Dummy read           */
    SCIF_LSR_0 = 0x0000U;       /* Clear ORER bit       */

    SCIF_SCR_0 = 0x0000U;       /* Select internal clock, SC_CLK pin unused for output pin */
    SCIF_SMR_0 = 0x0000U;       /* Set asynchronous, 8bit data, no-parity, 1 stop and Po/1 */

    sbi_timer_udelay(100U);     /* wait for 100us */

    data16 = SCIF_SEMR_0;
    SCIF_SEMR_0 = data16 & (~SCIF_SEMR_MDDRS);                  /* Select to access BRR */
    SCIF_BRR_0 = 0x1AU;         /* Select P1(phi)(100MHz)/1, 115.2kbps*/
                                /* : N = 100/(64/2*115200)*10^6-1 = 26=> 0x1A */

    SCIF_SEMR_0 = (data16 | SCIF_SEMR_BRME | SCIF_SEMR_MDDRS);  /* Select to access MDDR */
    SCIF_BRR_0 = 0xffU;        /*  = 256*(115200*64/2*(17+1))/(100*10^6) = 0xff */

    sbi_timer_udelay(100U);     /* wait for 100us */

    /* FTCR is left at initial value, because this interrupt isn't used. */

    SCIF_FCR_0 = SCIF_FCR_RST_NGATE_TFRF;   /* Reset negate tx-FIFO, rx-FIFO. */

    scif_init_pinfunction();

    SCIF_SCR_0 = SCIF_SCR_RCV_TRN_EN;       /* Enable receive & transmit w/SC_CLK=no output */

    sbi_timer_udelay(100U);     /* wait for 100us */

    sbi_console_set_device(&scif_console);

    return 0;
}
/**********************************************************************************************************************
 * End of function scif_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: scif_put_char
 * Description  : Put character via SCIF.
 * Arguments    : outChar : output character.
 * Return Value : 0: OK, -1: NG
 *********************************************************************************************************************/
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
/**********************************************************************************************************************
 * End of function scif_put_char
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: scif_poweron
 * Description  : Power SCIF module on.
 * Arguments    : none.
 * Return Value : none.
 *********************************************************************************************************************/
static void scif_poweron(void)
{
    CPG_CLKON_SCIF = CPG_CLKON_WEN_CH0  | CPG_CLKON_SCIF0_ON;      /* Supply power to SPI multi0 module */
#ifdef _UT_
    CPG_CLKMON_SCIF = CPG_CLKON_SCIF0_OFF;
#endif
    while (CPG_CLKON_SCIF0_ON != (CPG_CLKMON_SCIF & CPG_CLKON_SCIF0_ON))
    {
        #ifdef _UT_
        CPG_CLKMON_SCIF = CPG_CLKON_SCIF0_ON;
        #endif
        /* No operation */
        /* Wait for supplying power to SCIF0 module */
    }


    CPG_RST_SCIF = (CPG_RST_SCIF0_WEN | CPG_RST_SCIF0_OFF);
    /* Reset off for SCIF0 */
    while (CPG_RSTMON_SCIF0_OFF != (CPG_RSTMON_SCIF & CPG_RSTMON_SCIF0_MSK))
    {
        #ifdef _UT_
        CPG_RSTMON_SCIF = CPG_RSTMON_SCIF0_OFF;
        #endif
        /* No operation */
        /* Wait for reset off SCIF0 module */
    }
}
/**********************************************************************************************************************
 * End of function scif_poweron
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: scif_init_pinfunction
 * Description  : Initialize pin function for SCIF.
 * Arguments    : none.
 * Return Value : none.
 *********************************************************************************************************************/
static void scif_init_pinfunction(void)
{
    GPIO_PWPR = GPIO_PWPR_B0WI_WEN;         /* Write-enable PWPR.PFCWE bit */
    GPIO_PWPR = GPIO_PWPR_PFCWE_WEN;        /* Write-enable PFC registers  */

	/*OTP:RZG2L,RZG2LC区別用(G2L:0,G2LC:1)*/
    if (OTPTMPA1_DEVICE_ID_FIVE_2 != (REG_OTP_OTPTMPA1 & OTPTMPA1_DEVICE_ID_MASK))
    {
        GPIO_PMC16 = GPIO_PMC16_SCIF0;          /* Select SCIF0 on P06_4 and P06_3 */
        GPIO_PFC16 = GPIO_PFC16_SCIF0;          /* Select RXD and TXD function */
    }
    else
    {
        GPIO_PMC18 = GPIO_PMC18_SCIF0;          /* Select SCIF0 on P13_1 and P13_0 */
        GPIO_PFC18 = GPIO_PFC18_SCIF0;          /* Select RXD and TXD function */
    }
#ifndef FPGA_BOARD
#else
    GPIO_PMC36 = GPIO_PMC36_SCIF0;          /* Select SCIF0 on P38_1 and P38_0 */
    GPIO_PFC36 = GPIO_PFC36_SCIF0;          /* Select RXD and TXD function */
#endif  /* FPGA_BOARD */
}
/**********************************************************************************************************************
 * End of function scif_init_pinfunction
 *********************************************************************************************************************/

/* End of File */

