#include <SDL.h>
#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>

char handle_key(SDL_Event *ev);
static char PATH[20];

static void sh_printf(const char *format, ...)
{
    static char buf[256] = {};
    va_list ap;
    va_start(ap, format);
    int len = vsnprintf(buf, 256, format, ap);
    va_end(ap);
    term->write(buf, len);
}

static void sh_banner()
{
    sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt()
{
    sh_printf("sh> ");
}

static void sh_execute(char *command, char **mainargs, int arg_num)
{
    if (strcmp(command, "execve") == 0)
    {
        printf("%s\n", mainargs[0]);
        printf("%d\n", arg_num);
        char *argv[5];
        for (int i = 0; i < 5; i++)
            argv[i] = (char *)malloc(sizeof(char) * 10);
        for (int i = 0; i < arg_num - 1; i++)
            strcpy(argv[i], mainargs[i + 1]);
        argv[arg_num-1] = NULL;
        printf("here\n");

        if (arg_num == 1)
        {
            execve(mainargs[0], NULL, NULL);
            printf("arg_num = 1\n");
        }
        if (arg_num == 2)
            execve(mainargs[0], argv, NULL);
        if (arg_num == 3)
        {
            sh_printf("%s ", mainargs[0]);
            sh_printf("%s ", mainargs[1]);
            sh_printf("%s\n", mainargs[2]);
            execve(mainargs[0], (char **)mainargs[1], (char **)mainargs[2]);
        }

        for (int i = 0; i < 5; i++)
            free(argv[i]);
    }
    else if (strcmp(command, "echo") == 0)
    {
        if (mainargs != NULL)
        {
            sh_printf("%s\n", mainargs[0]);
        }
    }
    else if (strcmp(command, "setenv") == 0)
    {
        if (mainargs != NULL)
        {
            sscanf(mainargs[0], "PATH=%s", PATH);
            sh_printf("PATH=%s\n", PATH);
        }
    }
    else if (strcmp(command, "execvp") == 0)
    {
        if (arg_num == 1)
        {
            if (*(mainargs[0]) != '/')
                strcat(PATH, "/");
            strcat(PATH, mainargs[0]);
            execve(PATH, NULL, NULL);
        }
    }
    else if (strcmp(command, "exit") == 0)
        exit(0);
    else 
    {
        if (command[0] == '.')
            execve(&command[1], NULL, NULL);
    }
}

static void sh_handle_cmd(const char *cmd)
{
    if (cmd == nullptr)
        return;

    char command[20];
    char *mainargs[20];
    for (int i = 0; i < 20; i++)
    {
        mainargs[i] = (char *)malloc(sizeof(char) * 20);
    }

    int pos = 0;
    int current_pos = 0;
    int arg_pos = -1;
    while (cmd[pos] != '\n')
    {
        if (arg_pos == -1)
        {
            while (cmd[pos] == ' ')
                pos++;
            while (cmd[pos] != ' ' && cmd[pos] != '\n')
                command[current_pos++] = cmd[pos++];
            if (current_pos == 0)
                return;
            command[current_pos] = '\0';
            printf("%s\n", command);
            arg_pos++;
            current_pos = 0;
        }
        else
        {
            while (cmd[pos] == ' ')
                pos++;
            while (cmd[pos] != ' ' && cmd[pos] != '\n')
            {
                mainargs[arg_pos][current_pos++] = cmd[pos++];
                printf("%c\n", cmd[pos]);
            }
            if (current_pos == 0)
                break;
            mainargs[arg_pos][current_pos] = '\0';
            arg_pos++;
            current_pos = 0;
        }
    }
    mainargs[arg_pos] = NULL;
    printf("arg_pos : %d\n", arg_pos);
    if (arg_pos == 0)
        sh_execute(command, NULL, NULL);
    else
        sh_execute(command, mainargs, arg_pos);

    for (int i =0 ; i < 20; i++)
    {
        free(mainargs[i]);
    }
}

void builtin_sh_run()
{
    sh_banner();
    sh_prompt();

    while (1)
    {
        SDL_Event ev;
        if (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN)
            {
                const char *res = term->keypress(handle_key(&ev));
                if (res)
                {
                    sh_handle_cmd(res);
                    sh_prompt();
                }
            }
        }
        refresh_terminal();
    }
}
