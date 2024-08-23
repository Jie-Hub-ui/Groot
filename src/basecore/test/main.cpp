#include <iostream>
#include "app/ncJson.h"
#include "util/ncSingleton.h"

int main(int argc, char *argv[])
{

#ifdef AUTO_RUN
    std::cout << "define" << std::endl;
#else
    std::cout << "no define" << std::endl;
#endif

    return 0;
}