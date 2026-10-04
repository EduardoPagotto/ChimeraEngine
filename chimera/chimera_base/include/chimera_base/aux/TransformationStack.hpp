#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ce {

    class TransformationStack {
      public:
        TransformationStack() {
            transformation_stack_.push_back(glm::mat4(1.0f));
            trans_cache_ = &transformation_stack_.back();
        }

        inline void push(const glm::mat4& matrix) {
            transformation_stack_.push_back(transformation_stack_.back() * matrix);
            trans_cache_ = &transformation_stack_.back();
        }

        inline void push_over(const glm::mat4& matrix) {
            transformation_stack_.push_back(matrix);
            trans_cache_ = &transformation_stack_.back();
        }

        inline const glm::vec3 multipl_vec3(const glm::vec3& point) const {
            return glm::vec3((*trans_cache_) * glm::vec4(point, 1.0f));
        }

        inline void pop() {
            if (transformation_stack_.size() > 1)
                transformation_stack_.pop_back();

            trans_cache_ = &transformation_stack_.back();
            // TODO: log here!!!
        }

        inline glm::mat4 const get() const { return *trans_cache_; }

      private:
        std::vector<glm::mat4> transformation_stack_;
        const glm::mat4* trans_cache_;
    };
} // namespace ce
