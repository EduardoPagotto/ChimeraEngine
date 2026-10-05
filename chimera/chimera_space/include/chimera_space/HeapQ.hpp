#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace ce {

    template <class T>
    class HeapQ {
      public:
        explicit HeapQ(bool max = true) noexcept : max_(max) {}

        virtual ~HeapQ() noexcept { heap_.clear(); }

        inline const uint32_t size() const noexcept { return heap_.size(); }

        inline const bool empty() const noexcept { return heap_.size() == 0; }

        /// @brief Insert key into the heap
        /// @param key
        void push(const T& key) noexcept {
            heap_.push_back(key);
            heapify_up(heap_.size() - 1);
        }

        /// @brief Function to remove an element with the highest priority (present at the root)
        void pop() {
            if (heap_.size() == 0)
                throw std::out_of_range("Vector<X>::back() :index is out of range(Heap underflow)");
            heap_[0] = heap_.back();
            heap_.pop_back();

            // call heapify-down on the root node
            heapify_down(0);
        }

        /// @brief Function to return an element with the highest priority (present at the root)
        /// @return
        T top() {
            if (heap_.size() == 0)
                throw std::out_of_range("Vector<X>::at() index is out of range(Heap underflow)");

            return heap_.at(0);
        }

        void re_build() noexcept {
            // TODO: Testar
            int half_size = heap_.size() / 2;
            for (int index = half_size; index > 0; index--)
                heapify_down(index);
        }

        void get_raw(std::vector<T>& v) noexcept {
            for (int index = 0; index < heap_.size(); index++)
                v.push_back(heap_[index]);
        }

        int height() noexcept {
            int altura = -1;
            int indice = 0;
            while (indice < heap_.size()) {
                indice = this->left_child_index(indice);
                altura++;
            }
            return altura;
        }

        void pre_ordem(const int& indice, std::vector<T>& v) noexcept {
            if (indice < heap_.size()) {
                v.push_back(heap_[indice]);
                this->pre_ordem(this->left_child_index(indice), v);
                this->pre_ordem(this->right_child_index(indice), v);
            }
        }

      private:
        inline const int parent_index(const int& i) const noexcept { return (i - 1) / 2; }

        inline const int left_child_index(const int& i) const noexcept { return (2 * i + 1); }

        inline const int right_child_index(const int& i) const noexcept { return (2 * i + 2); }

        void swap(const int& i0, const int& i1) noexcept {
            int temp = heap_[i0];
            heap_[i0] = heap_[i1];
            heap_[i1] = temp;
        }

        /// @brief Recursive heapify-down algorithm
        /// @param indice
        void heapify_down(const int& indice) noexcept {

            const int left_index = this->left_child_index(indice);
            const int right_index = this->right_child_index(indice);
            int new_index = indice;

            if (this->max_) {
                if (left_index < heap_.size() && heap_[left_index] > heap_[indice])
                    new_index = left_index;
                if (right_index < heap_.size() && heap_[right_index] > heap_[new_index])
                    new_index = right_index;
            } else {
                if (left_index < heap_.size() && heap_[left_index] < heap_[indice])
                    new_index = left_index;
                if (right_index < heap_.size() && heap_[right_index] < heap_[new_index])
                    new_index = right_index;
            }

            if (new_index != indice) {
                swap(indice, new_index);
                heapify_down(new_index);
            }
        }

        /// @brief Recursive heapify-up algorithm
        /// @param indice
        void heapify_up(const int& indice) noexcept {
            const T& t1 = heap_[this->parent_index(indice)];
            const T& t2 = heap_[indice];

            const bool do_swap = this->max_ ? (t1 < t2) : (t1 > t2);
            if (indice && do_swap) {
                swap(indice, this->parent_index(indice));
                heapify_up(this->parent_index(indice));
            }
        }

      private:
        std::vector<T> heap_;
        bool max_{true};
    };
} // namespace ce
