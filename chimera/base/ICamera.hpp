#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace ce {

    constexpr float fsp_camera_max_speed{40.0};
    constexpr float fsp_camera_rotation_sensitivity{0.3};
    constexpr float camera_max_fov{45.0};

    enum class CamKind { FPS = 0, ORBIT = 1, STATIC = 3 };

    class Camera {

      protected:
        glm::vec3 position{glm::vec3(0, 0, 0)};
        glm::mat4 projection{glm::mat4(1.0f)};

      public:
        const glm::mat4& getProjection() const { return projection; }

        const glm::vec3& getPosition() const { return position; }

        void setPosition(const glm::vec3& position) { this->position = position; }

        virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;

        virtual bool isOrtho() const = 0;
    };

    class CameraOrtho : public Camera {

      private:
        float xsize{0.0f}, ysize{0.0f}, xmag{800.0f}, ymag{600.0f}, near{0.1f}, far{1000.0f};

      public:
        CameraOrtho(const float& xmag, const float& ymag, const float& near, const float& far)
            : xmag(xmag), ymag(ymag), near(near), far(far) {}

        virtual ~CameraOrtho() = default;

        void setViewportSize(const uint32_t& width, const uint32_t& height) override {
            float halfAspectRatio = ((float)width / (float)height) * 0.5f;
            xsize = xmag * halfAspectRatio;
            ysize = ymag * 0.5f;
            projection = glm::ortho(-xsize, xsize, -ysize, ysize, near, far);
        }

        bool isOrtho() const override { return true; }

        const glm::vec2 getSize() const { return glm::vec2(xsize, ysize); }
    };

    class CameraPerspective : public Camera {

      private:
        float fov{camera_max_fov}, near{0.1f}, far{1000.0f};

      public:
        CameraPerspective(const float& fov, const float& near, const float& far) : fov(fov), near(near), far(far) {}

        virtual ~CameraPerspective() = default;

        void setViewportSize(const uint32_t& width, const uint32_t& height) override {
            projection = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
        }

        bool isOrtho() const override { return false; }
    };
} // namespace ce
