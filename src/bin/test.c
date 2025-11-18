#include <stdio.h>
#include "bolt12_offer_decode.h"
#include "bigsize.h"

int main()
{
  const char* string="lno1pgx9getnwss8vetrw3hhyucs5ypjgef743p5fzqq9nqxh0ah7y87rzv3ud0eleps9kl2d5348hq2k8qzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgqpqqqqqqqqqqqqqqqqqqqqqqqqqqqzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqqzq3zyg3zyg3zygs";
  struct bolt12_offer b12;
  init_bolt12_offer(&b12);

  int ret=bolt12_decode(string, &b12);

  printf("ret=%i, nrecords=%lu\n", ret, b12.nrecords);
  return 0;
}
