/**

    @file      HelloWorldApp.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "HelloWorldApp.h"

#include <blink/blink.h>

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
