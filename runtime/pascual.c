#include "pascual.h"

extern void pascual_main();

int main(int argc, const char** argv) {
  InitOs(argc, argv);
  InitFile();
  pascual_main();
  return 0;
}