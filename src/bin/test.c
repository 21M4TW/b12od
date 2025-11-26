#include <stdio.h>
#include "bolt12_offer_json.h"

int main()
{
  const char* string="lno1pgx9getnwss8vetrw3hhyucs5ypjgef743p5fzqq9nqxh0ah7y87rzv3ud0eleps9kl2d5348hq2k8qzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgqpqqqqqqqqqqqqqqqqqqqqqqqqqqqzqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqqzq3zyg3zyg3zygs";
  struct bolt12_json b12j;
  int ret;
  char const* json_string;

  bolt12_offer_json_init(&b12j);

  ret = bolt12_offer_json(&b12j, string, &json_string);

  if(ret) {
    fprintf(stderr, "Failed with error %i\n", ret);
  }

  printf("%s\n", json_string);

  bolt12_offer_json_free(&b12j);
  return (ret != 0);
}
