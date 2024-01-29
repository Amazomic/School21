#if !defined(PARSER_H)
#define PARSER_H

#include <getopt.h>
#include <stdio.h>

typedef struct option_c {
  int b;
  int n;
  int e;
  int s;
  int t;
  int v;
  int T;
  int E;
} opt;

void parcer_flags(int argc, char **argv, opt *option_cat);

#endif  // PARSER_H
