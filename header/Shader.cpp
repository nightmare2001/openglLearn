#include "Shader.h"

#include "../glm/gtc/type_ptr.hpp"

Shader::Shader(const char* vertexPath,const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();       
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();     
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    auto vShader = compileShader(GL_VERTEX_SHADER,vShaderCode);
    auto fShader = compileShader(GL_FRAGMENT_SHADER,fShaderCode);

    ID = glCreateProgram();

    glAttachShader(ID,vShader);
    glAttachShader(ID,fShader);
    glLinkProgram(ID);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

unsigned Shader::compileShader(int shaderType, const char* shaderSource)
{
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader,1,&shaderSource,nullptr);
    glCompileShader(shader);
    int success;
    char infolog[512];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(shader,512,nullptr,infolog);
        std::cout<<"error::"<<infolog<<std::endl;
    }
    return shader;
}
void Shader::setMat4(const std::string& name, glm::mat4 mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(mat4));
}

void Shader::setMat3(const std::string& name, glm::mat3 mat3) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(mat3));
}

void Shader::setMat2(const std::string& name, glm::mat2 mat2) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(mat2));
}








