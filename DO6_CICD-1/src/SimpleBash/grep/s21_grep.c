/// copyright amazomic
#include "s21_grep.h"

int main(int argc, char **argv) {
  opt option_grep = {0};

  if (argc < 3)
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n\t[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]\n");
  else {
    parcer_flags(argc, argv, &option_grep);
    run_flags(argc, argv, &option_grep);
  }

  return 0;
}
