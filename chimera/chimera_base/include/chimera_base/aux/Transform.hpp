#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ce {

    class ITrans {
      public:
        virtual ~ITrans() = default;
        virtual const glm::vec3 get_position() const = 0;
        virtual const glm::mat4 get_matrix() const = 0;
        virtual const glm::mat4 translate_src(const glm::vec3& pos) const = 0;
        // virtual const glm::vec3 getRotation() = 0; // TODO: Implementar
        virtual void set_position(const glm::vec3& pos) = 0;
        virtual void set_rotation(const glm::vec3& rot) = 0;
        virtual void set_matrix(const glm::mat4& transform) = 0;
    };

    class Transform final : public ITrans {
      public:
        Transform() = default;
        Transform(const glm::mat4& transform) : transform_(transform) {}
        virtual ~Transform() = default;
        virtual const glm::vec3 get_position() const override { return glm::vec3(this->transform_[3]); }
        virtual const glm::mat4 get_matrix() const override { return this->transform_; }
        virtual const glm::mat4 translate_src(const glm::vec3& pos) const override {
            glm::mat4 matrix_coord = transform_;
            float* matrix = glm::value_ptr(matrix_coord);
            // pega posicao do objeto horigem de desenho (viewpoint fixo),desloca desenha para o pbjeto horigem
            matrix[12] -= pos.x;
            matrix[13] -= pos.y;
            matrix[14] -= pos.z;
            return glm::make_mat4(matrix);
        }
        // void setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
        //     glm::quat myQuat(_rotation);                                    // trocar (pitch, yaw, roll) por (yaw,
        //     pitch, roll) ????? glm::mat4 matRot = glm::toMat4(myQuat);                         // matriz rotacao
        //     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
        //     transform = matRot * matTrans;                                  // primeiro translada depois rotaciona,
        //     ordem é importante!!!
        // }
        virtual void set_position(const glm::vec3& pos) override {
            this->transform_ = glm::translate(this->transform_, pos);
        }
        virtual void set_rotation(const glm::vec3& rot) override {
            transform_ = glm::eulerAngleYXZ(rot.y, rot.x, rot.z);
        }
        virtual void set_matrix(const glm::mat4& transform) override { this->transform_ = transform; }

      private:
        glm::mat4 transform_ = glm::mat4(1.0f);
    };
} // namespace ce
