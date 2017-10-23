#include <imagine/ig.h>
#include <imagine/core/log.h>

int main(int argc, char* argv[]) {
  log_(ig::info, "Hello world {} v{}.{}", "imagine", 1, 0);
  return 0;
}
