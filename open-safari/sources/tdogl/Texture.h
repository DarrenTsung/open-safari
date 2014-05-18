//
//  Texture.h
//  open-safari
//
//  Created by Darren Tsung on 5/17/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

#ifndef __open_safari__Texture__
#define __open_safari__Texture__

#include "Bitmap.h"
#include <iostream>
#include <GL/glew.h>

namespace tdogl {
    /**
     Class for representing textures.
     */
    class Texture {
    public:
        /** 
         Creates a texture from a bitmap.
         
         The texture will be loaded upside down since tdogl::Bitmap data is ordered
         from the top down, but OpenGL expects the date to be from the bottom up.
         
         @param bitmap          the bitmap to load the texture from
         @param minMagFilter    GL_NEAREST or GL_LINEAR (default)
         @param wrapMode        GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE (default), or GL_CLAMP_TO_BORDER
         */
        Texture(const Bitmap& bitmap,
                GLint minMagFilter = GL_LINEAR,
                GLint wrapMode = GL_CLAMP_TO_EDGE);
        
        /**
         Deletes the texture object using glDeleteTextures()
         */
        ~Texture();
        
        /**
         @result the texture object as created by glGenTextures()
         */
        GLuint object() const;
        
        /**
         @result the original width (in pixels) of the bitmap this texture was made from
         */
        GLfloat originalWidth() const;
        
        /**
         @result the original height (in pixels) of the bitmap this texture was made from
         */
        GLfloat originalHeight() const;
        
    private:
        GLuint _object;
        GLfloat _originalWidth, _originalHeight;
        
        //copying disabled
        Texture(const Texture&);
        const Texture& operator=(const Texture&);
    };
}

#endif /* defined(__open_safari__Texture__) */
