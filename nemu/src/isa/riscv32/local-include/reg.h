/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#ifndef __RISCV_REG_H__
#define __RISCV_REG_H__

#include <common.h>

static inline int check_reg_idx(int idx)
{
    IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < MUXDEF(CONFIG_RVE, 16, 32)));
    return idx;
}

#define gpr(idx) (cpu.gpr[check_reg_idx(idx)])
#define csr(addr) (cpu.csr[map_addr2idx(addr)])

static inline const char *reg_name(int idx)
{
    extern const char *regs[];
    return regs[check_reg_idx(idx)];
}

static inline const int map_addr2idx(word_t addr)
{
    if (addr == 0x342) return 0;
    else if (addr == 0x300) return 1;
    else if (addr == 0x341) return 2;
    else if (addr == 0x305) return 3;
    else if (addr == 0x180) return 4;
    else 
    {
        printf("0x%x\n", addr);
        printf("INVALID CSR !\n");
        abort();
    }
}

#endif
