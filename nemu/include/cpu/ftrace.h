#ifndef __CPU_FTRACE_H__
#define __CPU_FTRACE_H__

#include <isa.h>
#include <elf.h>

#define MAX_FTRACE_LEN 200
#define MAX_FUNC_NUM 40
#define MAX_FUNC_NAME_LEN 20

typedef struct Ftrace
{
    enum
    {
        CALL = 0,
        CALL_RET,
    } type;
    vaddr_t inst_addr;
    char func_name[20];
    vaddr_t func_addr;
    vaddr_t ret_addr;

} Ftrace;

typedef struct Func 
{
    char func_name[MAX_FUNC_NAME_LEN];
    Elf32_Addr address;
    uint32_t size;
} Func;

extern Ftrace ftrace[MAX_FTRACE_LEN];
extern Func func_table[MAX_FUNC_NUM];
extern char *str_table;
extern Elf32_Sym *sym_table;
extern int func_num;
extern int ftrace_len;
#endif
