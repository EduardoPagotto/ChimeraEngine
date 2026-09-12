#pragma once
#include "VertexBuffer.hpp"
#include <memory>

namespace ce {

    class VertexArray {

      private:
        uint32_t arrayID = 0;
        std::vector<std::shared_ptr<VertexBuffer>> vbos;

      public:
        VertexArray();

        virtual ~VertexArray();

        void bind() const;

        static void unbind();

        inline void push(std::shared_ptr<VertexBuffer> buffer) { this->vbos.push_back(buffer); }

        inline std::shared_ptr<VertexBuffer> getBuffer(const uint32_t& index) const { return vbos[index]; }

        inline std::shared_ptr<VertexBuffer> getLast() const { return vbos.back(); }
    };
} // namespace ce
