#pragma once
#include "chimera_core/gl/Material.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include <entt/entt.hpp>

namespace ce {
    class WaveFront {
      public:
        explicit WaveFront(std::shared_ptr<entt::registry> registry) : registry_(registry) {}

        void wavefront_obj_load(const std::string& path, Mesh* mesh, std::string& file_math);
        void wavefront_mtl_load(const std::string& path, std::shared_ptr<Material> material);

      private:
        std::shared_ptr<entt::registry> registry_;
    };
} // namespace ce
