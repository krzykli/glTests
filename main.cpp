#include <stdio.h>
#include <string.h>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "mesh.h"
#include "shader.c"
#include "glwindow.h"
#include "camera.h"
#include "util.h"

global_variable const char* PROJECT_ROOT = "C:\\Users\\krzykli\\glCourse";

global_variable GLfloat deltaTime = 0.0f;
global_variable GLfloat lastTime = 0.0f;

Mesh* CreatePiramid()
{
    static unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    static GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    Mesh *m = (struct Mesh*) malloc(sizeof(Mesh));
    CreateMesh(m, vertices, indices, 12, 12);
    return m;
}

Mesh* CreatePlane()
{
    static unsigned int indices[] = {
            0, 1, 2,
            2, 3, 1,
    };

    static GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };

    Mesh *m = (struct Mesh*) malloc(sizeof(Mesh));
    CreateMesh(m, vertices, indices, 12, 6);
    return m;
}


global_variable bool keys[1024] = {0};
global_variable GLfloat camera_lastX;
global_variable GLfloat camera_lastY;
global_variable GLfloat camera_changeX;
global_variable GLfloat camera_changeY;
global_variable GLfloat camera_changeX_value;
global_variable GLfloat camera_changeY_value;
global_variable bool mouseFirstMoved;


void onKeyPressedCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
         glfwSetWindowShouldClose(window, GL_TRUE);
         printf("Pressed Escape\n");
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
             keys[key] = true;
             //printf("Pressed key %d\n", key);
        }
        else if(action == GLFW_REPEAT)
        {
             keys[key] = true;
             //printf("Pressed key %d\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
             keys[key] = false;
             //printf("Released key %d\n", key);
        }
    }
}

void onMouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
{
     if (!mouseFirstMoved)
     {
          camera_lastX = xPos;
          camera_lastY = yPos;
          mouseFirstMoved = true;
     }

      camera_changeX = xPos - camera_lastX ;
      camera_changeY = camera_lastY - yPos;

      camera_lastX = xPos;
      camera_lastY = yPos;

      //printf("X %.6f, Y %.6f\n", camera_changeX, camera_changeY);

      camera_changeX_value = camera_changeX;
      camera_changeY_value = camera_changeY;
      camera_changeX = 0;
      camera_changeY = 0;
}

int _initializeGLFW()
{
    if (!glfwInit())
    {
        printf("GLFW failed");
        glfwTerminate();
        return 1;
    }

    /*// Version setting*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return 0;
}

int main()
{
    _initializeGLFW();

    WindowData windowData = WindowData();
    windowData.width = 800;
    windowData.height = 600;

    GLFWwindow* mainWindow = InitializeGLFWWindow(windowData, "OpenGl Course");
    /*glewExperimental = GL_TRUE;*/
    // GLEW needs context
    glfwMakeContextCurrent(mainWindow);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        glfwTerminate();
        printf("GLFW failed here");
        return 1;
    }

    glViewport(0, 0, windowData.bufferWidth, windowData.bufferHeight);

    const u32 meshCount = 3;
    Mesh *objects[meshCount] = {0};

    for (u8 i=0; i<meshCount - 1; i++)
    {
        Mesh *m = CreatePiramid();
        objects[i] = m;
    }
    Mesh *m = CreatePlane();
    objects[2] = m;

    const char* vertexFilePath = util::ConcatStrings(PROJECT_ROOT, "\\shaders\\vertex.glsl");
    const char* fragmentFilePath = util::ConcatStrings(PROJECT_ROOT, "\\shaders\\fragment.glsl");

    const char* gradientVertexFilePath = util::ConcatStrings(PROJECT_ROOT, "\\shaders\\gradient_vertex.glsl");
    const char* gradientFragmentFilePath = util::ConcatStrings(PROJECT_ROOT, "\\shaders\\gradient_fragment.glsl");

    glfwSetKeyCallback(mainWindow, onKeyPressedCallback);
    glfwSetCursorPosCallback(mainWindow, onMouseMovedCallback);

    ShaderProgram *shaderProg = (struct ShaderProgram*) malloc(sizeof(ShaderProgram));
    CreateShadersFromFiles(shaderProg, vertexFilePath, fragmentFilePath);

    ShaderProgram *gradientProg = (struct ShaderProgram*) malloc(sizeof(ShaderProgram));
    CreateShadersFromFiles(gradientProg, gradientVertexFilePath, gradientFragmentFilePath);

    Camera cam = Camera();

    InitializeCamera(
        &cam, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f, 0.0f, 1.0f, .1f);

    glm::mat4 projection = glm::perspective(
            45.0f, (GLfloat)windowData.bufferWidth/(GLfloat)windowData.bufferHeight, .1f, 1000.0f);

    while(!glfwWindowShouldClose(mainWindow))
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // user input
        glfwPollEvents();
        ControlCameraKeys(&cam, keys, deltaTime);
        //printf("X %.6f, Y %.6f\n", camera_changeX_value, camera_changeY_value);
        ControlCameraMouse(&cam, camera_changeX_value, camera_changeY_value);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Gradient
        glDisable(GL_DEPTH_TEST);
        UseShaderProgram(gradientProg);
        glUniform2f(gradientProg->uniformResolution, (GLfloat)windowData.width, (GLfloat)windowData.height);
        glUniform2f(gradientProg->uniformMousePosition, (GLfloat)camera_lastX, (GLfloat)camera_lastY);
        glUniform1f(gradientProg->uniformTime, (GLfloat)now);
        RenderMesh(objects[2]);
        glEnable(GL_DEPTH_TEST);

        UseShaderProgram(shaderProg);

        glm::mat4 model;
        glUniformMatrix4fv(shaderProg->uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(shaderProg->uniformTime, (GLfloat)now);

        glUniformMatrix4fv(
            shaderProg->uniformView, 1, GL_FALSE, glm::value_ptr(CalculateViewMatrix(&cam)));

        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2, 0, 1));
        //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(shaderProg->uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        RenderMesh(objects[0]);
        // location count transpose, pointer

        model = glm::mat4();
        //model = glm::translate(model, glm::vec3(0, 2, -2));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(shaderProg->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        RenderMesh(objects[1]);

        camera_changeX_value = 0;
        camera_changeY_value = 0;

        glfwSwapBuffers(mainWindow);
    }

    ClearShaderProgram(shaderProg);

    for (u8 i=0; i<meshCount; i++)
    {
        ClearMesh(objects[i]);
        free(objects[i]);
    }

    return 0;
}
