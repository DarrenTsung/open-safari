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
        
    private:
        GLuint _object;
        
        // copying is disabled
        Program(const Program&);
        const Program& operator=(const Program&);
    };
}

#endif /* defined(__open_safari__Program__) */
