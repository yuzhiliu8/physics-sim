#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
    public:
        Camera(
            const glm::vec3& pos, 
            float aspect_ratio,
            float yaw = -90.0,
            float pitch = 0.0,
            float fov = 45.0f
        );
        glm::mat4 view();
        glm::mat4 projection();
        void update_front();
        void forward(float d);
        void backward(float d);
        void left(float d);
        void right(float d);
        void up(float d);
        void down(float d);
        void acc_yaw(float x_offset);
        void acc_pitch(float y_offset);
        void set_aspect_ratio(float ar);

    private:
        glm::vec3 cam_pos_;
        glm::vec3 cam_front_;
        glm::vec3 cam_up_;
        float yaw_;
        float pitch_;
        float fov_;
        float aspect_ratio_;

        const float SENSITIVITY = 0.1;


};

#endif