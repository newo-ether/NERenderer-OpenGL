// shader.cpp

#include "include/shader.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "../../third-party/glad/include/glad/glad.h"

using namespace std;

Shader::Shader(const string &vertexShaderPath, const string &fragmentShaderPath) {
    GetShaderSource(vertexShaderPath, fragmentShaderPath);
    CompileShader();
}

Shader::~Shader() {
    glDeleteProgram(shaderProgramID);
    delete [] vertexShaderSource;
    delete [] fragmentShaderSource;
}

void Shader::GetShaderSource(const string &vertexShaderPath, const string &fragmentShaderPath) {
    ifstream vertexShaderFile, fragmentShaderFile;

    // Open Vertex Shader File
    vertexShaderFile.open(vertexShaderPath);
    if (!vertexShaderFile.is_open()) { 
        cout << "[Error] Shader: Failed to open vertex shader file." << endl; 
        exit(-1);
    } 

    // Open Fragment Shader File
    fragmentShaderFile.open(fragmentShaderPath);
    if (!fragmentShaderFile.is_open()) { 
        cout << "[Error] Shader: Failed to open fragment shader file." << endl; 
        exit(-1);
    } 

    // Get Shader Source
    stringstream vertexShaderStream, fragmentShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();       
    string vertexShaderString = vertexShaderStream.str();
    string fragmentShaderString = fragmentShaderStream.str(); 
    vertexShaderFile.close();
    fragmentShaderFile.close();

    // Copy Shader Source
    vertexShaderSource = new char[vertexShaderString.length() + 1];
    fragmentShaderSource = new char[fragmentShaderString.length() + 1];
    strcpy(vertexShaderSource, vertexShaderString.c_str());
    strcpy(fragmentShaderSource, fragmentShaderString.c_str());
}

void Shader::CompileShader() {
    // Vertex Shader Compilation
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[2048];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 2048, NULL, infoLog);
        cout << "[Error] Shader: Vertex shader compile failed." << infoLog << endl;
        exit(-1);
    }

    // Fragment Shader Compilation
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 2048, NULL, infoLog);
        cout << "[Error] Shader: Fragment shader compile failed." << infoLog << endl;
        exit(-1);
    }

    // Link Shaders to Create Shader Program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 2048, NULL, infoLog);
        cout << "[Error] Shader: Link failed." << infoLog << endl;
        exit(-1);
    }

    // Delete Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::SetUniform(const char *name, int var) {
    glUniform1i(GetUniform(name), var);
}

void Shader::SetUniform(const char *name, int var1, int var2) {
    glUniform2i(GetUniform(name), var1, var2);
}

void Shader::SetUniform(const char *name, int var1, int var2, int var3) {
    glUniform3i(GetUniform(name), var1, var2, var3);
}

void Shader::SetUniform(const char *name, int *var, int num) {
    switch (num) {
        case 1: glUniform1iv(GetUniform(name), 1, var); break;
        case 2: glUniform2iv(GetUniform(name), 1, var); break;
        case 3: glUniform3iv(GetUniform(name), 1, var); break;
        default:
            cout << "[Error] Shader: Uniform variable number error." << endl;
            exit(-1);
    }
}

void Shader::SetUniform(const char *name, unsigned int var) {
    glUniform1ui(GetUniform(name), var);
}

void Shader::SetUniform(const char *name, unsigned int var1, unsigned int var2) {
    glUniform2ui(GetUniform(name), var1, var2);
}

void Shader::SetUniform(const char *name, unsigned int var1, unsigned int var2, unsigned int var3) {
    glUniform3ui(GetUniform(name), var1, var2, var3);
}

void Shader::SetUniform(const char *name, unsigned int *var, int num) {
    switch (num) {
        case 1: glUniform1uiv(GetUniform(name), 1, var); break;
        case 2: glUniform2uiv(GetUniform(name), 1, var); break;
        case 3: glUniform3uiv(GetUniform(name), 1, var); break;
        default:
            cout << "[Error] Shader: Uniform variable number error." << endl;
            exit(-1);
    }
}

void Shader::SetUniform(const char *name, float var) {
    glUniform1f(GetUniform(name), var);
}

void Shader::SetUniform(const char *name, float var1, float var2) {
    glUniform2f(GetUniform(name), var1, var2);
}

void Shader::SetUniform(const char *name, float var1, float var2, float var3) {
    glUniform3f(GetUniform(name), var1, var2, var3);
}

void Shader::SetUniform(const char *name, float *var, int num) {
    switch (num) {
        case 1: glUniform1fv(GetUniform(name), 1, var); break;
        case 2: glUniform2fv(GetUniform(name), 1, var); break;
        case 3: glUniform3fv(GetUniform(name), 1, var); break;
        default:
            cout << "[Error] Shader: Uniform variable number error." << endl;
            exit(-1);
    }
}

int Shader::GetUniform(const char *name) {
    glUseProgram(shaderProgramID);
    int uniform = glGetUniformLocation(shaderProgramID, name);
    if (uniform == -1) {
        cout << "[Warning] Shader: Failed to locate uniform variable \""
             << name << "\"." << endl;
        // exit(-1);
    }
    return uniform;
}

unsigned int Shader::GetShaderProgramID() const {
    return shaderProgramID;
}
