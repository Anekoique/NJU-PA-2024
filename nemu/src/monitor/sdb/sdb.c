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

#include "sdb.h"
#include <cpu/cpu.h>
#include <isa.h>
#include <memory/vaddr.h>
#include <readline/history.h>
#include <readline/readline.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
void new_wp(char *arges);
void free_wp(int no);
void show_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
    static char *line_read = NULL;
    if (line_read)
    {
        free(line_read);

        line_read = NULL;
    }

    line_read = readline("(nemu) ");

    if (line_read && *line_read)
    {
        add_history(line_read);
    }

    return line_read;
}

static int str2val(char *str)
{
    int value = 0;
    char *str_start = str;
    while (*str_start != '\0')
    {
        value = value * 10 + (*str_start - '0');
        str_start++;
    }
    return value;
}

static int cmd_c(char *args)
{
    cpu_exec(-1);
    return 0;
}

static int cmd_q(char *args)
{
    return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *arges)
{
    printf("%x\n", *(int *)0x80000000);
    uint64_t n = 0;
    if (arges == NULL)
    {
        cpu_exec(1);
        return 1;
    }
    int len = strlen(arges);
    while (len--)
    {
        n = n * 10 + (*arges - '0');
        arges++;
    }

    cpu_exec(n);
    return 0;
}

static int cmd_info(char *arges)
{
    if (arges == NULL)
    {
        printf("Please input available arges [r/w]\n");
        return 0;
    }
    if (*arges == 'r')
    {
        isa_reg_display();
    }
    else if (*arges == 'w')
    {
        show_wp();
    }
    else
    {
        printf("Please input available arges [r/w]!\n");
    }
    return 0;
}

static int cmd_x(char *arges)
{
    if (arges == NULL)
    {
        printf("Please input available arges [N] + [EXPR]!\n");
        return 0;
    }
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");
    int N = str2val(arg1);
    word_t value, vaddr;

    bool success = false;
    word_t nr_tokens = expr(arg2, &success);
    if (success)
    {
        vaddr = eval(0, nr_tokens - 1);
    }
    else
    {
        printf("Please input available arges!\n");
        return 0;
    }

    while (N--)
    {
        value = vaddr_read(vaddr, 4);
        vaddr += 4;
        printf("%x\n", value);
    }
    return 0;
}

static int cmd_p(char *arges)
{
    word_t value;

    bool success = false;
    word_t nr_tokens = expr(arges, &success);
    if (success)
    {
        value = eval(0, nr_tokens - 1);
        printf("%x", value);
    }
    else
    {
        printf("Please input available arges!\n");
    }
    return 0;
}

static int cmd_w(char *arges)
{
    init_wp_pool();
    new_wp(arges);
    return 0;
}

static int cmd_d(char *arges)
{
    int NO = str2val(arges);
    free_wp(NO);
    return 0;
}

static struct
{
    const char *name;
    const char *description;
    int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display information about all supported commands", cmd_help},
    {"c",    "Continue the execution of the program",            cmd_c   },
    {"q",    "Exit NEMU",                                        cmd_q   },
    {"si",   "Executate the following instructions",             cmd_si  },
    {"info", "Print register state/watchpoint information",      cmd_info},
    {"x",    "Output N consecutive 4 bytes in hexadecimal form", cmd_x   },
    {"p",    "Print the value of the expression",                cmd_p   },
    {"w",    "Set a watchpoint",                                 cmd_w   },
    {"d",    "Delete a watchpoint",                              cmd_d   },
    /* TODO: Add more commands */
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL)
    {
        /* no argument given */
        for (i = 0; i < NR_CMD; i++)
        {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else
    {
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(arg, cmd_table[i].name) == 0)
            {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

void sdb_set_batch_mode()
{
    is_batch_mode = true;
}

void sdb_mainloop()
{
    if (is_batch_mode)
    {
        cmd_c(NULL);
        return;
    }

    for (char *str; (str = rl_gets()) != NULL;)
    {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL)
        {
            continue;
        }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end)
        {
            args = NULL;
        }

#ifdef CONFIG_DEVICE
        extern void sdl_clear_event_queue();
        sdl_clear_event_queue();
#endif

        int i;
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(cmd, cmd_table[i].name) == 0)
            {
                if (cmd_table[i].handler(args) < 0)
                {
                    nemu_state.state = NEMU_QUIT;
                    return;
                }
                break;
            }
        }

        if (i == NR_CMD)
        {
            printf("Unknown command '%s'\n", cmd);
        }
    }
}

void init_sdb()
{
    /* Compile the regular expressions. */
    init_regex();

    /* Initialize the watchpoint pool. */
    init_wp_pool();
}
