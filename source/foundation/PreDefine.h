/**

    @file      PreDefine.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

// define the namespace for current project
#define SAFE_DELETE(x)                                                                                                                                         \
    if (x)                                                                                                                                                     \
    {                                                                                                                                                          \
        delete x;                                                                                                                                              \
        x = nullptr;                                                                                                                                           \
    }
#define SAFE_DELETE_ARRAY(x)                                                                                                                                   \
    if (x)                                                                                                                                                     \
    {                                                                                                                                                          \
        delete[] x;                                                                                                                                            \
        x = nullptr;                                                                                                                                           \
    }
#define SAFE_DELETE_AND_TERMINATE(x)                                                                                                                           \
    if (x)                                                                                                                                                     \
    {                                                                                                                                                          \
        x->terminate();                                                                                                                                        \
        delete x;                                                                                                                                              \
        x = nullptr;                                                                                                                                           \
    }

#define ALIGN_BYTES_4(x) ((((x) + 3) >> 2) << 2)
