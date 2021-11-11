/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Andes Technology Corporation
 *
 * Authors:
 *   Zong Li <zong@andestech.com>
 */

#ifndef _RZFIVE_PLMT_H_
#define _RZFIVE_PLMT_H_

int plmt_warm_timer_init(void);

int plmt_cold_timer_init(unsigned long base, u32 hart_count);

#endif /* _RZFIVE_PLMT_H_ */
