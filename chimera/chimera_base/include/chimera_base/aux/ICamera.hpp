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
        glm::mat4 projection{glm::mat4(1.0F)};

      public:
        const glm::mat4& get_projection() const { return projection; }
        const glm::vec3& get_position() const { return position; }
        void set_position(const glm::vec3& position) { this->position = position; }

        virtual void set_viewport_size(const uint32_t& width, const uint32_t& height) = 0;
        virtual bool is_ortho() const = 0;
    };

    class CameraOrtho final : public Camera {

      public:
        CameraOrtho(const float& xmag, const float& ymag, const float& near, const float& far)
            : xmag_(xmag), ymag_(ymag), near_(near), far_(far) {}

        virtual ~CameraOrtho() = default;

        void set_viewport_size(const uint32_t& width, const uint32_t& height) override {
            float half_aspect_ratio = ((float)width / (float)height) * 0.5F;
            xsize_ = xmag_ * half_aspect_ratio;
            ysize_ = ymag_ * 0.5F;
            projection = glm::ortho(-xsize_, xsize_, -ysize_, ysize_, near_, far_);
        }

        bool is_ortho() const override { return true; }

        glm::vec2 get_size() const { return glm::vec2(xsize_, ysize_); }

      private:
        float xsize_{0.0F};
        float ysize_{0.0F};
        float xmag_{800.0F};
        float ymag_{600.0F};
        float near_{0.1F};
        float far_{1000.0F};
    };

    class CameraPerspective final : public Camera {

      public:
        CameraPerspective(const float& fov, const float& near, const float& far) : fov_(fov), near_(near), far_(far) {}

        virtual ~CameraPerspective() = default;

        void set_viewport_size(const uint32_t& width, const uint32_t& height) override {
            projection = glm::perspective(glm::radians(fov_), (float)width / (float)height, near_, far_);
        }

        bool is_ortho() const override { return false; }

      private:
        float fov_{camera_max_fov};
        float near_{0.1F};
        float far_{1000.0F};
    };
} // namespace ce
