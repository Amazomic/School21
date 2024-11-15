/// copyright amazomic
#include "parcer.h"

void parcer_flags(int argc, char **argv, opt *option_grep) {
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  int c = -1;
  while (1) {
    c = getopt_long(argc, argv, "e:ivclnhsf:o", long_options, NULL);
    if (c == -1) break;
    switch (c) {
      case 'e':
        option_grep->e = 1;
        if (optarg != NULL)
          strcpy(option_grep->pattern[option_grep->ind++], optarg);
        break;
      case 'i':
        option_grep->i = 1;
        break;
      case 'v':
        option_grep->v = 1;
        break;
      case 'c':
        option_grep->c = 1;
        break;
      case 'l':
        option_grep->l = 1;
        break;
      case 'n':
        option_grep->n = 1;
        break;
      case 'h':
        option_grep->h = 1;
        break;
      case 's':
        option_grep->s = 1;
        break;
      case 'f':
        option_grep->f = 1;
        FILE *fp;
        char *read;
        fp = fopen(optarg, "r");
        if (fp == NULL) exit(EXIT_FAILURE);
        char line[1000];
        while ((read = fgets(line, 1000, fp)) != NULL)
          strcpy(option_grep->pattern[option_grep->ind++], line);
        for (int ind = option_grep->ind; ind >= 0;
             ind--) {  //// убираем \n в конце строки
          for (int x = 0; option_grep->pattern[ind][x++] != '\0';)
            if (option_grep->pattern[ind][x] == '\n' && x != 0)
              option_grep->pattern[ind][x] = '\0';
        }
        fclose(fp);
        break;
      case 'o':
        option_grep->o = 1;
        break;
      case '?':
        break;
      default:
        fprintf(stderr, "?? getopt возвратило код символа 0%o ??\n", c);
    }
  }
}