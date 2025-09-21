#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec3 DEFAULT_POS = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);

static constexpr float DEFAULT_YAW = -90.0f;
static constexpr float DEFAULT_PITCH = 0.0f;
static constexpr float DEFAULT_SPEED = 2.5f;
static constexpr float DEFAULT_SENS = 0.1f;
static constexpr float DEFAULT_ZOOM = 45.0f;

enum MoveDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(glm::vec3 pos = DEFAULT_POS, glm::vec3 world_up = DEFAULT_UP, 
            float pitch = DEFAULT_PITCH, float yaw = DEFAULT_YAW);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
            float up_z, float pitch, float yaw);

    void set_speed(float speed);
    void set_sens(float sens);

    glm::mat4 get_view_matrix() const;
    float get_zoom() const;
    glm::vec3 get_position() const;
    glm::vec3 get_front() const;
    
    void process_movement(MoveDirection dir, float dt);
    void process_mouse(float dx, float dy, bool clamp_pitch = true);
    void process_scroll(float dy);

private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _world_up;

    float _yaw;
    float _pitch;

    float _speed;
    float _sens;
    float _zoom;

    void _update_camera_vectors();
};
