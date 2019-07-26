#pragma once
#include <fmt/format.h>
#include <Log.h>

#define LOC_TAG() fmt::format("{0}({1})", __FILE__, __LINE__)

#define GL_ERROR_CHECK() {GLenum glErrCode = glGetError(); if (glErrCode != GL_NO_ERROR) LOGE("gl Error Code: {0:04x}", glErrCode);}

#define LOG_GL_INT_CAPACITY(x)                  \
{                                               \
    GLint value = 0;                            \
    glGetIntegerv(x, &value);                   \
    LOGI("Gl Capacity {0} = {1}", #x, value);   \
}
