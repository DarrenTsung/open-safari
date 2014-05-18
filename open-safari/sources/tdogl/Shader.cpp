//
//  Shader.cpp
//  open-safari
//
//  Created by Darren Tsung on 5/16/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

#include "Shader.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

using namespace tdogl;

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
    _object(0),
    _refCount(NULL)
{
    // store the shader ID
    _object = glCreateShader(shaderType);
    if (_object == 0)
        throw std::runtime_error("glCreateShader() failed");
    
    // set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(_object, 1, (const GLchar**)&code, NULL);
    
    // compile
    glCompileShader(_object);
    
    // check if compile error occured, throw exception if needed
    GLint status;
    glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile error in the shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(_object); _object = 0;
        throw std::runtime_error(msg);
    }
    
    _refCount = new unsigned;
    *_refCount = 1;
}

Shader::Shader(const Shader& other) :
    _object(other._object),
    _refCount(other._refCount)
{
    _retain();
}

Shader::~Shader() {
    if (_refCount) _release();
}

GLuint Shader::object() const {
    return _object;
}

Shader& Shader::operator = (const Shader& other) {
    _release();
    _object = other._object;
    _refCount = other._refCount;
    _retain();
    return *this;
}

Shader Shader::shaderFromFile(const std::string &filePath, GLenum shaderType) {
    // open the file
    std::ifstream f;
    f.open(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
        throw std::runtime_error(std::string("Error opening shader file: ") + filePath);
    
    // read the whole file into the string buffer
    std::stringstream buffer;
    buffer << f.rdbuf();
    
    // return new shader
    Shader shader(buffer.str(), shaderType);
    return shader;
}

void Shader::_retain() {
    assert(_refCount);
    *_refCount += 1;
}

void Shader::_release() {
    assert(_refCount && *_refCount > 0);
    *_refCount -= 1;
    if (*_refCount == 0) {
        glDeleteShader(_object); _object = 0;
        delete _refCount; _refCount = NULL;
    }
}

