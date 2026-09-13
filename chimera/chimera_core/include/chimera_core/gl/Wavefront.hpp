#pragma once
#include "chimera_core/gl/Material.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include <entt/entt.hpp>

namespace ce {
    class WaveFront {
      public:
        explicit WaveFront(std::shared_ptr<entt::registry> registry) : registry(registry) {}

        void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath);
        void wavefrontMtlLoad(const std::string& path, std::shared_ptr<Material> material);

      private:
        std::shared_ptr<entt::registry> registry;
    };
} // namespace ce
