#include <stdio.h>
#include "bolt12_offer_json.h"

int main(const int nargs, const char* args[])
{
  struct bolt12_json* b12j;
  int ret=0;
  char const* json_string;

  if(nargs != 2) {
    fprintf(stderr, "Usage: %s offer_string\n", args[0]);
    return 1;
  }
  const char* offer_string = args[1];

  b12j = bolt12_offer_json_new();

  if(!b12j) {
    fprintf(stderr, "Failed initializing\n");

  } else {
    json_string = bolt12_offer_json(b12j, offer_string);

    if(!json_string) {
      fprintf(stderr, "Failed unexpectedly\n");
      ret = 1;

    } else printf("%s\n", json_string);
  }

  bolt12_offer_json_delete(b12j);
  return (ret != 0);
}
