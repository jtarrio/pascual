#include "pascual.h"

extern void pascual_main();

int main(int argc, const char** argv, const char** envp) {
  InitOs(argc, argv, envp);
  InitFile();
  pascual_main();
  return 0;
}