#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_execute(char *command, char **mainargs, int arg_num) 
{
    printf("%s\n", command);
    if (strcmp(command, "execve") == 0)
    {
        //printf("%s\n", mainargs[0]);
        if (arg_num == 1) execve(mainargs[0], NULL, NULL);
    }
    else if (strcmp(command, "echo") == 0)
    {
        //printf("%s\n", mainargs[0]);
        if (mainargs != NULL)
            sh_printf("%s", mainargs[0]);
    }
}

static void sh_handle_cmd(const char *cmd) {
    if (cmd == nullptr) return;
    printf("%s\n", cmd);
    char command[20];
    char *mainargs[20];
    int pos = 0;
    int current_pos = 0;
    int arg_pos = -1;
    while (cmd[pos] != '\n')
    {
        if (arg_pos == -1) 
        {
            while (cmd[pos] == ' ') pos++;
            while (cmd[pos] != ' ' && cmd[pos] != '\n') 
                command[current_pos++] = cmd[pos++];
            if (current_pos == 0) return;
            command[current_pos] = '\0';
            arg_pos++;
            current_pos = 0;
        }
        else 
        {
            while (cmd[pos] == ' ') pos++;
            while (cmd[pos] != ' ' && cmd[pos] != '\n')
                mainargs[arg_pos][current_pos++] = cmd[pos++];
            if (current_pos == 0) break;
            mainargs[arg_pos][current_pos] = '\0';
            arg_pos++;
            current_pos = 0;
        }
    }
    printf("%d\n", arg_pos);
    if (arg_pos == 0)
        sh_execute(command, NULL, NULL);
    else 
        sh_execute(command, mainargs, arg_pos);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
