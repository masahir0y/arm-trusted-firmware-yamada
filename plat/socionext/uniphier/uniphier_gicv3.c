/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <gicv3.h>
#include <plat_uniphier.h>
#include <platform.h>
#include <platform_def.h>

#define UNIPHIER_GICD_BASE			0x5fe00000
#define UNIPHIER_LD11_GICR_BASE			0x5fe40000
#define UNIPHIER_LD20_GICR_BASE			0x5fe80000
#define UNIPHIER_PXS3_GICR_BASE			0x5fe80000

#define UNIPHIER_GICD_BASE

#define ARM_IRQ_SEC_PHY_TIMER		29

#define ARM_IRQ_SEC_SGI_0		8
#define ARM_IRQ_SEC_SGI_1		9
#define ARM_IRQ_SEC_SGI_2		10
#define ARM_IRQ_SEC_SGI_3		11
#define ARM_IRQ_SEC_SGI_4		12
#define ARM_IRQ_SEC_SGI_5		13
#define ARM_IRQ_SEC_SGI_6		14
#define ARM_IRQ_SEC_SGI_7		15

/* The GICv3 driver only needs to be initialized in EL3 */
static uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];

/* Array of Group1 secure interrupts to be configured by the gic driver */
static const unsigned int g1s_interrupt_array[] = {
	ARM_IRQ_SEC_PHY_TIMER,
	ARM_IRQ_SEC_SGI_1,
	ARM_IRQ_SEC_SGI_2,
	ARM_IRQ_SEC_SGI_3,
	ARM_IRQ_SEC_SGI_4,
	ARM_IRQ_SEC_SGI_5,
	ARM_IRQ_SEC_SGI_7,
};

/* Array of Group0 interrupts to be configured by the gic driver */
static const unsigned int g0_interrupt_array[] = {
	ARM_IRQ_SEC_SGI_0,
	ARM_IRQ_SEC_SGI_6,
};

static unsigned int uniphier_mpidr_to_core_pos(u_register_t mpidr)
{
	return plat_core_pos_by_mpidr(mpidr);
}

static const struct gicv3_driver_data uniphier_gic_driver_data[] = {
	[UNIPHIER_SOC_LD11] = {
		.gicd_base = 0x5fe00000,
		.gicr_base = 0x5fe40000,
		.g0_interrupt_num = ARRAY_SIZE(g0_interrupt_array),
		.g1s_interrupt_num = ARRAY_SIZE(g1s_interrupt_array),
		.g0_interrupt_array = g0_interrupt_array,
		.g1s_interrupt_array = g1s_interrupt_array,
		.rdistif_num = PLATFORM_CORE_COUNT,
		.rdistif_base_addrs = rdistif_base_addrs,
		.mpidr_to_core_pos = uniphier_mpidr_to_core_pos,
	},
	[UNIPHIER_SOC_LD20] = {
		.gicd_base = 0x5fe00000,
		.gicr_base = 0x5fe80000,
		.g0_interrupt_num = ARRAY_SIZE(g0_interrupt_array),
		.g1s_interrupt_num = ARRAY_SIZE(g1s_interrupt_array),
		.g0_interrupt_array = g0_interrupt_array,
		.g1s_interrupt_array = g1s_interrupt_array,
		.rdistif_num = PLATFORM_CORE_COUNT,
		.rdistif_base_addrs = rdistif_base_addrs,
		.mpidr_to_core_pos = uniphier_mpidr_to_core_pos,
	},
	[UNIPHIER_SOC_PXS3] = {
		.gicd_base = 0x5fe00000,
		.gicr_base = 0x5fe80000,
		.g0_interrupt_num = ARRAY_SIZE(g0_interrupt_array),
		.g1s_interrupt_num = ARRAY_SIZE(g1s_interrupt_array),
		.g0_interrupt_array = g0_interrupt_array,
		.g1s_interrupt_array = g1s_interrupt_array,
		.rdistif_num = PLATFORM_CORE_COUNT,
		.rdistif_base_addrs = rdistif_base_addrs,
		.mpidr_to_core_pos = uniphier_mpidr_to_core_pos,
	},
};

void uniphier_gic_driver_init(unsigned int soc)
{
	assert(soc < ARRAY_SIZE(uniphier_gic_driver_data));

	gicv3_driver_init(&uniphier_gic_driver_data[soc]);
}

/******************************************************************************
 * common helper to initialize the GIC. Only invoked by BL31
 *****************************************************************************/
void uniphier_gic_init(void)
{
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
}

/******************************************************************************
 * common helper to enable the GIC CPU interface
 *****************************************************************************/
void plat_arm_gic_cpuif_enable(void)
{
	gicv3_cpuif_enable(plat_my_core_pos());
}

/******************************************************************************
 * common helper to disable the GIC CPU interface
 *****************************************************************************/
void plat_uniphier_gic_cpuif_disable(void)
{
	gicv3_cpuif_disable(plat_my_core_pos());
}

/******************************************************************************
 * common helper to initialize the per-cpu redistributor interface in GICv3
 *****************************************************************************/
void plat_uniphier_gic_pcpu_init(void)
{
	gicv3_rdistif_init(plat_my_core_pos());
}
