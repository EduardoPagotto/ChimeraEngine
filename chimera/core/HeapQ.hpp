#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>

template <class T>
class HeapQ {
  private:
    std::vector<T> heap;
    bool max{true};

  public:
    explicit HeapQ(bool max = true) noexcept : max(max) {}

    virtual ~HeapQ() noexcept { heap.clear(); }

    inline const uint32_t size() const noexcept { return heap.size(); }

    inline const bool empty() const noexcept { return heap.size() == 0; }

    /// @brief Insert key into the heap
    /// @param key
    void push(const T& key) noexcept {
        heap.push_back(key);
        heapify_up(heap.size() - 1);
    }

    /// @brief Function to remove an element with the highest priority (present at the root)
    void pop() {
        if (heap.size() == 0)
            throw std::out_of_range("Vector<X>::back() :index is out of range(Heap underflow)");
        heap[0] = heap.back();
        heap.pop_back();

        // call heapify-down on the root node
        heapify_down(0);
    }

    /// @brief Function to return an element with the highest priority (present at the root)
    /// @return
    T top() {
        if (heap.size() == 0)
            throw std::out_of_range("Vector<X>::at() index is out of range(Heap underflow)");

        return heap.at(0);
    }

    void reBuild() noexcept {
        // TODO: Testar
        int halfSize = heap.size() / 2;
        for (int index = halfSize; index > 0; index--)
            heapify_down(index);
    }

    void getRaw(std::vector<T>& v) noexcept {
        for (int index = 0; index < heap.size(); index++)
            v.push_back(heap[index]);
    }

    int height() noexcept {
        int altura = -1;
        int indice = 0;
        while (indice < heap.size()) {
            indice = this->leftChildIndex(indice);
            altura++;
        }
        return altura;
    }

    void preOrdem(const int& indice, std::vector<T>& v) noexcept {
        if (indice < heap.size()) {
            v.push_back(heap[indice]);
            this->preOrdem(this->leftChildIndex(indice), v);
            this->preOrdem(this->rightChildIndex(indice), v);
        }
    }

  private:
    inline const int parentIndex(const int& i) const noexcept { return (i - 1) / 2; }

    inline const int leftChildIndex(const int& i) const noexcept { return (2 * i + 1); }

    inline const int rightChildIndex(const int& i) const noexcept { return (2 * i + 2); }

    void swap(const int& i0, const int& i1) noexcept {
        int temp = heap[i0];
        heap[i0] = heap[i1];
        heap[i1] = temp;
    }

    /// @brief Recursive heapify-down algorithm
    /// @param indice
    void heapify_down(const int& indice) noexcept {

        const int leftIndex = this->leftChildIndex(indice);
        const int rightIndex = this->rightChildIndex(indice);
        int newIndex = indice;

        if (this->max) {
            if (leftIndex < heap.size() && heap[leftIndex] > heap[indice])
                newIndex = leftIndex;
            if (rightIndex < heap.size() && heap[rightIndex] > heap[newIndex])
                newIndex = rightIndex;
        } else {
            if (leftIndex < heap.size() && heap[leftIndex] < heap[indice])
                newIndex = leftIndex;
            if (rightIndex < heap.size() && heap[rightIndex] < heap[newIndex])
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
        const T& t1 = heap[this->parentIndex(indice)];
        const T& t2 = heap[indice];

        const bool doSwap = this->max ? (t1 < t2) : (t1 > t2);
        if (indice && doSwap) {
            swap(indice, this->parentIndex(indice));
            heapify_up(this->parentIndex(indice));
        }
    }
};
