#pragma once
#include <array>
#include <glm/glm.hpp>

namespace ce {

    struct ViewProjectionMatrixs {
        glm::mat4 view;
        glm::mat4 viewProjection;
        glm::mat4 viewProjectionInverse;

        ViewProjectionMatrixs() = default;
        void update(const glm::mat4& view, const glm::mat4& projection) {
            this->view = view;
            this->viewProjection = projection * view;
            this->viewProjectionInverse = glm::inverse(view) * glm::inverse(projection);
        }
    };

    class ViewProjection {

      public:
        ViewProjection() = default;
        explicit ViewProjection(const float& noze) { this->set_noze(noze); }

        ViewProjection(const ViewProjection& o) = delete;
        ViewProjection& operator=(const ViewProjection& o) = delete;
        virtual ~ViewProjection() = default;

        float get_noze() const { return noze_; }

        void set_noze(const float& noze) {
            this->noze_ = noze;
            size_ = (noze == 0.0F) ? 1 : 2;
        }

        void set_index(const uint8_t s) { indice_ = (s >= 0 && s < 2) ? s : 0; }
        uint8_t get_size() const { return size_; }

        ViewProjectionMatrixs& get_sel() { return vpm_[indice_]; }
        ViewProjectionMatrixs& get_left() { return vpm_[0]; }  // 0
        ViewProjectionMatrixs& get_right() { return vpm_[1]; } // 1

      private:
        float noze_{0.0F};
        uint8_t indice_{0};
        uint8_t size_{1};
        std::array<ViewProjectionMatrixs, 2> vpm_;
    };
} // namespace ce
