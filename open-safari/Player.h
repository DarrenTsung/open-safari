//
//  Player.h
//  open-safari
//
//  Created by Darren Tsung on 5/19/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

#ifndef __open_safari__Player__
#define __open_safari__Player__

#include <iostream>
#include <glm/glm.hpp>

class Player {
    /**
     Type for keeping track of the player state
     */
    enum PlayerState {
        GROUND = 0,
        IN_AIR = 1
    };
    
    /**
     Player class that controls the camera and handles movement.
     
     Uses the matrix method to give a view and projection to the program.
     */
public:
    Player();
    
    /**
     Update the camera based on player input
     */
    void update(float delta);
    
    /**
     The position of the camera.
     */
    const glm::vec3& position() const;
    void setPosition(const glm::vec3& position);
    void offsetPosition(const glm::vec3& offset);
    
    /**
     The vertical viewing angle of the camera, in degrees.

     Determines how "wide" the view of the camera is. Large angles appear to be zoomed out,
     as the camera has a wide view. Small values appear to be zoomed in, as the camera has a
     very narrow view.

     The value must be between 0 and 180.
     */
    float fieldOfView() const;
    void setFieldOfView(float fieldOfView);
    
    /**
     The closest visible distance from the camera.

     Objects that are closer to the camera than the near plane distance will not be visible.

     Value must be greater than 0.
     */
    float nearPlane() const;

    /**
     The farthest visible distance from the camera.

     Objects that are further away from the than the far plane distance will not be visible.

     Value must be greater than the near plane
     */
    float farPlane() const;
    
    /**
     Sets the near and far plane distances.

     Everything between the near plane and the var plane will be visible. Everything closer
     than the near plane, or farther than the far plane, will not be visible.

     @param nearPlane  Minimum visible distance from camera. Must be > 0
     @param farPlane   Maximum visible distance from vamera. Must be > nearPlane
     */
    void setNearAndFarPlanes(float nearPlane, float farPlane);

    /**
     A rotation matrix that determines the direction the camera is looking.

     Does not include translation (the camera's position).
     */
    glm::mat4 orientation() const;

    /**
     Offsets the cameras orientation.

     The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.

     @param upAngle     the angle (in degrees) to offset upwards. Negative values are downwards.
     @param rightAngle  the angle (in degrees) to offset rightwards. Negative values are leftwards.
     */
    void offsetOrientation(float upAngle, float rightAngle);

    /**
     Orients the camera so that is it directly facing `position`

     @param position  the position to look at
     */
    void lookAt(glm::vec3 position);

    /**
     The width divided by the height of the screen/window/viewport

     Incorrect values will make the 3D scene look stretched.
     */
    float viewportAspectRatio() const;
    void setViewportAspectRatio(float viewportAspectRatio);

    /** A unit vector representing the direction the camera is facing */
    glm::vec3 forward() const;

    /** A unit vector representing the direction to the right of the camera*/
    glm::vec3 right() const;

    /** A unit vector representing the direction out of the top of the camera*/
    glm::vec3 up() const;

    /**
     The combined camera transformation matrix, including perspective projection.

     This is the complete matrix to use in the vertex shader.
     */
    glm::mat4 matrix() const;

    /**
     The perspective projection transformation matrix
     */
    glm::mat4 projection() const;

    /**
     The translation and rotation matrix of the camera.

     Same as the `matrix` method, except the return value does not include the projection
     transformation.
     */
    glm::mat4 view() const;

private:
    glm::vec3 _position, _velocity;
    float _horizontalAngle;
    float _verticalAngle;
    float _fieldOfView;
    float _nearPlane;
    float _farPlane;
    float _viewportAspectRatio;
    
    float _height;
    /**
     The movement system projects the direction vector onto the player's movement plane
     which is determined by the plane the player is standing on
     
     We only need to keep track of that plane's normal to define the plane
     */
    glm::vec3 _movementPlaneNormal;
    PlayerState _state;

    void normalizeAngles();
};

#endif /* defined(__open_safari__Player__) */
