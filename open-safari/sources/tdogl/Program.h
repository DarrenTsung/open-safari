//
//  Program.h
//  open-safari
//
//  Created by Darren Tsung on 5/16/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//
//  Reference - Thomas Dalling - http://tomdalling.com/
//

#ifndef __open_safari__Program__
#define __open_safari__Program__

#include "Shader.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace tdogl {
    
    /** 
     Represents an OpenGL program
     */
    class Program {
    public:
        /**
         Creates a program from a vector of tdogl::shaders
         
         @param shaders     The shaders to link together to create the program
         
         @throws std::exception if an error occurs
         
         @see tdogl::shader
         */
        Program(const std::vector<Shader>& shaders);
        ~Program();
        
        /**
         @result the program's object id, as returned from glCreateProgram()
         */
        GLuint object() const;
        
        /**
         @result the attribute index for the given name, as returned by glGetAttribLocation
         */
        GLint attrib(const GLchar* attribName) const;
        
        /**
         @result the uniform index for the given name, as returned by glGetUniformLocation
         */
        GLint uniform(const GLchar* uniformName) const;
        
        void use() const;
        
        bool isInUse() const;
        
        void stopUsing() const;
        
        /**
         Setters for attribute and uniform variables.
         
         These are convenience methods for the glVertexAttrib* and glUniform* functions.
         */
#define _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
        \
        void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
        \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
        \
        void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)
        
        void setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
        void setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
        void setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
        void setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose=GL_FALSE);
        void setUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose=GL_FALSE);
        void setUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose=GL_FALSE);
        void setUniform(const GLchar* uniformName, const glm::vec3& v);
        void setUniform(const GLchar* uniformName, const glm::vec4& v);
        
    private:
        GLuint _object;
        
        // copying is disabled
        Program(const Program&);
        const Program& operator=(const Program&);
    };
}

#endif /* defined(__open_safari__Program__) */
