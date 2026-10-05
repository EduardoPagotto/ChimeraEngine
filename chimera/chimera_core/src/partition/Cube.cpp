#include "chimera_core/partition/Cube.hpp"

namespace ce {

    static TrisIndex t_vert_index;
    static TrisIndex t_tex_index;
    static std::vector<glm::vec2> t_tex_seq;

    void initCubeBase() {
        t_vert_index.push_back(glm::uvec3(0, 1, 3)); // f00 N0
        t_vert_index.push_back(glm::uvec3(1, 3, 2)); // f01 N1
        t_vert_index.push_back(glm::uvec3(3, 2, 0)); // f02 N2
        t_vert_index.push_back(glm::uvec3(2, 0, 1)); // f03 N3

        t_vert_index.push_back(glm::uvec3(1, 5, 7)); // f04 E0
        t_vert_index.push_back(glm::uvec3(5, 7, 3)); // f05 E1 RPNEU
        t_vert_index.push_back(glm::uvec3(7, 3, 1)); // f06 E2
        t_vert_index.push_back(glm::uvec3(3, 1, 5)); // f07 E3 RPNED

        t_vert_index.push_back(glm::uvec3(5, 4, 6)); // f08 S0
        t_vert_index.push_back(glm::uvec3(4, 6, 7)); // f09 S1
        t_vert_index.push_back(glm::uvec3(6, 7, 5)); // f10 S2
        t_vert_index.push_back(glm::uvec3(7, 5, 4)); // f11 S3

        t_vert_index.push_back(glm::uvec3(4, 0, 2)); // f12 W0 RPNWD
        t_vert_index.push_back(glm::uvec3(0, 2, 6)); // f13 W1
        t_vert_index.push_back(glm::uvec3(2, 6, 4)); // f14 W2 RPWU
        t_vert_index.push_back(glm::uvec3(6, 4, 0)); // f15 W3

        t_vert_index.push_back(glm::uvec3(7, 6, 2)); // f16 U0
        t_vert_index.push_back(glm::uvec3(6, 2, 3)); // f17 U1
        t_vert_index.push_back(glm::uvec3(2, 3, 7)); // f18 U2
        t_vert_index.push_back(glm::uvec3(3, 7, 6)); // f19 U3

        t_vert_index.push_back(glm::uvec3(4, 5, 1)); // f20 D0
        t_vert_index.push_back(glm::uvec3(5, 1, 0)); // f21 D1
        t_vert_index.push_back(glm::uvec3(1, 0, 4)); // f22 D2
        t_vert_index.push_back(glm::uvec3(0, 4, 5)); // f23 D3
                                                     //--
        t_vert_index.push_back(glm::uvec3(0, 5, 7)); // f24 DIA1
        t_vert_index.push_back(glm::uvec3(7, 2, 0)); // f25 DIA1
        t_vert_index.push_back(glm::uvec3(4, 1, 3)); // f26 DIA2
        t_vert_index.push_back(glm::uvec3(3, 6, 4)); // f27 DIA2

        t_vert_index.push_back(glm::uvec3(4, 5, 3)); // f28 RP NS
        t_vert_index.push_back(glm::uvec3(3, 2, 4)); // f29 RP NS
        t_vert_index.push_back(glm::uvec3(0, 4, 7)); // f30 RP EW
        t_vert_index.push_back(glm::uvec3(7, 3, 0)); // f31 RP EW

        t_vert_index.push_back(glm::uvec3(1, 0, 6)); // f32 RP SN
        t_vert_index.push_back(glm::uvec3(6, 7, 1)); // f33 RP SN
        t_vert_index.push_back(glm::uvec3(5, 1, 2)); // f34 RP WE
        t_vert_index.push_back(glm::uvec3(2, 6, 5)); // f35 RP WE

        //---
        t_tex_index.push_back(glm::uvec3(0, 2, 3)); // T0 Q0
        t_tex_index.push_back(glm::uvec3(3, 1, 0)); // T1 Q2
        t_tex_index.push_back(glm::uvec3(2, 0, 1)); // T2 !Q0
        t_tex_index.push_back(glm::uvec3(1, 3, 2)); // T3 !Q2

        t_tex_index.push_back(glm::uvec3(1, 0, 2)); // T4 F Q1(NW)
        t_tex_index.push_back(glm::uvec3(2, 3, 1)); // T5 F Q3(SE)
        t_tex_index.push_back(glm::uvec3(2, 0, 1)); // T6 c Q1(NW)
        t_tex_index.push_back(glm::uvec3(1, 3, 2)); // T7 c

        t_tex_index.push_back(glm::uvec3(3, 2, 0)); // T8 c
        t_tex_index.push_back(glm::uvec3(0, 1, 3)); // T9 c

        // ---
        t_tex_seq.push_back(glm::vec2(0, 0)); // 0
        t_tex_seq.push_back(glm::vec2(0, 1)); // 1
        t_tex_seq.push_back(glm::vec2(1, 0)); // 2
        t_tex_seq.push_back(glm::vec2(1, 1)); // 3
    }

    void cleanupCubeBase() {
        t_vert_index.clear();
        t_tex_index.clear();
        t_tex_seq.clear();
    }

    glm::ivec3 getCardinalPos(DEEP deep, CARDINAL card, const glm::ivec3& dist, glm::ivec3 const& pos) {
        glm::ivec3 val{pos};
        switch (deep) {
            case DEEP::UP:
                val.y = pos.y + dist.y;
                break;
            case DEEP::MIDDLE:
                val.y = pos.y;
                break;
            case DEEP::DOWN:
                val.y = pos.y - dist.y;
            default:
                break;
        }

        switch (card) {
            case CARDINAL::NORTH:
                val.z = pos.z - dist.z;
                break;
            case CARDINAL::NORTH_EAST:
                val.z = pos.z - dist.z;
                val.x = pos.x + dist.x;
                break;
            case CARDINAL::EAST:
                val.x = pos.x + dist.x;
                break;
            case CARDINAL::SOUTH_EAST:
                val.z = pos.z + dist.z;
                val.x = pos.x + dist.x;
                break;
            case CARDINAL::SOUTH:
                val.z = pos.z + dist.z;
                break;
            case CARDINAL::SOUTH_WEST:
                val.z = pos.z + dist.z;
                val.x = pos.x - dist.x;
                break;
            case CARDINAL::WEST:
                val.x = pos.x - dist.x;
                break;
            case CARDINAL::NORTH_WEST:
                val.z = pos.z - dist.z;
                val.x = pos.x - dist.x;
                break;
            default:
                break;
        }
        return val;
    }

    Cube* getCubeNeighbor(DEEP deep, CARDINAL card, glm::ivec3 const& pos, const glm::ivec3& size,
                          std::vector<Cube*>& vp_cube) {

        const glm::ivec3 val{getCardinalPos(deep, card, glm::ivec3(1), pos)};
        // get Valid position
        if ((val.z >= 0) && (val.z < size.z) && (val.x >= 0) && (val.x < size.x) && (val.y >= 0) && (val.y < size.y))
            return vp_cube[getIndexArrayPos(val, size)];

        return nullptr;
    }

    glm::vec3 minimal(const float& size_block, const glm::vec3 half_block, const glm::ivec3& pos) {
        const float x_min{(pos.x * size_block) - half_block.x}; // (width++)
        const float y_min{(pos.y * size_block) - half_block.y}; // Altura Minima andar
        const float z_min{(pos.z * size_block) - half_block.z}; // (height--)
        return glm::vec3(x_min, y_min, z_min);
    }

    uint32_t getIndexArrayPos(const glm::ivec3& pos, const glm::ivec3& size) {
        return pos.x + (pos.z * size.x) + (pos.y * size.x * size.z);
    }

    Cube::Cube(const char& caracter, const glm::vec3& min, const glm::vec3& max)
        : AABB(min, max), p_north_(nullptr), p_east_(nullptr), p_south_(nullptr), p_west_(nullptr), p_up_(nullptr),
          p_down_(nullptr) {

        space_ = (caracter == 0x20) ? SPACE::EMPTY : (SPACE)(caracter - 0x30);
    }

    void linkCubes(const glm::ivec3& size, std::vector<Cube*>& vp_cube) {
        glm::ivec3 pos(0);
        for (pos.y = 0; pos.y < size.y; pos.y++) {
            for (pos.z = 0; pos.z < size.z; pos.z++) {
                for (pos.x = 0; pos.x < size.x; pos.x++) {
                    Cube* p_cube = vp_cube[getIndexArrayPos(pos, size)];

                    Cube* p_before = getCubeNeighbor(DEEP::DOWN, CARDINAL::NONE, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::DOWN, CARDINAL::NONE, p_before);

                    p_before = getCubeNeighbor(DEEP::UP, CARDINAL::NONE, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::UP, CARDINAL::NONE, p_before);

                    p_before = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::NORTH, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::MIDDLE, CARDINAL::NORTH, p_before);

                    p_before = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::EAST, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::MIDDLE, CARDINAL::EAST, p_before);

                    p_before = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::SOUTH, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::MIDDLE, CARDINAL::SOUTH, p_before);

                    p_before = getCubeNeighbor(DEEP::MIDDLE, CARDINAL::WEST, pos, size, vp_cube);
                    if (p_before != nullptr)
                        p_cube->set_neighbor(DEEP::MIDDLE, CARDINAL::WEST, p_before);
                }
            }
        }
    }

    //-----

    Cube::~Cube() {}

    void Cube::set_neighbor(DEEP deep, CARDINAL card, Cube* p_cube) {

        switch (deep) {
            case DEEP::UP: {
                if (card == CARDINAL::NONE)
                    this->p_up_ = p_cube;
            } break;

            case DEEP::DOWN:
                this->p_down_ = p_cube;
                break;

            case DEEP::MIDDLE: {

                switch (card) {
                    case CARDINAL::NORTH:
                        this->p_north_ = p_cube;
                        break;

                    case CARDINAL::EAST:
                        this->p_east_ = p_cube;
                        break;

                    case CARDINAL::SOUTH:
                        this->p_south_ = p_cube;
                        break;

                    case CARDINAL::WEST:
                        this->p_west_ = p_cube;
                        break;
                    default:
                        break;
                }
            }
            default:
                break;
        }
    }

    void Cube::add_face(bool clockwise, int num_face, int num_tex) {

        const glm::uvec3 tri{t_vert_index[num_face]}; // Face index
        const glm::uvec3 tex{t_tex_index[num_tex]};   // Texture index

        const glm::vec3 va{vertex[tri.x]}; // Point A
        const glm::vec3 vb{vertex[tri.y]}; // Point B
        const glm::vec3 vc{vertex[tri.z]}; // Point C

        const glm::vec2 ta{t_tex_seq[tex.x]}; // Tex point A
        const glm::vec2 tb{t_tex_seq[tex.y]}; // Tex point B
        const glm::vec2 tc{t_tex_seq[tex.z]}; // Tex point C

        uint32_t ia, ib, ic;
        if (!clockwise) {
            ia = mesh_->iFace.size() * 3; // tl->size() * 3;
            ib = ia + 1;
            ic = ib + 1;
        } else {
            ic = mesh_->iFace.size() * 3; // tl->size() * 3;
            ib = ic + 1;
            ia = ib + 1;
        }

        const glm::vec3 vn = glm::normalize(glm::cross(vb - va, vc - va)); // CROSS(U,V)
        mesh_->vertex.push_back({va, vn, ta});
        mesh_->vertex.push_back({vb, vn, tb});
        mesh_->vertex.push_back({vc, vn, tc});

        mesh_->iFace.push_back(glm::uvec3(ia, ib, ic)); // Face
    }

    CARDINAL Cube::empty_quadrant_diag(DEEP deep, bool invert) {
        Cube* p_val = nullptr;

        switch (deep) {
            case DEEP::UP:
                p_val = this->p_up_;
                break;
            case DEEP::MIDDLE:
                p_val = this;
                break;
            case DEEP::DOWN:
                p_val = this->p_down_;
                break;
        }

        if (p_val != nullptr) {

            const bool is_n{(p_val->p_north_ != nullptr) ? p_val->p_north_->empty_space() : false};
            const bool is_e{(p_val->p_east_ != nullptr) ? p_val->p_east_->empty_space() : false};
            const bool is_s{(p_val->p_south_ != nullptr) ? p_val->p_south_->empty_space() : false};
            const bool is_w{(p_val->p_west_ != nullptr) ? p_val->p_west_->empty_space() : false};

            if (is_n && is_e)
                return (!invert) ? CARDINAL::NORTH_EAST : CARDINAL::SOUTH_WEST;

            if (is_s && is_e)
                return (!invert) ? CARDINAL::SOUTH_EAST : CARDINAL::NORTH_WEST;

            if (is_s && is_w)
                return (!invert) ? CARDINAL::SOUTH_WEST : CARDINAL::NORTH_EAST;

            if (is_n && is_w)
                return (!invert) ? CARDINAL::NORTH_WEST : CARDINAL::SOUTH_EAST;
        }

        return CARDINAL::NONE;
    }

    const bool Cube::has_neighbor(DEEP deep, CARDINAL card, SPACE space) {
        Cube* p_val = nullptr;

        switch (deep) {
            case DEEP::UP:
                p_val = this->p_up_;
                break;
            case DEEP::MIDDLE:
                p_val = this;
                break;
            case DEEP::DOWN:
                p_val = this->p_down_;
                break;
        }

        if (p_val != nullptr) {

            const bool v_n{(p_val->p_north_ != nullptr) ? (p_val->p_north_->get_space() == space) : false};
            const bool v_e{(p_val->p_east_ != nullptr) ? (p_val->p_east_->get_space() == space) : false};
            const bool v_s{(p_val->p_south_ != nullptr) ? (p_val->p_south_->get_space() == space) : false};
            const bool v_w{(p_val->p_west_ != nullptr) ? (p_val->p_west_->get_space() == space) : false};
            const bool cb{(p_val->get_space() == space)};

            switch (card) {
                case CARDINAL::NORTH:
                    return v_n;
                case CARDINAL::NORTH_EAST:
                    return (v_n && v_e);
                case CARDINAL::EAST:
                    return v_e;
                case CARDINAL::SOUTH_EAST:
                    return (v_s && v_e);
                case CARDINAL::SOUTH:
                    return v_s;
                case CARDINAL::SOUTH_WEST:
                    return (v_s && v_w);
                case CARDINAL::WEST:
                    return v_w;
                case CARDINAL::NORTH_WEST:
                    return (v_n && v_w);
                case CARDINAL::NONE:
                    return cb;
                default:
                    break;
            }
        }

        return false;
    }

    void Cube::new_wall() {
        if ((this->p_north_ != nullptr) && (this->p_north_->get_space() == SPACE::SOLID)) {
            this->add_face(false, 0, 0);
            this->add_face(false, 2, 1);
        }

        if ((this->p_east_ != nullptr) && (this->p_east_->get_space() == SPACE::SOLID)) {
            this->add_face(false, 4, 0);
            this->add_face(false, 6, 1);
        }

        if ((this->p_south_ != nullptr) && (this->p_south_->get_space() == SPACE::SOLID)) {
            this->add_face(false, 8, 0);
            this->add_face(false, 10, 1);
        }

        if ((this->p_west_ != nullptr) && (this->p_west_->get_space() == SPACE::SOLID)) {
            this->add_face(false, 12, 0);
            this->add_face(false, 14, 1);
        }
    }

    void Cube::new_ramp(bool is_floor, CARDINAL card) {
        bool west_wall_down{false}, west_wall_up{false}, east_wall_down{false}, east_wall_up{false},    // West/East
            north_wall_down{false}, north_wall_up{false}, south_wall_down{false}, south_wall_up{false}; // Morth/Soult

        if (p_west_ != nullptr) {
            west_wall_down = p_west_->empty_space();
            west_wall_up = (p_west_->get_space() == SPACE::SOLID);
        }

        if (p_east_ != nullptr) {
            east_wall_down = p_east_->empty_space();
            east_wall_up = (p_east_->get_space() == SPACE::SOLID);
        }

        if (p_north_ != nullptr) {
            north_wall_down = p_north_->empty_space();
            north_wall_up = (p_north_->get_space() == SPACE::SOLID);
        }

        if (p_south_ != nullptr) {
            south_wall_down = p_south_->empty_space();
            south_wall_up = (p_south_->get_space() == SPACE::SOLID);
        }

        if (is_floor) {
            switch (card) {
                case CARDINAL::NORTH: { // OK
                    this->add_face(false, 28, 0);
                    this->add_face(false, 29, 1);
                    if (west_wall_down)
                        this->add_face(true, 12, 2);
                    if (west_wall_up)
                        this->add_face(false, 14, 1);
                    if (east_wall_down)
                        this->add_face(true, 7, 8);
                    if (east_wall_up)
                        this->add_face(false, 5, 5);
                } break;
                case CARDINAL::EAST: {
                    this->add_face(false, 30, 0);
                    this->add_face(false, 31, 1);

                    if (north_wall_down) // OK
                        this->add_face(true, 0, 2);
                    if (north_wall_up)
                        this->add_face(false, 2, 1);
                    if (south_wall_down)
                        this->add_face(true, 11, 8);
                    if (south_wall_up)
                        this->add_face(false, 9, 5);

                } break;
                case CARDINAL::SOUTH: { // OK
                    this->add_face(false, 32, 0);
                    this->add_face(false, 33, 1);
                    if (west_wall_down)
                        this->add_face(true, 15, 8);
                    if (west_wall_up)
                        this->add_face(false, 13, 5);
                    if (east_wall_down)
                        this->add_face(true, 4, 6);
                    if (east_wall_up)
                        this->add_face(false, 6, 1);
                } break;
                case CARDINAL::WEST: { // OK
                    this->add_face(false, 34, 0);
                    this->add_face(false, 35, 1);

                    if (north_wall_down)
                        this->add_face(true, 3, 8);
                    if (north_wall_up)
                        this->add_face(false, 1, 5);
                    if (south_wall_down)
                        this->add_face(true, 8, 2);
                    if (south_wall_up)
                        this->add_face(false, 10, 1);

                } break;
                default:
                    break;
            }
        } else {
            switch (card) { // OK
                case CARDINAL::NORTH: {
                    this->add_face(true, 32, 2);
                    this->add_face(true, 33, 3);

                    if (west_wall_down)
                        this->add_face(true, 13, 9);
                    if (west_wall_up)
                        this->add_face(false, 15, 4);
                    if (east_wall_down)
                        this->add_face(true, 6, 3);
                    if (east_wall_up)
                        this->add_face(false, 4, 0);

                } break;
                case CARDINAL::EAST:
                    this->add_face(true, 34, 2);
                    this->add_face(true, 35, 3);

                    if (north_wall_down)
                        this->add_face(true, 1, 9);
                    if (north_wall_up)
                        this->add_face(false, 3, 4);
                    if (south_wall_down)
                        this->add_face(true, 10, 3);
                    if (south_wall_up)
                        this->add_face(false, 8, 0);

                    break;
                case CARDINAL::SOUTH: {
                    this->add_face(true, 28, 2);
                    this->add_face(true, 29, 3);

                    if (west_wall_down)
                        this->add_face(true, 14, 3);
                    if (west_wall_up)
                        this->add_face(false, 12, 0);
                    if (east_wall_down)
                        this->add_face(true, 5, 9);
                    if (east_wall_up)
                        this->add_face(false, 7, 4);
                } break;
                case CARDINAL::WEST: {
                    this->add_face(true, 30, 2);
                    this->add_face(true, 31, 3);

                    if (north_wall_down)
                        this->add_face(true, 2, 3);
                    if (north_wall_up)
                        this->add_face(false, 0, 0);
                    if (south_wall_down)
                        this->add_face(true, 9, 9);
                    if (south_wall_up)
                        this->add_face(false, 11, 4);

                } break;
                default:
                    break;
            }
        }
    }

    void Cube::new_diag() {
        // get side
        const CARDINAL card{this->empty_quadrant_diag(DEEP::MIDDLE, false)};
        switch (card) {
            case CARDINAL::SOUTH_WEST:
                // ne (diag. sup. dir.)
                this->add_face(false, 24, 0);
                this->add_face(false, 25, 1);
                break;
            case CARDINAL::NORTH_WEST:
                // se (diag. inf. dir.)
                this->add_face(true, 26, 2);
                this->add_face(true, 27, 3);
                break;
            case CARDINAL::NORTH_EAST:
                // sw (diag. inf. esq.)
                this->add_face(true, 24, 2);
                this->add_face(true, 25, 3);
                break;
            case CARDINAL::SOUTH_EAST:
                // nw (diag. sup. esq.)
                this->add_face(false, 26, 0);
                this->add_face(false, 27, 1);
                break;
            default:
                break;
        }

        // floor of wall diag
        if (this->has_neighbor(DEEP::MIDDLE, card, SPACE::FLOOR) == true)
            new_flat_floor_ceeling(true, card);

        // ceeling of wall diag
        if (this->has_neighbor(DEEP::MIDDLE, card, SPACE::CEILING) == true)
            new_flat_floor_ceeling(false, card);

        if (this->has_neighbor(DEEP::MIDDLE, card, SPACE::FC) == true) {
            new_flat_floor_ceeling(true, card);
            new_flat_floor_ceeling(false, card);
        }
    }

    void Cube::new_floor() {
        CARDINAL card{CARDINAL::NONE};
        if ((this->p_down_ != nullptr) && (this->p_down_->get_space() == SPACE::DIAG))
            card = this->empty_quadrant_diag(DEEP::DOWN, true);

        this->new_flat_floor_ceeling(true, card);
    }

    void Cube::new_ceeling() {
        CARDINAL card{CARDINAL::NONE};
        if ((this->p_up_ != nullptr) && (this->p_up_->get_space() == SPACE::DIAG))
            card = this->empty_quadrant_diag(DEEP::UP, true);

        this->new_flat_floor_ceeling(false, card);
    }

    void Cube::new_flat_floor_ceeling(bool is_floor, CARDINAL card) {
        if (is_floor) {
            switch (card) {
                case CARDINAL::SOUTH_WEST:
                    this->add_face(false, 23, 4);
                    break;
                case CARDINAL::NORTH_WEST:
                    this->add_face(false, 22, 1);
                    break;
                case CARDINAL::NORTH_EAST:
                    this->add_face(false, 21, 5);
                    break;
                case CARDINAL::SOUTH_EAST:
                    this->add_face(false, 20, 0);
                    break;
                default:
                    this->add_face(false, 20, 0);
                    this->add_face(false, 22, 1);
                    break;
            }
        } else {
            switch (card) {
                case CARDINAL::SOUTH_WEST:
                    this->add_face(false, 16, 0);
                    break;
                case CARDINAL::NORTH_WEST:
                    this->add_face(false, 17, 5);
                    break;
                case CARDINAL::NORTH_EAST:
                    this->add_face(false, 18, 1);
                    break;
                case CARDINAL::SOUTH_EAST:
                    this->add_face(false, 19, 4);
                    break;
                default:
                    this->add_face(false, 16, 0);
                    this->add_face(false, 18, 1);
                    break;
            }
        }
    }

    void Cube::new_ramp_nsew(SPACE space) {
        if (space == SPACE::RAMP_FNS) {
            if ((p_north_ != nullptr) && (p_north_->empty_space())) {

                bool is_floor = (p_north_->get_space() == SPACE::FLOOR);
                this->new_ramp(is_floor, CARDINAL::SOUTH);

            } else if ((p_south_ != nullptr) && (p_south_->empty_space())) {

                bool is_floor = (p_south_->get_space() == SPACE::FLOOR);
                this->new_ramp(is_floor, CARDINAL::NORTH);
            }

        } else if (space == SPACE::RAMP_FEW) {
            if ((p_east_ != nullptr) && (p_east_->empty_space())) {

                bool is_floor = (p_east_->get_space() == SPACE::FLOOR);
                this->new_ramp(is_floor, CARDINAL::WEST);

            } else if ((p_west_ != nullptr) && (p_west_->empty_space())) {

                bool is_floor = (p_west_->get_space() == SPACE::FLOOR);
                this->new_ramp(is_floor, CARDINAL::EAST);
            }
        }
    }

    void Cube::create(Mesh* mesh) {
        this->mesh_ = mesh;

        const SPACE val{this->get_space()};
        switch (val) {
            case SPACE::EMPTY:
                this->new_wall();
                break;
            case SPACE::FLOOR: {
                this->new_wall();
                this->new_floor();
            } break;
            case SPACE::CEILING: {
                this->new_wall();
                this->new_ceeling();
            } break;
            case SPACE::FC: {
                this->new_wall();
                this->new_floor();
                this->new_ceeling();
            } break;
            case SPACE::DIAG: {
                this->new_diag();
            } break;
            case SPACE::RAMP_FNS:
            case SPACE::RAMP_FEW:
                this->new_ramp_nsew(val);
                break;
            default:
                break;
        }
    }
} // namespace ce
