/// copyright amazomic
#include "parcer.h"
#include "reader.h"

int main(int argc, char **argv) {
  if (argc < 2)
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n\t[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]\n");
  else {
    opt option_cat = {0};
    parcer_flags(argc, argv, &option_cat);
    run_flags(argc, argv, &option_cat);
  }

  return 0;
}
