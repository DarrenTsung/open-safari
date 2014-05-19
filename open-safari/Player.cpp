//
//  Player.cpp
//  open-safari
//
//  Created by Darren Tsung on 5/19/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glfw.h>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

static inline float RadiansToDegrees(float radians) {
    return radians * 180.0f / (float)M_PI;
}


Player::Player() :
_position(0.0f, 0.0f, 1.0f),
_horizontalAngle(0.0f),
_verticalAngle(0.0f),
_fieldOfView(50.0f),
_nearPlane(0.01f),
_farPlane(100.0f),
_viewportAspectRatio(4.0f/3.0f),
_height(2.0f),
_velocity(0.0f, 0.0f, 0.0f),
_state(GROUND)
{
}

void Player::update(float delta) {
    const float moveSpeed = 2.0f;
    
    if(glfwGetKey('S')){
        _position += delta * moveSpeed * -forward();
    } else if(glfwGetKey('W')){
        _position += delta * moveSpeed * forward();
    }
    if(glfwGetKey('A')){
        _position += delta * moveSpeed * -right();
    } else if(glfwGetKey('D')){
        _position += delta * moveSpeed * right();
    }
    
    if(glfwGetKey(' ')) {
        if (_state == GROUND) {
            _velocity = 3.0f * glm::vec3(0,1,0);
            _state = IN_AIR;
        }
    }
    
    // gravity
    const float psuedoGravity = 2.0f;
    _velocity += delta * psuedoGravity * -glm::vec3(0,1,0);
    
    // add velocity to position
    _position += delta * _velocity;
    
    // stop when colliding with y == 0
    if (_position.y <= 0.0 + _height/2) {
        _position.y = 0.0 + _height/2;
        _state = GROUND;
    }
    
    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.1;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
    glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window

    //increase or decrease field of view based on mouse wheel
    const float zoomSensitivity = -0.2;
    float fieldOfView = _fieldOfView + zoomSensitivity * (float)glfwGetMouseWheel();
    if(fieldOfView < 5.0f) fieldOfView = 5.0f;
    if(fieldOfView > 130.0f) fieldOfView = 130.0f;
    setFieldOfView(fieldOfView);
    glfwSetMouseWheel(0);
}

const glm::vec3& Player::position() const {
    return _position;
}

void Player::setPosition(const glm::vec3& position) {
    _position = position;
}

void Player::offsetPosition(const glm::vec3& offset) {
    _position += offset;
}

float Player::fieldOfView() const {
    return _fieldOfView;
}

void Player::setFieldOfView(float fieldOfView) {
    assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
    _fieldOfView = fieldOfView;
}

float Player::nearPlane() const {
    return _nearPlane;
}

float Player::farPlane() const {
    return _farPlane;
}

void Player::setNearAndFarPlanes(float nearPlane, float farPlane) {
    assert(nearPlane > 0.0f);
    assert(farPlane > nearPlane);
    _nearPlane = nearPlane;
    _farPlane = farPlane;
}

glm::mat4 Player::orientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, _verticalAngle, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, _horizontalAngle, glm::vec3(0,1,0));
    return orientation;
}

void Player::offsetOrientation(float upAngle, float rightAngle) {
    _horizontalAngle += rightAngle;
    _verticalAngle += upAngle;
    normalizeAngles();
}

void Player::lookAt(glm::vec3 position) {
    assert(position != _position);
    glm::vec3 direction = glm::normalize(position - _position);
    _verticalAngle = RadiansToDegrees(asinf(-direction.y));
    _horizontalAngle = -RadiansToDegrees(atan2f(-direction.x, -direction.z));
    normalizeAngles();
}

float Player::viewportAspectRatio() const {
    return _viewportAspectRatio;
}

void Player::setViewportAspectRatio(float viewportAspectRatio) {
    assert(viewportAspectRatio > 0.0);
    _viewportAspectRatio = viewportAspectRatio;
}

glm::vec3 Player::forward() const {
    glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0,0,-1,1);
    return glm::vec3(forward);
}

glm::vec3 Player::right() const {
    glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

glm::vec3 Player::up() const {
    glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}

glm::mat4 Player::matrix() const {
    return projection() * view();
}

glm::mat4 Player::projection() const {
    return glm::perspective(_fieldOfView, _viewportAspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Player::view() const {
    return orientation() * glm::translate(glm::mat4(), -_position);
}

void Player::normalizeAngles() {
    _horizontalAngle = fmodf(_horizontalAngle, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if(_horizontalAngle < 0.0f)
        _horizontalAngle += 360.0f;
    
    if(_verticalAngle > MaxVerticalAngle)
        _verticalAngle = MaxVerticalAngle;
    else if(_verticalAngle < -MaxVerticalAngle)
        _verticalAngle = -MaxVerticalAngle;
}