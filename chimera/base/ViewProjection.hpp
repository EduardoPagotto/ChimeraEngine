#pragma once
#include "ServiceLocator.hpp"
#include <array>
#include <glm/glm.hpp>

namespace ce {

    struct ViewProjectionMatrixs {
        glm::mat4 view, viewProjection, viewProjectionInverse;
        ViewProjectionMatrixs() = default;
        void update(const glm::mat4& _view, const glm::mat4& _projection) {
            view = _view;
            viewProjection = _projection * _view;
            viewProjectionInverse = glm::inverse(_view) * glm::inverse(_projection);
        }
    };

    class ViewProjection : public IService {

      private:
        float noze{0.0f};
        uint8_t indice{0};
        uint8_t size{1};
        std::array<ViewProjectionMatrixs, 2> vpm;

      public:
        explicit ViewProjection() = default;

        explicit ViewProjection(const float& noze) { this->setNoze(noze); }

        ViewProjection(const ViewProjection& o) = delete;

        ViewProjection& operator=(const ViewProjection& o) = delete;

        virtual ~ViewProjection() = default;

        std::type_index getTypeIndex() const override { return std::type_index(typeid(ViewProjection)); }

        const float getNoze() const { return noze; }

        void setNoze(const float& noze) {
            this->noze = noze;
            size = (noze == 0.0f) ? 1 : 2;
        }

        void setIndex(const uint8_t s) { indice = (s >= 0 && s < 2) ? s : 0; }

        const uint8_t getSize() const { return size; }

        ViewProjectionMatrixs& getSel() { return vpm[indice]; }

        ViewProjectionMatrixs& getLeft() { return vpm[0]; } // 0

        ViewProjectionMatrixs& getRight() { return vpm[1]; } // 1
    };
} // namespace ce
