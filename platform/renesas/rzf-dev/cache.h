/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 */

uintptr_t mcall_set_mcache_ctl(unsigned long input);
uintptr_t mcall_set_mmisc_ctl(unsigned long input);
uintptr_t mcall_icache_op(unsigned int enable);
uintptr_t mcall_dcache_op(unsigned int enable);
uintptr_t mcall_l1_cache_i_prefetch_op(unsigned long enable);
uintptr_t mcall_l1_cache_d_prefetch_op(unsigned long enable);
uintptr_t mcall_non_blocking_load_store(unsigned long enable);
uintptr_t mcall_write_around(unsigned long enable);
