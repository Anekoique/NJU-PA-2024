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

#define NR_WP 32

typedef struct watchpoint
{
    int NO;
    struct watchpoint *next;

    /* TODO: Add more members if necessary */
    char *expression;
    word_t pre_value;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; i++)
    {
        wp_pool[i].NO = i;
        wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    }

    head = NULL;
    free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp(char *arges)
{
    if (!free_)
        assert(0);
    free_->expression = arges;

    bool *success = false;
    word_t nr_tokens = expr(arges, success);
    if (success)
    {
        WP *temp = free_->next;
        free_->pre_value = eval(0, nr_tokens - 1);
        free_ = free_->next;
        free_->next = head;
        head = free_;
        free_ = temp;
    }
    else
    {
        printf("Please input available arges!\n");
    }
    return;
}

void free_wp(int no)
{
    WP *current = head;
    WP *pre = NULL;

    while (current != NULL)
    {
        if (current->NO == no)
        {
            if (pre == NULL)
                head = head->next;
            else
                pre->next = current->next;
            free_ = current;
            break;
        }
        pre = current;
        current = current->next;
    }
    printf("unavailable NO!\n");
    return;
}
