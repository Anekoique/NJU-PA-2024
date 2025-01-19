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

#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

int isa_mmu_check(vaddr_t vaddr, int len, int type)
{
    if (cpu.csr[4] != 0) 
        return MMU_TRANSLATE;
    else return MMU_DIRECT;
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type)
{
    printf("gethere\n");
    uintptr_t offset = (uintptr_t)vaddr & 0xfff;
    uintptr_t vpn[2];
    vpn[0] = ((uintptr_t)vaddr >> 12) & 0x3ff;
    vpn[1] = ((uintptr_t)vaddr >> 22) & 0x3ff;

    uintptr_t *pte = (uintptr_t *)(((uintptr_t)cpu.csr[4] << 12) + vpn[1] * 4);
    printf("%x\n", cpu.csr[4]);
    uintptr_t pt = (*pte & 0xfffffc00) << 2;
    printf("here\n");
    uintptr_t *leaf_pte = (uintptr_t *)(pt + vpn[0] * 4);
    uintptr_t pa = ((*leaf_pte & 0xfffffc00) << 2) + offset;
    assert(vaddr == pa);
    printf("here\n");
    return pa;
}

