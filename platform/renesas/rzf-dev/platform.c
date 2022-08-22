/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Andes Technology Corporation
 *
 * Authors:
 *   Zong Li <zong@andestech.com>
 *   Nylon Chen <nylon7@andestech.com>
 *
 * Copyright (c) 2021 Renesas Electronics Corporation
 */

#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_trap.h>
#include <sbi_utils/fdt/fdt_fixup.h>
#include <sbi_utils/irqchip/plic.h>
#include <sbi_utils/serial/uart8250.h>
#include <sbi_utils/serial/scif_drv.h>
#include "platform.h"
#include "plicsw.h"
#include "plmt.h"
#include "pma.h"
#include "cache.h"

static struct plic_data plic = {
	.addr = RZF_PLIC_ADDR,
	.num_src = RZF_PLIC_NUM_SOURCES,
};

static void sbi_clear_mmiscctl_msa(void)
{
	unsigned long mmisc_ctl;

	mmisc_ctl = csr_read(CSR_MMISCCTL);
	mmisc_ctl &= ~(V5_MMISC_CTL_MSA_OR_UNA_EN);
	csr_write(CSR_MMISCCTL, mmisc_ctl);
}

/* Platform final initialization. */
static int rzf_final_init(bool cold_boot)
{
	void *fdt;

	/* enable L1 cache */
	uintptr_t mcache_ctl_val = csr_read(CSR_MCACHECTL);

	if (!(mcache_ctl_val & V5_MCACHE_CTL_IC_EN))
		mcache_ctl_val |= V5_MCACHE_CTL_IC_EN;
	if (!(mcache_ctl_val & V5_MCACHE_CTL_DC_EN))
		mcache_ctl_val |= V5_MCACHE_CTL_DC_EN;
	if (!(mcache_ctl_val & V5_MCACHE_CTL_CCTL_SUEN))
		mcache_ctl_val |= V5_MCACHE_CTL_CCTL_SUEN;
	if (!(mcache_ctl_val & V5_MCACHE_CTL_L1I_PREFETCH_EN))
		mcache_ctl_val |= V5_MCACHE_CTL_L1I_PREFETCH_EN;
	if (!(mcache_ctl_val & V5_MCACHE_CTL_L1D_PREFETCH_EN))
		mcache_ctl_val |= V5_MCACHE_CTL_L1D_PREFETCH_EN;
	mcache_ctl_val &= ~(V5_MCACHE_CTL_DC_WAROUND_1_EN | V5_MCACHE_CTL_DC_WAROUND_2_EN);
	mcache_ctl_val |= V5_MCACHE_CTL_DC_WAROUND_1_EN;
	csr_write(CSR_MCACHECTL, mcache_ctl_val);

	uintptr_t mmisc_ctl_val = csr_read(CSR_MMISCCTL);
	if (!(mmisc_ctl_val & V5_MMISC_CTL_NON_BLOCKING_EN))
		mmisc_ctl_val |= V5_MMISC_CTL_NON_BLOCKING_EN;
	csr_write(CSR_MMISCCTL, mmisc_ctl_val);

	/* enable L2 cache */
	uint32_t *l2c_ctl_base = (void *)RZF_L2C_ADDR + V5_L2C_CTL_OFFSET;
	uint32_t l2c_ctl_val = *l2c_ctl_base;

	if (!(l2c_ctl_val & V5_L2C_CTL_ENABLE_MASK))
		l2c_ctl_val |= V5_L2C_CTL_ENABLE_MASK;
	*l2c_ctl_base = l2c_ctl_val;

	if (!cold_boot)
		return 0;

	fdt = sbi_scratch_thishart_arg1_ptr();
	fdt_fixups(fdt);

	init_pma();
	return 0;
}

/* Initialize the platform console. */
static int rzf_console_init(void)
{
	return scif_init(RZF_SCIF_ADDR,RZF_SCIF_FREQUENCY,RZF_SCIF_BAUDRATE);
}

/* Initialize the platform interrupt controller for current HART. */
static int rzf_irqchip_init(bool cold_boot)
{
	u32 hartid = current_hartid();
	int ret;

	if (cold_boot) {
		ret = plic_cold_irqchip_init(&plic);
		if (ret)
			return ret;
	}

	return plic_warm_irqchip_init(&plic, 2 * hartid, 2 * hartid + 1);
}

/* Platform early initialization. */
static int rzf_early_init(bool cold_boot)
{
	int ret=0;

	sbi_clear_mmiscctl_msa();

	if (cold_boot) {
		ret = plmt_cold_timer_init(RZF_PLMT_ADDR,
					   RZF_HART_COUNT);
	}
	return ret;
}

/* Initialize platform timer for current HART. */
static int rzf_timer_init(bool cold_boot)
{
	return plmt_warm_timer_init();
}

/* Vendor-Specific SBI handler */
static int rzf_vendor_ext_provider(long extid, long funcid,
	const struct sbi_trap_regs *regs, unsigned long *out_value,
	struct sbi_trap_info *out_trap)
{
	int ret = 0;
	switch (funcid) {
	case SBI_EXT_ANDES_GET_MCACHE_CTL_STATUS:
		*out_value = csr_read(CSR_MCACHECTL);
		break;
	case SBI_EXT_ANDES_GET_MMISC_CTL_STATUS:
		*out_value = csr_read(CSR_MMISCCTL);
		break;
	case SBI_EXT_ANDES_SET_MCACHE_CTL:
		ret = mcall_set_mcache_ctl(regs->a0);
		break;
	case SBI_EXT_ANDES_SET_MMISC_CTL:
		ret = mcall_set_mmisc_ctl(regs->a0);
		break;
	case SBI_EXT_ANDES_ICACHE_OP:
		ret = mcall_icache_op(regs->a0);
		break;
	case SBI_EXT_ANDES_DCACHE_OP:
		ret = mcall_dcache_op(regs->a0);
		break;
	case SBI_EXT_ANDES_L1CACHE_I_PREFETCH:
		ret = mcall_l1_cache_i_prefetch_op(regs->a0);
		break;
	case SBI_EXT_ANDES_L1CACHE_D_PREFETCH:
		ret = mcall_l1_cache_d_prefetch_op(regs->a0);
		break;
	case SBI_EXT_ANDES_NON_BLOCKING_LOAD_STORE:
		ret = mcall_non_blocking_load_store(regs->a0);
		break;
	case SBI_EXT_ANDES_WRITE_AROUND:
		ret = mcall_write_around(regs->a0);
		break;
	case SBI_EXT_ANDES_SET_PMA:
		*out_value = mcall_set_pma(regs->a0, regs->a1, regs->a2, regs->a3);
		break;
	case SBI_EXT_ANDES_FREE_PMA:
		mcall_free_pma(regs->a0);
		break;
	case SBI_EXT_ANDES_PROBE_PMA:
		*out_value = mcall_prob_pma();
		break;
	case SBI_EXT_ANDES_DCACHE_WBINVAL_ALL:
		ret = mcall_dcache_wbinval_all();
		break;
	case SBI_EXT_ANDES_GET_MICM_CTL_STATUS:
		*out_value = csr_read(CSR_MICM_CFG);
		break;
	case SBI_EXT_ANDES_GET_MDCM_CTL_STATUS:
		*out_value = csr_read(CSR_MDCM_CFG);
		break;
	case SBI_EXT_ANDES_GET_MMSC_CTL_STATUS:
		*out_value = csr_read(CSR_MMSC_CFG);
		break;
	case SBI_EXT_ANDES_GET_MISA_CTL_STATUS:
		*out_value = csr_read(CSR_MISA_CFG);
		break;
	default:
		sbi_printf("Unsupported vendor sbi call : %ld\n", funcid);
		asm volatile("ebreak");
	}
	return ret;
}

/* Platform descriptor. */
const struct sbi_platform_operations platform_ops = {
	.final_init = rzf_final_init,

	.console_init = rzf_console_init,

	.irqchip_init = rzf_irqchip_init,

	.early_init = rzf_early_init,

	.timer_init = rzf_timer_init,

	.vendor_ext_provider = rzf_vendor_ext_provider
};

const struct sbi_platform platform = {
	.opensbi_version = OPENSBI_VERSION,
	.platform_version = SBI_PLATFORM_VERSION(0x0, 0x01),
	.name = "Renesas RZ/Five",
	.features = SBI_PLATFORM_DEFAULT_FEATURES,
	.hart_count = RZF_HART_COUNT,
	.hart_stack_size = SBI_PLATFORM_DEFAULT_HART_STACK_SIZE,
	.platform_ops_addr = (unsigned long)&platform_ops
};
