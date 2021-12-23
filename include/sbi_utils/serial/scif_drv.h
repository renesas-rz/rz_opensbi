/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2021 Renesas Electronics Corporation
 */
/*
 * File Name    : scif_drv.h
 * Version      : 1.0
 * Description  : SCIF driver header file.
 */

/*
 Includes   <System Includes> , "Project Includes"
 */

/*
 Macro definitions
 */
#ifndef DRIVERS_SCIF_DRIVER_INC_SCIF_DRV_H_
#define DRIVERS_SCIF_DRIVER_INC_SCIF_DRV_H_

/*
 Global Typedef definitions
 */

/*
 External global variables
 */

/*
 Exported global functions
 */
int scif_init(unsigned long, unsigned long,unsigned long);
void scif_put_char(char outChar);

#endif /* DRIVERS_SCIF_DRIVER_INC_SCIF_DRV_H_ */
