/// copyright amazomic
#include "parcer.h"

void run_flags(int argc, char **argv, opt *option_cat) {
  int begin_str = 0, begin_str_a = 0, propusk_ch_v = 0;
  int end_str = 0;
  ssize_t i = 1;
  int propusk_ch = 0, propusk_ch_t = 0;
  while (optind < argc) {
    char ch;
    FILE *stream;
    stream = fopen(argv[optind], "r");
    if (stream == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[optind]);
      fclose(stream);
    } else
      while ((ch = getc(stream)) != EOF) {
        (ch == '\n' && option_cat->s ? propusk_ch++ : (propusk_ch = 0));  //-s

        if (begin_str == 0 && option_cat->n && propusk_ch < 3 &&  //-n
            !option_cat->b)
          (printf("%6ld\t", i++));
        (ch != '\n' && option_cat->n) ? (begin_str = 1) : (begin_str = 0);

        (ch == '\n' && (option_cat->e || option_cat->E) && propusk_ch < 3)  //-e
            ? (end_str = 1) && printf("$")
            : (end_str = 0);

        if (begin_str_a == 0 && ch != '\n' && (option_cat->b))  //-b
          (printf("%6ld\t", i++));
        (ch != '\n' && option_cat->b ? (begin_str_a = 1) : (begin_str_a = 0));

        (ch == '\t' && (option_cat->t || option_cat->T)  //-t
             ? printf("^I") && (propusk_ch_t = 1)
             : (propusk_ch_t = 0));

        if (option_cat->v || option_cat->t || option_cat->e) {  //-v
          propusk_ch_v = 0;
          (ch < 32 && ch != '\t' && ch != '\n') &&
              (printf("^%c", ch + 64) && (propusk_ch_v = 1));
          if ((ch == 127) && ((propusk_ch_v = 1))) printf("^?");
        }

        if (propusk_ch < 3 && propusk_ch_t == 0 && propusk_ch_v == 0)
          printf("%c", ch);
      }
    i = 1;
    fclose(stream);
    fflush(stdin);
    optind++;
  }
}