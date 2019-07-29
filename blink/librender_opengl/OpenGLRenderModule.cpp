/*!
 * \file OpenGLRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "OpenGLRenderModule.h"
#include "util/ImageLoader.h"
#include "GlConfig.h"

namespace blink
{
    static bool createTextureFromRawData(ImageLoader::TextureInfo& textureInfo, int width, int height, int channels, const void* data)
    {
        GLint glFormat = GL_ALPHA;
        switch (channels)
        {
        case 1:
            glFormat = GL_LUMINANCE;
            break;
        case  2:
            glFormat = GL_LUMINANCE_ALPHA;
        case 3:
            glFormat = GL_RGB;
            break;
        case 4:
            glFormat = GL_RGBA;
            break;
        default:
            return false;
            break;
        }

        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        if (textureId == 0) return false;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        textureInfo.textureId = textureId;
        textureInfo.texSize = glm::ivec2(width, height);
        return true;
    }


    void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
//        g_app->m_ex.events.emit<MouseEvent>(MouseEvent::Action::Move, glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
//         MouseEvent::Action act = MouseEvent::Action::ButtonDown;
//         if (action == GLFW_PRESS) act = MouseEvent::Action::ButtonDown;
//         else if (action == GLFW_RELEASE) act = MouseEvent::Action::ButtonUp;
// 
//         g_app->m_ex.events.emit<MouseEvent>(act, static_cast<MouseEvent::MouseButton>(button), static_cast<uint32>(mods));
    }

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
//         g_app->m_ex.events.emit<MouseEvent>(MouseEvent::Action::Scroll, glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset)));
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
//         g_app->m_ex.events.emit<KeyboardEvent>(key, scancode, action, mods);
    }

    OpenGLRenderModule::~OpenGLRenderModule()
    {

    }

    bool OpenGLRenderModule::createDevice(const glm::ivec2& deviceSize)
    {
        /* Initialize the library */
        if (!glfwInit()) return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(1280, 720, "blink", nullptr, nullptr);
        if (!m_window) return false;

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(m_window, mousePositionCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, mouseScrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        // access gl API after context created
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

        // depth test setup
        glEnable(GL_DEPTH_TEST);

        // cull face setup
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        // blend mode setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // create vao
        GLuint vao{};
        glGenVertexArrays(1, &vao);
        GL_ERROR_CHECK();

        // bind vao
        glBindVertexArray(vao);
        GL_ERROR_CHECK();

        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        return true;
    }

    void OpenGLRenderModule::destroyDevice()
    {
        glfwTerminate();
    }

    bool OpenGLRenderModule::gameLoop()
    {
        double begin = glfwGetTime();

        /* Loop until the user closes the window */
        if (glfwWindowShouldClose(m_window)) return false;

        /* Poll for and process events */
        glfwPollEvents();

        double end = glfwGetTime();
        double duration = end - begin;
        begin = end;

//         app->step(static_cast<float>(duration));

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        return true;
    }
}
