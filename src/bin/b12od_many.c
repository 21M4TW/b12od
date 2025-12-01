#include <stdio.h>

#include <b12od/bolt12_offer_json.h>

int main(const int nargs, const char* args[])
{
  struct bolt12_json* b12j;
  int ret=0;
  char const* json_string;
  int i;

  if(nargs < 2) {
    fprintf(stderr, "Usage: %s offer_string1 [offer_string2] ...\n", args[0]);
    return 1;
  }
  b12j = bolt12_offer_json_new();

  if(!b12j) {
    fprintf(stderr, "Failed initializing\n");

  } else {

    for(i=1; i<nargs; ++i) {
      printf("%s: ", args[i]);
      json_string = bolt12_offer_json(b12j, args[i]);

      if(!json_string) {
	fprintf(stderr, "Failed unexpectedly\n");
	ret = 1;

      } else printf("%s\n", json_string);
    }
  }

  bolt12_offer_json_delete(b12j);
  return (ret != 0);
}
