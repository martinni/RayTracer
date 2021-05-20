#include <iostream>
#include <stdlib.h>

#include "matrix.h"
#include "vector.h"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main()
{
  std::cout << "Hello World" << std::endl;

  Vec3f vec(1, 2, 3);
  Matrix44f mat(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
}
