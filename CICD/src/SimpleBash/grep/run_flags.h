#if !defined(RUN_FLAG_H)
#define RUN_FLAG_H

#include "s21_grep.h"

void preprocess(opt *option_grep, char **argv);
void flag(int schet_filov, opt *option_grep, int tek_stroka, char **argv);
void postprocess(opt *option_grep, int shetchik_sovpadeniy, int schet_filov,
                 char **argv);
void run_flags(int argc, char **argv, opt *option_grep);

#endif  // RUN_FLAG_H
