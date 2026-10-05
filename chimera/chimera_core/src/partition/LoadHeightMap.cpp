#include "chimera_core/partition/LoadHeightMap.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <cstddef>

namespace ce {

    LoadHeightMap::LoadHeightMap(int square_x, int square_z)
        : p_image_(nullptr), square_x_(square_x), square_z_(square_z) {
        clean();
    }

    LoadHeightMap::~LoadHeightMap() { clean(); }

    void LoadHeightMap::clean() {
        if (p_image_ != nullptr) {
            SDL_DestroySurface(p_image_);
            p_image_ = nullptr;
        }
    }

    uint32_t LoadHeightMap::get_height(const uint32_t& w, const uint32_t& h) {
        const uint32_t w1{w > p_image_->w ? p_image_->w : w};
        const uint32_t h1{h > p_image_->h ? p_image_->h : h};
        return getpixel(w1, h1);
    }

    uint32_t LoadHeightMap::getpixel(const uint32_t& w, const uint32_t& h) {

        const SDL_PixelFormatDetails* detail = SDL_GetPixelFormatDetails(p_image_->format);
        const int bpp{detail->bytes_per_pixel};

        uint8_t* p{(uint8_t*)p_image_->pixels + (static_cast<size_t>(h * p_image_->pitch)) +
                   (static_cast<size_t>(w * bpp))};

        switch (bpp) {
            case 1:
                return *p;
                break;

            case 2:
                return *(uint16_t*)p;
                break;

            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    return p[0] << 16 | p[1] << 8 | p[2];
                } else {
                    return p[0] | p[1] << 8 | p[2] << 16;
                }
                break;

            case 4:
                return *(uint32_t*)p;
                break;

            default:
                return 0; /* shouldn't happen, but avoids warnings */
        }
    }

    glm::vec3 LoadHeightMap::define_scale(const glm::vec3& size) {
        uint32_t max;
        minimal_ = max = get_height(0, 0);
        for (uint32_t z{0}; z < p_image_->h; z++) {
            for (uint32_t x{0}; x < p_image_->w; x++) {
                const uint32_t val{get_height(x, z)};

                max = std::max(val, max);
                minimal_ = std::min(val, minimal_);
            }
        }
        return glm::vec3(size.x / (float)p_image_->w, size.y / (float)(max - minimal_), size.z / (float)p_image_->h);
    }

    // glm::vec3 LoadHeightMap::calcNormalHeight(uint32_t x, uint32_t z) {
    //     return glm::normalize(glm::vec3((scale.y * getHeight(x - 1, z)) - (scale.y * getHeight(x + 1, z)),   // norx
    //                                     2.0f,                                                                // nory
    //                                     (scale.y * getHeight(x, z - 1)) - (scale.y * getHeight(x, z + 1)))); // norz
    // }

    bool LoadHeightMap::get_mesh(const std::string& file_name, Mesh& mesh, const glm::vec3& size) {

        p_image_ = IMG_Load(file_name.c_str());
        if (p_image_ == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", file_name.c_str());
            return false;
        }

        const float half_h{static_cast<float>(p_image_->h) / 2.0F};
        const float hald_w{static_cast<float>(p_image_->w) / 2.0F};
        const float v{1.0F / static_cast<float>(p_image_->h - 1)};
        const float u{1.0F / static_cast<float>(p_image_->w - 1)};
        const glm::vec3 scale = define_scale(size);

        for (uint32_t z = 0; z < p_image_->h; z++) {
            for (uint32_t x = 0; x < p_image_->w; x++) {

                // point, normal, UV
                mesh.vertex.push_back({glm::vec3(static_cast<float>(x) - hald_w, get_height(x, z) - minimal_,
                                                 half_h - static_cast<float>(z)) *
                                           scale,
                                       glm::vec3(0.0F),
                                       glm::vec2(u * static_cast<float>(x), v * static_cast<float>(z))});
            }
        }

        const uint32_t tot_h{static_cast<uint32_t>(p_image_->h - 1)};
        const uint32_t tot_w{static_cast<uint32_t>(p_image_->w - 1)};

        for (uint32_t z{0}; z < tot_h; z++) {
            for (uint32_t x{0}; x < tot_w; x++) {
                // triangles point
                const uint32_t pa{get_index(x, z)};
                const uint32_t pb{get_index(x + 1, z)};
                const uint32_t pc{get_index(x + 1, z + 1)};
                const uint32_t pd{get_index(x, z + 1)};
                // Face index
                mesh.iFace.push_back({pa, pb, pc}); // T1
                mesh.iFace.push_back({pc, pd, pa}); // T2
            }
        }

        // Calcula normal apos todo o mapeamento de altura
        for (uint32_t i{0}; i < mesh.iFace.size(); i++) {

            const glm::vec3& pa = mesh.vertex[mesh.iFace[i].x].point;
            const glm::vec3& pb = mesh.vertex[mesh.iFace[i].y].point;
            const glm::vec3& pc = mesh.vertex[mesh.iFace[i].z].point;
            const glm::vec3 vn = glm::normalize(glm::cross(pb - pa, pc - pa)); // CROSS(U,V)

            mesh.vertex[mesh.iFace[i].x].normal = vn;
            mesh.vertex[mesh.iFace[i].y].normal = vn;
            mesh.vertex[mesh.iFace[i].z].normal = vn;
        }

        mesh_debug(mesh, false);

        this->width_ = p_image_->w;
        this->height_ = p_image_->h;

        return true;
    }

    void LoadHeightMap::split(TrisIndex& vertex_index_in, std::vector<TrisIndex>& v_tris_index_out) const {

        bool done{false};
        uint32_t start_height{0};
        uint32_t start_width{0};
        uint32_t contador{0};
        const uint32_t total_height{(height_ - 1) * 2};
        const uint32_t total_width{(width_ - 1) * 2};
        const uint32_t square_height{square_z_};
        const uint32_t square_width{square_x_ * 2};
        const uint32_t threshold_widht{total_height * square_z_};

        while (!done) {

            uint32_t end_height = start_height + square_height;
            uint32_t end_width = start_width + square_width;
            const uint32_t teste_a = (start_height * total_height) + start_width;

            if (teste_a >= vertex_index_in.size()) { // all faces
                done = true;
                continue;
            }

            end_height = std::min(end_height, height_ - 1);
            end_width = std::min(end_width, total_width);

            TrisIndex node;

            uint32_t face;                                           //, base;
            for (uint32_t h = start_height; h < end_height; h++) {   // z
                for (uint32_t w = start_width; w < end_width; w++) { // x
                    face = ((h * total_height) + w);
                    // base = face * 3;
                    node.push_back(
                        glm::uvec3(vertex_index_in[face].x, vertex_index_in[face].y, vertex_index_in[face].z));
                    contador++;
                }
            }

            if (contador >= threshold_widht) {
                start_height = end_height;
                contador = 0;
                start_width = 0;
            } else {
                start_width = end_width;
            }

            if (node.size() != 0) {
                v_tris_index_out.push_back(node);
            } else {
                done = true;
            }
            // node.debugDados();
        }
    }
} // namespace ce
