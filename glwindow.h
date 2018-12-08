#ifndef GLWINDOW
#define GLWINDOW
#include "GLFW/glfw3.h"

struct WindowData
{
     u32 width;
     u32 height;

     u32 bufferWidth;
     u32 bufferHeight;
};


GLFWwindow* InitializeGLFWWindow(WindowData &wd, const char* title)
{
    GLFWwindow *mainWindow = glfwCreateWindow(wd.width, wd.height, title, NULL, NULL);
    if (!mainWindow)
    {
        printf("Main window failed");
        glfwTerminate();
        return NULL;
    }
    else
    {
        glfwGetFramebufferSize(mainWindow, (int *)&wd.bufferWidth, (int *)&wd.bufferHeight);
        return mainWindow;
    }
}

#endif
