// shader.hpp

#pragma once

#include <string>

using namespace std;

class Shader {
    public:
        Shader(const string &vertexShaderPath, const string &fragmentShaderPath);
        ~Shader();

        void SetUniform(const char *name, int var);
        void SetUniform(const char *name, int var1, int var2);
        void SetUniform(const char *name, int var1, int var2, int var3);
        void SetUniform(const char *name, int *var, int num);

        void SetUniform(const char *name, unsigned int var);
        void SetUniform(const char *name, unsigned int var1, unsigned int var2);
        void SetUniform(const char *name, unsigned int var1, unsigned int var2, unsigned int var3);
        void SetUniform(const char *name, unsigned int *var, int num);

        void SetUniform(const char *name, float var);
        void SetUniform(const char *name, float var1, float var2);
        void SetUniform(const char *name, float var1, float var2, float var3);
        void SetUniform(const char *name, float *var, int num);

        int GetUniform(const char *name);
        unsigned int GetShaderProgramID() const;

    private:
        char *vertexShaderSource, *fragmentShaderSource;
        unsigned int shaderProgramID;

        void GetShaderSource(const string &vertexShaderPath, const string &fragmentShaderPath);
        void CompileShader();
};
