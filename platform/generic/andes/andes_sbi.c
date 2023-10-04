// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2023 Renesas Electronics Corp.
 *
 */
#include <andes/andes.h>
#include <andes/andes_sbi.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_error.h>

enum sbi_ext_andes_fid {
	SBI_EXT_ANDES_FID0 = 0, /* Reserved for future use */
	SBI_EXT_ANDES_IOCP_SW_WORKAROUND,
	SBI_EXT_RENESAS_RZFIVE_ETH_WORKAROUND,
};

static bool andes_cache_controllable(void)
{
	return (((csr_read(CSR_MICM_CFG) & MICM_CFG_ISZ_MASK) ||
		 (csr_read(CSR_MDCM_CFG) & MDCM_CFG_DSZ_MASK)) &&
		(csr_read(CSR_MMSC_CFG) & MMSC_CFG_CCTLCSR_MASK) &&
		(csr_read(CSR_MCACHE_CTL) & MCACHE_CTL_CCTL_SUEN_MASK) &&
		misa_extension('U'));
}

static bool andes_iocp_disabled(void)
{
	return (csr_read(CSR_MMSC_CFG) & MMSC_IOCP_MASK) ? false : true;
}

static bool andes_apply_iocp_sw_workaround(void)
{
	return andes_cache_controllable() & andes_iocp_disabled();
}

int andes_sbi_vendor_ext_provider(long funcid,
				  struct sbi_trap_regs *regs,
				  struct sbi_ecall_return *out,
				  const struct fdt_match *match)
{
	switch (funcid) {
	case SBI_EXT_ANDES_IOCP_SW_WORKAROUND:
		out->value = andes_apply_iocp_sw_workaround();
		break;

	case SBI_EXT_RENESAS_RZFIVE_ETH_WORKAROUND: {
		uintptr_t mcache_ctl_val = csr_read(0x7ca);
		u8 status = (u8)regs->a0;

		if (status)
			mcache_ctl_val |= BIT(1);
		else
			mcache_ctl_val &= ~BIT(1);
		csr_write(0x7cc, 6);
		csr_write(0x7ca, mcache_ctl_val);
		if (status) {
			uint32_t *l2c_ctl_base = (void *)0x13400008;
			uint32_t l2c_ctl_val = *l2c_ctl_base;
			l2c_ctl_val |= 0x1;
			*l2c_ctl_base = l2c_ctl_val;
			l2c_ctl_val = *l2c_ctl_base;
			while ((l2c_ctl_val & BIT(14)))
				l2c_ctl_val = *l2c_ctl_base;
		}
		break;
	}

	default:
		return SBI_EINVAL;
	}

	return 0;
}
