#pragma once
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/Triangle.hpp"
#include <SDL3/SDL.h>
#include <string>

namespace ce {

    class LoadHeightMap {

      public:
        LoadHeightMap(int square_x, int square_z);
        virtual ~LoadHeightMap();
        void clean();

        bool get_mesh(const std::string& file_name, Mesh& mesh, const glm::vec3& size);
        void split(TrisIndex& vertex_index_in, std::vector<TrisIndex>& v_tris_index_out) const;

      private:
        uint32_t get_index(const uint32_t& x, const uint32_t& z) { return (p_image_->w * z) + x; }
        uint32_t getpixel(const uint32_t& w, const uint32_t& h);
        uint32_t get_height(const uint32_t& w, const uint32_t& h);
        glm::vec3 define_scale(const glm::vec3& size);
        // glm::vec3 calcNormalHeight(int x, int z);

      private:
        SDL_Surface* p_image_{nullptr};
        uint32_t width_{0}, height_{0}, square_x_{0}, square_z_{0}, minimal_{0};
    };
} // namespace ce
