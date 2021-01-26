#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <vector>

#include <common/Window.hpp>

using namespace std;

void Errors(const char *comment)
{
    GLenum er;
    while (er = glGetError())
        fprintf(stderr, "\nOpenGL ERROR: 0x%04x    =============%s===\n", er, comment);
}

float Window::Viewport(int _vtx, int _vty, int _vwd, int _vht)
{
    glViewport(_vtx, _vty, _vwd, _vht);
    return float(_vwd) / float(_vht);
}

float Window::ViewportOne(int _vtx, int _vty, int _vwd, int _vht)
{
    vtx = _vtx;
    vty = _vty;
    vwd = _vwd;
    vht = _vht;
    glViewport(_vtx, _vty, _vwd, _vht);
    return float(_vwd) / float(_vht);
}

void Window::Resize(int _wd, int _ht)
{

    wd = _wd;
    ht = _ht;
    int side = min(wd, ht);
    aspect = float(ht) / float(wd);
    if (ht > wd)
    {
        ViewportOne(0, abs((GLsizei)wd - (GLsizei)ht), side, side);
    }
    else
    {
        ViewportOne(0, 0, side, side);
    }
}

void Window::KeyCB(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        SetFullScreen(!IsFullScreen());
    }
}

// void Window::MouseButtonCB(int button, int action, int mods) {}
// void Window::MousePosCB(double xp, double yp) {}

void Window::WaitForFixedFPS(float frame_time)
{
    double wait_time = frame_time - (glfwGetTime() - prev_time);
    if (wait_time > 0.0)
        usleep(wait_time); //before swapBuf.. and time sync 60Hz..
    prev_time = glfwGetTime();
}

//===========================================================================
// Private functions:
//===========================================================================

// Init now not private:
void Window::Init(int width, int height, const char *name, int fullscr, int vers)
{
    if (!glfwInit())
    {
        glfwTerminate();
        throw std::runtime_error("Error initializing glfw   !");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
    if (vers > 32)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vers / 10);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vers % 10);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    GLFWmonitor **mons;
    int count;
    mons = glfwGetMonitors(&count);
    monitors[0] = mons[0];
    monitors[1] = mons[count - 1];
    monitor = (fullscr ? monitors[fullscr - 1] : monitors[0]);

    if (width == 0 || height == 0 || fullscr)
    {
        // if 0 or fullscr=1 or 2 than WINDOWLESS FULL SCREEN :
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        /*    glfwWindowHint(GLFW_RED_BITS,     mode->redBits);
       *    glfwWindowHint(GLFW_GREEN_BITS,   mode->greenBits);
       *    glfwWindowHint(GLFW_BLUE_BITS,    mode->blueBits);
       *    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
       */
        window = glfwCreateWindow(mode->width, mode->height, name, monitor, nullptr);
        winSize[0] = width, winSize[1] = height;
    }
    else
    {
        // Open a window and create its OpenGL context
        window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        glfwGetWindowSize(window, &winSize[0], &winSize[1]);
    }
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Error initializing window !");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, Window::CallbackResize);
    glfwSetKeyCallback(window, Window::CallbackKey);
    // glfwSetMouseButtonCallback(window, Window::CallbackMouseButton);
    // glfwSetCursorPosCallback(window, Window::CallbackMousePos);

    glfwGetWindowPos(window, &winPos[0], &winPos[1]);
    // Resize() begining:
    wd = width;
    ht = height;
    aspect = float(wd) / ht;
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
    glfwSetCursorPos(window, width / 2, height / 2);

    glfwSwapInterval(1); // 60 fps ?
    glEnable(GL_MULTISAMPLE);
}

void Window::CallbackResize(GLFWwindow *window, int cx, int cy)
{
    void *ptr = glfwGetWindowUserPointer(window);
    if (Window *winPtr = static_cast<Window *>(ptr))
        winPtr->Resize(cx, cy);
}

void Window::CallbackKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    void *ptr = glfwGetWindowUserPointer(window);
    if (Window *winPtr = static_cast<Window *>(ptr))
        winPtr->KeyCB(key, scancode, action, mods);
}
// void Window::CallbackMouseButton(GLFWwindow *window, int button, int action, int mods)
// {
//     void *ptr = glfwGetWindowUserPointer(window);
//     if (Window *winPtr = static_cast<Window *>(ptr))
//         winPtr->MouseButtonCB(button, action, mods);
// }

// void Window::CallbackMousePos(GLFWwindow *window, double xp, double yp)
// {
//     void *ptr = glfwGetWindowUserPointer(window);
//     if (Window *winPtr = static_cast<Window *>(ptr))
//         winPtr->MousePosCB(xp, yp);
// }

bool Window::IsFullScreen(void)
{
    return glfwGetWindowMonitor(window) != nullptr;
}

void Window::SetFullScreen(bool fullscreen)
{
    if (IsFullScreen() == fullscreen)
        return;
    if (fullscreen)
    {
        glfwGetWindowPos(window, &winPos[0], &winPos[1]);
        glfwGetWindowSize(window, &winSize[0], &winSize[1]);

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, winPos[0], winPos[1], winSize[0], winSize[1], 0);
        glfwShowWindow(window);
    }
    glfwGetFramebufferSize(window, &wd, &ht);
    Resize(wd, ht);
}
