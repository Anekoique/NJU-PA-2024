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
#include <memory/vaddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum
{
    TK_NOTYPE = 256,
    TK_EQ,
    TK_NUMBER,
    TK_DEREF,
    TK_HEX,
    TK_NEQ,
    TK_AND,
    TK_REG,
    /* TODO: Add more token types */
};

static struct rule
{
    const char *regex;
    int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +",          TK_NOTYPE}, // spaces
    {"\\+",         '+'      }, // plus
    {"==",          TK_EQ    }, // equal
    {"!=",          TK_NEQ   },
    {"&&",          TK_AND   },
    {"*",           '*'      },
    {"/",           '/'      },
    {"-",           '-'      },
    {"\\(",         '('      },
    {"\\)",         ')'      },
    {"[0-9]+",      TK_NUMBER},
    {"0x[0-9a-f]+", TK_HEX   },
    {"^\\$[a-zA-Z][0-9]+$",     TK_REG   },
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token
{
    int type;
    char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0')
    {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++)
        {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position,
                    substr_len, substr_len, substr_start);

                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */
                switch (rules[i].token_type)
                {
                case (TK_NOTYPE):
                    break;
                default:
                    if (substr_len >= 32)
                    {
                        printf("Your number's length >= 32\n");
                        assert(0);
                    }
                    else
                    {
                        Token token;
                        strncpy(token.str, substr_start, substr_len);
                        token.str[substr_len] = '\0';
                        token.type = rules[i].token_type;
                        tokens[nr_token++] = token;
                    }
                }

                break;
            }
        }

        if (i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

word_t expr(char *e, bool *success)
{
    if (!make_token(e))
    {
        *success = false;
        return 0;
    }

    /* TODO: Insert codes to evaluate the expression. */

    for (int i = 0; i < nr_token; i++)
    {

        if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '('))
        {
            tokens[i].type = TK_DEREF;
        }
    }

    *success = true;
    return nr_token;
}

bool check_parentheses(int p, int q)
{
    int l = 0, r = 0;
    if (tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    for (int i = p + 1; i < q; i++)
    {
        if (tokens[i].type == '(')
            l++;
        else if (tokens[i].type == ')')
            r++;
    }
    if (l == r)
        return true;
    else
    {
        printf("( and ) unmatched\n");
        assert(0);
    };
}

int find_op(int p, int q)
{
    int op1 = 0;
    int op2 = 0;
    int op3 = 0;
    for (int i = p; i < q; i++)
    {
        if (tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ || tokens[i].type == TK_AND)
            return tokens[i].type;
        else if (!op1 && (tokens[i].type == '+' || tokens[i].type == '-'))
            op1 = tokens[i].type;
        else if (!op2 && (tokens[i].type == '*' || tokens[i].type == '/'))
            op2 = tokens[i].type;
        else if (!op3 && tokens[i].type == TK_DEREF)
            op3 = tokens[i].type;

    }
    return op1 == 0 ? (op2 == 0 ? op3 : op2) : op1;
}

word_t eval(int p, int q)
{
    if (p > q)
    {
        /* Bad expression */
        printf("error : p > q\n");
        assert(0);
    }
    else if (p == q)
    {
        /* Single token.
         * For now this token should be a number.
         * Return the value of the number.
         */
        if (tokens[p].type == TK_REG)
        {
            bool *success = false;
            word_t val = isa_reg_str2val(tokens[p].str + 1, success);
            if (success)
                return val;
            else 
            {
                printf("Please input the available register!\n");
                assert(0);
            }
        }
        else 
        {
            word_t val = (word_t)strtoul(tokens[p].str, NULL, 16);
            return val;
        }
    }
    else if (check_parentheses(p, q) == true)
    {
        /* The expression is surrounded by a matched pair of parentheses.
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1);
    }
    else
    {
        int op = find_op(p, q);
        word_t val1 = 0, val2 = 0;
        if (op != TK_DEREF)
            val1 = eval(p, op - 1);
        val2 = eval(op + 1, q);

        switch (tokens[op].type)
        {
        case '+':
            return val1 + val2;
        case '-':
            return val1 - val2;
        case '*':
            return val1 * val2;
        case '/':
            return val1 / val2;
        case TK_EQ:
            return val1 == val2;
        case TK_NEQ:
            return val1 != val2;
        case TK_AND:
            return val1 && val2;
        case TK_DEREF:
            return vaddr_read(val2, 4);
        default:
            printf("unknown type!\n");
            assert(0);
        }
    }
    return 0;
}
