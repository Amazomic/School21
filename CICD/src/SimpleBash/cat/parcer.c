/// copyright amazomic
#include "parcer.h"

void parcer_flags(int argc, char **argv, opt *option_cat) {
  int c;
  int option_index = 0;
  while (1) {
    static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                           {"number", 0, 0, 'n'},
                                           {"squeeze-blank", 0, 0, 's'},
                                           {0, 0, 0, 0}};
    c = getopt_long(argc, argv, "+benstvET", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'b':
        option_cat->b = 1;
        break;

      case 'e':
        option_cat->e = 1;
        break;

      case 'n':
        option_cat->n = 1;
        break;

      case 's':
        option_cat->s = 1;
        break;

      case 't':
        option_cat->t = 1;
        break;

      case 'v':
        option_cat->v = 1;
        break;

      case 'T':
        option_cat->T = 1;
        break;

      case 'E':
        option_cat->E = 1;
        break;

      case '?':
        break;

      default:
        fprintf(stderr, "?? getopt возвратило код символа 0%o ??\n", c);
    }
  }
}