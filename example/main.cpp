#include <iostream>
#include <sstream>
#include <string>
#include "types2d.h"

using namespace xeekworx::types2d;

int main(int argc, char *argv[])
{
    point<float> pt(5.1f, 2.2f);
    rectangle<float> rect(0.1f, 2.5f, 3.3f, 4.9f);

    std::cout << pt.str() << std::endl;
    std::cout << rect.str() << std::endl;
    return 0;
}