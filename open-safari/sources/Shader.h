//
//  Shader.h
//  open-safari
//
//  Created by Darren Tsung on 5/16/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//
//  Reference - Thomas Dalling - http://tomdalling.com/
//

#ifndef __open_safari__Shader__
#define __open_safari__Shader__

#include <GL/glew.h>
#include <string>

namespace tdogl {
    
    /**
     Represents a compiled OpenGL shader
     */
    class Shader {
    public:
        
        /**
         Creates a shader from a text file
         
         @param filePath    the path to the text file containing the shader source
         @param shaderType  same as the argument to glCreateShader; for example GL_VERTEX_SHADER 
                            or GL_FRAGMENT_SHADER
         
         @throws std::exception if error occurs
         */
        static Shader shaderFromFile(const std::string& filePath, GLenum shaderType);
        
        
        /**
         Creates a shader from a string of shader code
         
         @param shaderCode  the source code for the shader
         @param shaderType  same as the argument to glCreateShader; for example GL_VERTEX_SHADER
                            or GL_FRAGMENT_SHADER
         
         @throws std::exception if error occurs
         */
        Shader(const std::string& shaderCode, GLenum shaderType);
        
        
        /**
         @result the shader's object ID, as returned from glCreateShader
         */
        GLuint object() const;
        
        // tdogl::Shader objects can be copied and assigned because they are reference counted
        // like a shared pointer
        Shader(const Shader& other);
        Shader& operator =(const Shader& other);
        ~Shader();
        
    private:
        GLuint _object;
        unsigned* _refCount;
        
        void _retain();
        void _release();
    };
}

#endif /* defined(__open_safari__Shader__) */
