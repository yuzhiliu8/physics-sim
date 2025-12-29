#include "camera.hpp"

Camera::Camera(
    const glm::vec3& pos, 
    float aspect_ratio,
    float yaw,
    float pitch,
    float fov){
    cam_pos_ = pos;
    cam_up_ = glm::vec3(0.0, 1.0, 0.0); //initially upright, no roll or pitch
    yaw_ = yaw;
    pitch_ = pitch;
    fov_ = fov;
    aspect_ratio_ = aspect_ratio;
}

glm::mat4 Camera::view(){
    update_front();
    return glm::lookAt(cam_pos_, cam_pos_ + cam_front_, cam_up_);
}

glm::mat4 Camera::projection(){
    return glm::perspective(glm::radians(fov_), aspect_ratio_, 0.1f, 10000.0f);
}

void Camera::update_front(){
    cam_front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    cam_front_.y = sin(glm::radians(pitch_));
    cam_front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    cam_front_ = glm::normalize(cam_front_);
}

void Camera::forward(float d){
    cam_pos_ += d*cam_front_;
}
void Camera::backward(float d){
    cam_pos_ -= d*cam_front_;
}
void Camera::right(float d){
    cam_pos_ += d*glm::normalize(glm::cross(cam_front_, cam_up_));
}
void Camera::left(float d){
    cam_pos_ -= d*glm::normalize(glm::cross(cam_front_, cam_up_));
}
void Camera::up(float d){
    cam_pos_ += d*cam_up_;
}
void Camera::down(float d){
    cam_pos_ -= d*cam_up_;
}
void Camera::set_aspect_ratio(float ar){
    aspect_ratio_ = ar;
}

void Camera::acc_yaw(float x_offset){
    yaw_ += x_offset * SENSITIVITY;
}
void Camera::acc_pitch(float y_offset){
    pitch_ += y_offset * SENSITIVITY;
    if (pitch_ > 89.0){
        pitch_ = 89.0;
    } else if (pitch_ < -89.0){
        pitch_ = -89.0;
    }
}