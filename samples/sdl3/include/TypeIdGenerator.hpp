#pragma once
#include <cstdint>

namespace ce {

    using ResourceTypeId = std::uint32_t;

    class TypeIdGenerator {
      private:
        static inline ResourceTypeId nextId = 0;

      public:
        template <typename T>
        static ResourceTypeId Get() {
            static const ResourceTypeId id = nextId++;
            return id;
        }
    };

} // namespace ce
