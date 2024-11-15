/// copyright amazomic
#include "run_flags.h"

void run_flags(int argc, char **argv, opt *option_grep) {
  preprocess(option_grep, argv);
  int schet_filov = 0;
  for (int shet_filov = optind; shet_filov < argc; shet_filov++) schet_filov++;
  while (optind < argc) {
    int shetchik_sovpadeniy = 0, tek_stroka = 0, i;

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read, column_num = 0;
    regmatch_t struct_o;
    regex_t re;
    fp = fopen(argv[optind], "r");
    if (fp == NULL) {
      if (!option_grep->s)
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[optind]);
    } else {
      while ((read = getline(&line, &len, fp)) != -1) {
        column_num++;
        int propusk = 0, start_o = 0;
        for (int x = 0; x <= option_grep->ind; x++) {
          if (option_grep->i) {  //// flag -i
            if (regcomp(&re, option_grep->pattern[x],
                        REG_NEWLINE + REG_ICASE) != 0)
              fprintf(stderr, "regcomp error\n");
          } else if (regcomp(&re, option_grep->pattern[x], REG_NEWLINE) != 0)
            fprintf(stderr, "regcomp error\n");
          i = regexec(&re, line, 0, 0, 0);
          if (option_grep->v && option_grep->ind != 0) {
            regfree(&re);
            for (int x = 0; x <= option_grep->ind; x++) {
              if (option_grep->i) {  //// flag -i
                if (regcomp(&re, option_grep->pattern[x],
                            REG_NEWLINE + REG_ICASE) != 0)
                  fprintf(stderr, "regcomp error\n");
              } else if (regcomp(&re, option_grep->pattern[x], REG_NEWLINE) !=
                         0)
                fprintf(stderr, "regcomp error\n");
              i = regexec(&re, line, 0, 0, 0);
              (i == 0) && (propusk = 1);
              regfree(&re);
            }
          }
          if (tek_stroka == column_num) {
            regfree(&re);
            break;
          } else
            tek_stroka = 0;
          if ((i == 0 && !option_grep->v) ||
              (i != 0 && option_grep->v && propusk != 1)) {
            ++shetchik_sovpadeniy;
            tek_stroka = column_num;
            (option_grep->l || option_grep->c) &&  /// dla flaga l and c
                (propusk = 1);
            flag(schet_filov, option_grep, tek_stroka, argv);
            if (option_grep->o && !option_grep->c &&
                !option_grep->l &&  //// flag 0
                !option_grep->v) {
              while ((regexec(&re, line + start_o, 1, &struct_o, 0)) == 0) {
                for (int st = struct_o.rm_so; st < struct_o.rm_eo; st++)
                  printf("%c", line[st + start_o]);
                printf("\n");
                start_o += struct_o.rm_eo;
              }
              propusk = 1;
              start_o = 0;
            }
            if (propusk == 0) printf("%s", line);
            if (line[read - 1] != '\n' && propusk != 1) printf("\n");
          }
          regfree(&re);
        }
      }
      postprocess(option_grep, shetchik_sovpadeniy, schet_filov, argv);
      if (line) free(line);
      fclose(fp);
    }
    optind++;
  }
}

void preprocess(opt *option_grep, char **argv) {
  if (!option_grep->e && !option_grep->f)
    strcpy(option_grep->pattern[option_grep->ind++], argv[optind++]);
  option_grep->ind--;
}

void flag(int schet_filov, opt *option_grep, int tek_stroka, char **argv) {
  if (schet_filov > 1 && !option_grep->l && !option_grep->c && !option_grep->h)
    printf("%s:", argv[optind]);
  if (option_grep->n && !option_grep->c && !option_grep->l)
    (printf("%d:", tek_stroka));
}

void postprocess(opt *option_grep, int shetchik_sovpadeniy, int schet_filov,
                 char **argv) {
  if (option_grep->l && option_grep->c && schet_filov == 1)
    (shetchik_sovpadeniy > 0) ? printf("1\n") : printf("0\n");
  if (option_grep->l && option_grep->c && schet_filov > 1 && !option_grep->h)
    (shetchik_sovpadeniy > 0) ? printf("%s:1\n", argv[optind])
                              : printf("%s:0\n", argv[optind]);
  if (option_grep->l && option_grep->c && schet_filov > 1 && option_grep->h)
    (shetchik_sovpadeniy > 0) ? printf("1\n") : printf("0\n");
  if (shetchik_sovpadeniy > 0 && option_grep->l) printf("%s\n", argv[optind]);
  if ((option_grep->c && schet_filov == 1 && !option_grep->l) ||
      (option_grep->h && option_grep->c && !option_grep->l))  //// flag c
    (printf("%d\n", shetchik_sovpadeniy));
  if (option_grep->c && schet_filov > 1 && !option_grep->h && !option_grep->l)
    (printf("%s:%d\n", argv[optind], shetchik_sovpadeniy));
}
