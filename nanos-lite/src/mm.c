#include <memory.h>

static void *pf = NULL;

void *new_page(size_t nr_page)
{
    void *current = pf;
    pf = (void *)ROUNDUP((uintptr_t)pf + nr_page * PGSIZE, PGSIZE);
    current = malloc((uintptr_t)pf - (uintptr_t)current);
    return current;
}

#ifdef HAS_VME
static void *pg_alloc(int n)
{
    void *pos = new_page(n / PGSIZE);
    memset(pos, 0, n);
    return pos;
}
#endif

void free_page(void *p)
{
    panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk)
{
    return 0;
}

void init_mm()
{
    pf = (void *)ROUNDUP(heap.start, PGSIZE);
    printf("pf : %p\n", pf);
    printf("heap.start : %p\n", heap.start);
    printf("heap.end : %p\n", heap.end);
    Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
    vme_init(pg_alloc, free_page);
#endif
}
