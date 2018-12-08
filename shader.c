#include <GL\glew.h>
#include "typedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

struct ShaderProgram
{
    GLuint programID;
    GLuint uniformProjection;
    GLuint uniformView;
    GLuint uniformModel;
    GLuint uniformResolution;
    GLuint uniformMousePosition;
    GLuint uniformTime;
};


void _AddShader(GLuint programID, const char* shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // shader, count of strings, t
    glShaderSource(shader, 1, theCode, codeLength);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader:\n%s\n", shaderType, eLog);
        return;
    }
    glAttachShader(programID, shader);

}

void _CompileShader(ShaderProgram *shaderProg, const char *vertexCode, const char *fragmentCode)
{
    GLuint programID = glCreateProgram();

    if (!programID)
    {
        printf("error generating shader program\n");
        return;
    }
    _AddShader(programID, vertexCode, GL_VERTEX_SHADER);
    _AddShader(programID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(programID, sizeof(eLog), NULL, eLog);
        printf("Error linking program %s \n", eLog);
        return;
    }

    glValidateProgram(programID);
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(programID, sizeof(eLog), NULL, eLog);
        printf("Error validating program %s \n", eLog);
        return;
    }

    shaderProg->programID = programID;
    shaderProg->uniformModel = glGetUniformLocation(programID, "model");
    shaderProg->uniformView = glGetUniformLocation(programID, "view");
    shaderProg->uniformProjection = glGetUniformLocation(programID, "projection");
    shaderProg->uniformResolution = glGetUniformLocation(programID, "resolution");
    shaderProg->uniformMousePosition = glGetUniformLocation(programID, "mousePos");
    shaderProg->uniformTime = glGetUniformLocation(programID, "time");
}

void UseShaderProgram(ShaderProgram *shaderProg)
{
    glUseProgram(shaderProg->programID);
}

void ClearShaderProgram(ShaderProgram *shaderProg)
{
    if (shaderProg->programID != 0)
    {
        glDeleteProgram(shaderProg->programID);
        shaderProg->programID = 0;
    }
    shaderProg->uniformProjection = 0;
    shaderProg->uniformModel = 0;
    shaderProg->uniformResolution = 0;
    shaderProg->uniformMousePosition = 0;
    shaderProg->uniformTime = 0;

}

void CreateShaderFromString(ShaderProgram *shaderProg,
        const char *vertexCode, const char *fragmentCode)
{
    _CompileShader(shaderProg, vertexCode, fragmentCode);
}


void CreateShadersFromFiles(ShaderProgram *shaderProg,
        const char *vertexFilePath, const char *fragmentFilePath)
{
    char *vertexCode = util::ReadFile(vertexFilePath, "rb");
    char *fragmentCode = util::ReadFile(fragmentFilePath, "rb");

    _CompileShader(shaderProg, vertexCode, fragmentCode);

    free(vertexCode);
    free(fragmentCode);
}

