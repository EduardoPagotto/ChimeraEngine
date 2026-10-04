#pragma once
#include "VertexBuffer.hpp"
#include <memory>

namespace ce {

    class VertexArray {

      public:
        VertexArray();
        virtual ~VertexArray();

        void bind() const;
        static void unbind();

        inline void push(std::shared_ptr<VertexBuffer> buffer) { this->vbos_.push_back(buffer); }
        inline std::shared_ptr<VertexBuffer> get_buffer(const uint32_t& index) const { return vbos_[index]; }
        inline std::shared_ptr<VertexBuffer> get_last() const { return vbos_.back(); }

      private:
        uint32_t array_id_ = 0;
        std::vector<std::shared_ptr<VertexBuffer>> vbos_;
    };
} // namespace ce
