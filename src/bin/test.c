#include <stdio.h>
#include "bolt12_offer_decode.h"
#include "bigsize.h"
#include "safe_char.h"
#include "tobytesbuf.h"

int main()
{
  const char* string="lno1pgx9getnwss8vetrw3hhyucs5ypjgef743p5fzqq9nqxh0ah7y87rzv3ud0eleps9kl2d5348hq2k8qzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgqpqqqqqqqqqqqqqqqqqqqqqqqqqqqzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqqzq3zyg3zyg3zygs";
  struct bolt12_offer b12;
  init_bolt12_offer(&b12);

  int ret=bolt12_decode(string, &b12);

  printf("ret=%i, nrecords=%lu\n", ret, b12.nrecords);

  char test_string[1024];
  sprintf(test_string, "ALLO\xBF");
  printf("'%s'\n", test_string);
  const size_t tslen=strlen(test_string);
  char outbuf[1024];
  ssize_t ret2=read_utf8(test_string, tslen, outbuf, sizeof(outbuf), SC_ALL);
  printf("read %li chars: '%.*s'\n", ret2, (int)ret2, outbuf);
  return 0;
}
