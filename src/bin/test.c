#include <stdio.h>
#include "bolt12_offer_json.h"

int main(const int nargs, const char* args[])
{
  struct bolt12_json b12j;
  int ret;
  char const* json_string;

  if(nargs != 2) {
    fprintf(stderr, "Usage: %s offer_string\n", args[0]);
    return 1;
  }
  const char* offer_string = args[1];

  bolt12_offer_json_init(&b12j);

  ret = bolt12_offer_json(&b12j, offer_string, &json_string);

  if(ret) {
    fprintf(stderr, "Failed with error %i\n", ret);
  }

  printf("%s\n", json_string);

  bolt12_offer_json_free(&b12j);
  return (ret != 0);
}
