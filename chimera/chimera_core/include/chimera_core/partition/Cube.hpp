#pragma once
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/AABB.hpp"
#include <vector>

namespace ce {

    enum class CARDINAL {
        NORTH = 0,      //
        NORTH_EAST = 1, // RIGHT
        EAST = 2,       // RIGHT
        SOUTH_EAST = 3, // RIGHT
        SOUTH = 4,      //
        SOUTH_WEST = 5, // LEFT
        WEST = 6,       // LEFT
        NORTH_WEST = 7, // LEFT
        NONE = 8
    };

    enum class DEEP {
        UP = 0,     // ABOVE
        MIDDLE = 1, // LEVEL
        DOWN = 2    // UNDER
    };

    enum class SPACE {
        EMPTY = 0,
        SOLID = 1,
        DIAG = 2,
        FLOOR = 3,
        CEILING = 4,
        FC = 5,
        RAMP_FNS = 6,
        RAMP_FEW = 7,
        INVALID = 99
    };

    class Cube : public AABB {
      public:
        Cube(const char& caracter, const glm::vec3& min, const glm::vec3& max);
        virtual ~Cube() = default;
        void set_neighbor(DEEP deep, CARDINAL card, Cube* p_cube);
        void create(Mesh* mesh);

        inline const SPACE get_space() const { return this->space_; }

        inline bool empty_space() const {
            return ((this->space_ == SPACE::EMPTY) || (this->space_ == SPACE::FLOOR) ||
                    (this->space_ == SPACE::CEILING) || (this->space_ == SPACE::FC));
        }

        CARDINAL empty_quadrant_diag(DEEP deep, bool invert);
        const bool has_neighbor(DEEP deep, CARDINAL card, SPACE space);

        void new_flat_floor_ceeling(bool is_floor, CARDINAL card);
        void add_face(bool clockwise, int num_face, int num_tex);

      private:
        void new_wall();
        void new_ramp(bool is_floor, CARDINAL card);
        void new_diag();
        void new_floor();
        void new_ceeling();
        void new_ramp_nsew(SPACE space);

        Cube* p_north_{nullptr};
        Cube* p_east_{nullptr};
        Cube* p_south_{nullptr};
        Cube* p_west_{nullptr};
        Cube* p_up_{nullptr};
        Cube* p_down_{nullptr};
        Mesh* mesh_{nullptr};
        SPACE space_;
    };

    void init_cube_base();
    void cleanup_cube_base();
    glm::ivec3 get_cardinal_pos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos);
    glm::vec3 minimal(const float& size_block, const glm::vec3 half_block, const glm::ivec3& pos);
    uint32_t get_index_array_pos(const glm::ivec3& pos, const glm::ivec3& size);
    Cube* get_cube_neighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos, const glm::ivec3& size,
                            std::vector<Cube*>& vp_cube);
    void link_cubes(const glm::ivec3& size, std::vector<Cube*>& vp_cube);
} // namespace ce
