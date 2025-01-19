#include <am.h>
#include <klib.h>
#include <nemu.h>

static AddrSpace kas = {};
static void *(*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void *) = NULL;
static int vme_enable = 0;

static Area segments[] = { // Kernel memory mappings
    NEMU_PADDR_SPACE};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir)
{
    uintptr_t mode = 1ul << (__riscv_xlen - 1);
    asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp()
{
    uintptr_t satp;
    asm volatile("csrr %0, satp" : "=r"(satp));
    return satp << 12;
}

bool vme_init(void *(*pgalloc_f)(int), void (*pgfree_f)(void *))
{
    pgalloc_usr = pgalloc_f;
    pgfree_usr = pgfree_f;

    kas.ptr = pgalloc_f(PGSIZE);

    int i;
    for (i = 0; i < LENGTH(segments); i++)
    {
        void *va = segments[i].start;
        for (; va < segments[i].end; va += PGSIZE)
        {
            map(&kas, va, va, 0);
        }
    }

    set_satp(kas.ptr);
    vme_enable = 1;

    return true;
}

void protect(AddrSpace *as)
{
    PTE *updir = (PTE *)(pgalloc_usr(PGSIZE));
    as->ptr = updir;
    as->area = USER_SPACE;
    as->pgsize = PGSIZE;
    // map kernel space
    memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as)
{
}

void __am_get_cur_as(Context *c)
{
    c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c)
{
    if (vme_enable && c->pdir != NULL)
    {
        set_satp(c->pdir);
    }
}

void map(AddrSpace *as, void *va, void *pa, int prot)
{
    uintptr_t offset = (uintptr_t)va & 0xfff;
    uintptr_t vpn[2];
    vpn[0] = ((uintptr_t)va >> 12) & 0x3ff;
    vpn[1] = ((uintptr_t)va >> 22) & 0x3ff;

    PTE *pte = (PTE *)(((uintptr_t)as->ptr) + vpn[1] * 4);
    uintptr_t pt;
    if ((*pte & 0x1) == 0)
    {
        pt = (uintptr_t)pgalloc_usr(PGSIZE);
        *pte = (((uintptr_t)pt >> 12) << 10) | 0x3ff;
    }
    else 
    {
        pt = (*pte & 0xfffffc00) << 2;
    }
    PTE *leaf_pte = (PTE *)(pt + vpn[0] * 4);
    *leaf_pte = (((uintptr_t)pa >> 12) << 10) | 0x3ff;
    if ((uintptr_t)va == 0x80001000) 
    {
        printf("old : %p\n", pte);
        printf("old : %p\n", vpn[0]);
        printf("old : %p\n", vpn[1]);
        printf("old : %p\n", (uintptr_t)as->ptr);
    }
    assert((uintptr_t)va == (((*leaf_pte & 0xfffffc00) << 2) + offset));
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry)
{
    Context *c = (Context *)(kstack.end - sizeof(Context));
    memset(c->gpr, 0, sizeof(c->gpr));
    c->gpr[2] = (uintptr_t)c;
    c->mcause = 0;
    c->mstatus = 0x1800;
    c->mepc = (uintptr_t)entry;
    printf("entry : %p\n", entry);

    return c;
}
