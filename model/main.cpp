/*
 Imagine v0.1
 [model]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

//! @see wiki
/// http://imagine-research.org

#include <imagine/ig.h>
#include <imagine/core/log.h>

int main(int argc, char* argv[]) {
  log_(ig::info, "Hello world {} v{}.{}", "imagine", 1, 0);
  return 0;
}
