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

        void reBuild() noexcept {
            // TODO: Testar
            int halfSize = heap_.size() / 2;
            for (int index = halfSize; index > 0; index--)
                heapify_down(index);
        }

        void getRaw(std::vector<T>& v) noexcept {
            for (int index = 0; index < heap_.size(); index++)
                v.push_back(heap_[index]);
        }

        int height() noexcept {
            int altura = -1;
            int indice = 0;
            while (indice < heap_.size()) {
                indice = this->leftChildIndex(indice);
                altura++;
            }
            return altura;
        }

        void preOrdem(const int& indice, std::vector<T>& v) noexcept {
            if (indice < heap_.size()) {
                v.push_back(heap_[indice]);
                this->preOrdem(this->leftChildIndex(indice), v);
                this->preOrdem(this->rightChildIndex(indice), v);
            }
        }

      private:
        inline const int parentIndex(const int& i) const noexcept { return (i - 1) / 2; }

        inline const int leftChildIndex(const int& i) const noexcept { return (2 * i + 1); }

        inline const int rightChildIndex(const int& i) const noexcept { return (2 * i + 2); }

        void swap(const int& i0, const int& i1) noexcept {
            int temp = heap_[i0];
            heap_[i0] = heap_[i1];
            heap_[i1] = temp;
        }

        /// @brief Recursive heapify-down algorithm
        /// @param indice
        void heapify_down(const int& indice) noexcept {

            const int leftIndex = this->leftChildIndex(indice);
            const int rightIndex = this->rightChildIndex(indice);
            int newIndex = indice;

            if (this->max_) {
                if (leftIndex < heap_.size() && heap_[leftIndex] > heap_[indice])
                    newIndex = leftIndex;
                if (rightIndex < heap_.size() && heap_[rightIndex] > heap_[newIndex])
                    newIndex = rightIndex;
            } else {
                if (leftIndex < heap_.size() && heap_[leftIndex] < heap_[indice])
                    newIndex = leftIndex;
                if (rightIndex < heap_.size() && heap_[rightIndex] < heap_[newIndex])
                    newIndex = rightIndex;
            }

            if (newIndex != indice) {
                swap(indice, newIndex);
                heapify_down(newIndex);
            }
        }

        /// @brief Recursive heapify-up algorithm
        /// @param indice
        void heapify_up(const int& indice) noexcept {
            const T& t1 = heap_[this->parentIndex(indice)];
            const T& t2 = heap_[indice];

            const bool doSwap = this->max_ ? (t1 < t2) : (t1 > t2);
            if (indice && doSwap) {
                swap(indice, this->parentIndex(indice));
                heapify_up(this->parentIndex(indice));
            }
        }

      private:
        std::vector<T> heap_;
        bool max_{true};
    };
} // namespace ce
