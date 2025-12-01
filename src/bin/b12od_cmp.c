#include <stdio.h>
#include <string.h>

#include <b12od/bolt12_offer_json.h>

int main(const int nargs, const char* args[])
{
  struct bolt12_json* b12j;
  int ret=0;
  char const* json_string;

  if(nargs != 3) {
    fprintf(stderr, "Usage: %s offer_string expected_result\n", args[0]);
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

    } else {
      printf("%s", json_string);

      if(strcmp(json_string, args[2])) {
	fprintf(stderr, "Comparison failed: '%s' vs '%s'\n", json_string, args[2]);
	ret = 1;
      }
    }
  }

  bolt12_offer_json_delete(b12j);
  return (ret != 0);
}
