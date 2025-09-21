#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 world_up,
        float pitch, float yaw) : _front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                  _speed(DEFAULT_SPEED), 
                                  _sens(DEFAULT_SENS), 
                                  _zoom(DEFAULT_ZOOM),
                                  _pitch(pitch),
                                  _yaw(yaw),
                                  _position(pos), 
                                  _world_up(world_up) {
    _update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
        float up_z, float pitch, float yaw) : _front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                              _speed(DEFAULT_SPEED),
                                              _sens(DEFAULT_SENS),
                                              _zoom(DEFAULT_ZOOM),
                                              _pitch(pitch),
                                              _yaw(yaw),
                                              _position(glm::vec3(pos_x, pos_y, pos_z)),
                                              _world_up(glm::vec3(up_x, up_y, up_z)) {
    _update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::_update_camera_vectors() {
    _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front.y = sin(glm::radians(_pitch));
    _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(_front);

    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::process_movement(MoveDirection dir, float dt) {
    float u = _speed * dt;
    
    switch (dir) {
        case FORWARD:
            _position += u * _front;
            break;
        case BACKWARD:
            _position -= u * _front;
            break;
        case LEFT:
            _position -= u * _right;
            break;
        case RIGHT:
            _position += u * _right;
            break;
    }
}

void Camera::process_mouse(float dx, float dy, bool clamp_pitch) {
    _yaw += dx * _sens;
    _pitch -= dy * _sens;

    if (clamp_pitch) {
        _pitch = std::min(89.0f, _pitch);
        _pitch = std::max(-89.0f, _pitch);
    }

    _update_camera_vectors();
}

void Camera::process_scroll(float dy) {
    _zoom -= dy;
    _zoom = std::max(1.0f, _zoom);
    _zoom = std::min(45.0f, _zoom);
}

float Camera::get_zoom() const {
    return _zoom;
}

glm::vec3 Camera::get_position() const {
    return _position;
}

glm::vec3 Camera::get_front() const {
    return _front;
}

void Camera::set_speed(float speed) {
    _speed = speed;
}

void Camera::set_sens(float sens) {
    _sens = sens;
}
