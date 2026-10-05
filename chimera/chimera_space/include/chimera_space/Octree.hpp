#pragma once
#include "AABB.hpp"
#include "HeapQ.hpp"
#include <array>
#include <memory>
#include <queue>
#include <vector>

namespace ce {

    class Octree {
      public:
        explicit Octree(const glm::vec3& pos, const glm::vec3& size, Octree* parent) noexcept
            : p_parent_(parent), capacity_(parent->capacity_), leaf_mode_(parent->leaf_mode_), deep_(parent->deep_ + 1),
              serial_(serial_master++) {
            boundary_.set_position(pos, size);
        }

        explicit Octree(const AABB& boundary, const uint32_t& capacity, const bool& leaf_mode) noexcept
            : p_parent_(nullptr), capacity_(capacity), leaf_mode_(leaf_mode), deep_(0), serial_(serial_master++),
              boundary_(boundary) {}

        virtual ~Octree() noexcept { destroy(); }

        void destroy() noexcept {
            if (!childs_.empty()) {
                for (auto& octree : childs_) {
                    octree->destroy();
                    octree = nullptr;
                }
                childs_.clear();
            }

            points_.clear();
            indexes_.clear();
        }

        bool insert(const glm::vec3& point, const uint32_t& index) noexcept {

            if (boundary_.contains(point) == false)
                return false;

            if ((points_.size() < capacity_) && ((!leaf_mode_) || childs_.empty())) {
                points_.push_back(point);
                indexes_.push_back(index);
                return true;
            }

            if (childs_.empty())
                this->subdivide();

            if (leaf_mode_) {
                for (std::size_t i = 0; i < points_.size(); i++)
                    this->insert_new(points_[i], indexes_[i]);

                points_.clear();
                indexes_.clear();
            }

            return this->insert_new(point, index);
        }

        void insert_aabb(const AABB& aabb, const uint32_t& index) noexcept {

            const std::array<glm::vec3, 8>& v_list = aabb.get_all_vertex();
            for (const glm::vec3& p : v_list) {
                this->insert(p, index);
            }

            this->insert(aabb.get_position(), index);
        }

        void query(const AABB& aabb, std::vector<glm::vec3>& found) noexcept {

            if (boundary_.intersects(aabb) == false)
                return;

            for (auto p : points_) {
                if (aabb.contains(p) == true)
                    found.push_back(p);
            }

            for (auto& octree : childs_) {
                octree->query(aabb, found);
            }
        }

        bool has_point(const glm::vec3& point) noexcept {

            if (boundary_.contains(point) == true) {
                for (auto& octree : childs_) {
                    if (octree->has_point(point))
                        return true;
                }

                for (auto p : points_) {
                    if (isNearV3(p, point))
                        return true;
                }
            }
            return false;
        }

        void visible(const Frustum& frustum, std::queue<uint32_t>& qIndexes) noexcept {
            HeapQ<uint32_t> heap_q(false);
            this->visible(frustum, heap_q);

            uint32_t last = -1;
            while (heap_q.empty() == false) {
                uint32_t n = heap_q.top();
                if (n != last) {
                    qIndexes.push(n);
                    last = n;
                }
                heap_q.pop();
            }
        }

        void get_bondary_list(std::vector<AABB>& list, const bool& show_empty) noexcept {

            if (!childs_.empty()) {
                for (auto& octree : childs_) {
                    octree->get_bondary_list(list, show_empty);
                }
            } else {
                if ((points_.size() > 0) || (show_empty)) {
                    list.push_back(boundary_);
                }
            }
        }

      private:
        void subdivide() noexcept {

            const glm::vec3 s = boundary_.get_size() / 2.0f;
            const glm::vec3 h = s / 2.0f;
            const glm::vec3 max = boundary_.get_position() + h;
            const glm::vec3 min = boundary_.get_position() - h;

            childs_.push_back(std::make_unique<Octree>(glm::vec3(min.x, min.y, min.z), s, this)); // AabbBondery::BSW 0
            childs_.push_back(std::make_unique<Octree>(glm::vec3(max.x, min.y, min.z), s, this)); // AabbBondery::BSE 1
            childs_.push_back(std::make_unique<Octree>(glm::vec3(min.x, max.y, min.z), s, this)); // AabbBondery::TSW 2
            childs_.push_back(std::make_unique<Octree>(glm::vec3(max.x, max.y, min.z), s, this)); // AabbBondery::TSE 3
            childs_.push_back(std::make_unique<Octree>(glm::vec3(min.x, min.y, max.z), s, this)); // AabbBondery::BNW 4
            childs_.push_back(std::make_unique<Octree>(glm::vec3(max.x, min.y, max.z), s, this)); // AabbBondery::BNE 5
            childs_.push_back(std::make_unique<Octree>(glm::vec3(min.x, max.y, max.z), s, this)); // AabbBondery::TNW 6
            childs_.push_back(std::make_unique<Octree>(glm::vec3(max.x, max.y, max.z), s, this)); // AabbBondery::TNE 7
        }

        void visible(const Frustum& frustum, HeapQ<uint32_t>& q_indexes) noexcept {

            if (boundary_.visible(frustum)) {
                for (auto& octree : childs_) {
                    octree->visible(frustum, q_indexes);
                }

                uint32_t last = -1;
                for (auto& i : this->indexes_) {
                    if (i != last) {
                        q_indexes.push(i);
                        last = i;
                    }
                }
            }
        }

        bool insert_new(const glm::vec3& point, const uint32_t& index) noexcept {
            for (auto& octree : childs_) {
                if (octree->insert(point, index))
                    return true;
            }
            return false;
        }

      private:
        [[maybe_unused]]
        Octree* p_parent_{nullptr};
        uint32_t capacity_{27};
        bool leaf_mode_{true};
        uint32_t deep_{0};
        uint32_t serial_{0};
        AABB boundary_;
        std::vector<std::unique_ptr<Octree>> childs_;
        std::vector<glm::vec3> points_;
        std::vector<uint32_t> indexes_;
        inline static uint32_t serial_master{0};
    };
} // namespace ce
