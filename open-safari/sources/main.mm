//
//  main.m
//  open-safari
//
//  Created by Darren Tsung on 5/16/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

// third-party libraries
#import <Foundation/Foundation.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// standard libraries
#import <iostream> 

#import "tdogl/Program.h"
#import "tdogl/Texture.h"
#import "Player.h"

// constants
const glm::vec2 SCREEN_SIZE(800, 600);
const float FPS = 60;

// globals
tdogl::Program* gProgram = NULL;
tdogl::Texture* gTexture = NULL;
Player gPlayer;
GLuint gVAO = 0;
GLuint gVBO = 0;
float gDegreesRotated = 0.0f;


// returns the full path the file `fileName` in the resources directory of the bundle
static std::string ResourcePath(std::string fileName) {
    NSString *fname = [NSString stringWithCString:(std::string("/") + fileName).c_str() encoding:NSUTF8StringEncoding];
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

// load shaders into a gProgram
static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
    
    gProgram->use();
    // create the camera matrix and set it as the uniform once (since it's not changing in the program)
    glm::mat4 camera = glm::lookAt(glm::vec3(3,3,3), glm::vec3(0,0,0), glm::vec3(0.0,1.0,0.0));
    //gProgram->setUniform("camera", camera);
    // create the projection matrix
    glm::mat4 projection = glm::perspective<float>(50.0, SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1, 100);
    //gProgram->setUniform("projection", projection);
    gProgram->stopUsing();
}

static void LoadTextures() {
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("wooden-crate.jpg"));
    bmp.flipVertically();
    gTexture = new tdogl::Texture(bmp);
}

static void LoadTriangle() {
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // put the vertices to represent a box
    GLfloat vertexData[] = {
        //  X     Y     Z       U     V
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        
        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 1.0f,
        
        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        
        // right
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("vert"));
    glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
    // connect the uv coordinates to the "vertTextCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
    glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// draws a single frame
static void Render() {
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // bind the program (shaders)
    gProgram->use();
    // bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture->object());
    gProgram->setUniform("tex", 0);
    
    gProgram->setUniform("model", glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(0,1,0)));
    gProgram->setUniform("player", gPlayer.matrix());
    
    // bind the VAO
    glBindVertexArray(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    
    // unbind the VAO and program
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    gProgram->stopUsing();
    
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers();
}

static void Update(float delta) {
    const GLfloat degreesPerSecond = 20.0f;
    gDegreesRotated += delta * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
    
    // update the player
    gPlayer.update(delta);
}

void AppMain() {
    if (!glfwInit())
        throw std::runtime_error("glfwInit() failed!");
    
    // open the window with hints
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if (!glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow() failed!");
    
    // GLFW settings
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // load the shaders and create the main program
    LoadShaders();
    // load the textures
    LoadTextures();
    
    // create buffers by points
    LoadTriangle();
    
    // setup gPlayer
    gPlayer.setPosition(glm::vec3(0,0,4));
    gPlayer.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
    
    double lastTime = glfwGetTime();
    // run while the window is open
    while(glfwGetWindowParam(GLFW_OPENED)){
        // update the scene based that the previous time
        double currTime = glfwGetTime();
        float delta = currTime - lastTime;
        while (delta <= 1.0f/FPS) {
            currTime = glfwGetTime();
            delta = currTime - lastTime;
        }
        lastTime = currTime;
        Update(delta);
        Render();
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;
        
        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
    }
    
    glfwTerminate();
}

int main(int argc, const char * argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
