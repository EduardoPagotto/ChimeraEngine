#pragma once
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

template <class T>
class HeapQ {
  private:
    std::vector<T> m_heap;
    bool m_max;

  public:
    HeapQ(bool max = true) : m_max(max) {}

    virtual ~HeapQ() { m_heap.clear(); }

    inline const uint32_t size() const { return m_heap.size(); }

    inline const bool empty() const { return m_heap.size() == 0; }

    inline void getRaw(std::vector<T>& v) const { v.assign(m_heap.begin(), m_heap.end()); }

    /**
     * @brief insert key into the heap
     *
     * @param key
     */
    void push(const T& key) {
        m_heap.push_back(key);
        heapify_up(m_heap.size() - 1);
    }

    /**
     * @brief Function to remove an element with the highest priority (present at the root)
     *
     */
    void pop() {

        if (m_heap.size() == 0)
            throw std::out_of_range("Vector<X>::back() :index is out of range(Heap underflow)");

        m_heap[0] = m_heap.back();
        m_heap.pop_back();

        // call heapify-down on the root node
        heapify_down(0);
    }

    /**
     * @brief Function to return an element with the highest priority (present at the root)
     *
     * @return T
     */
    T top() {

        if (m_heap.size() == 0)
            throw std::out_of_range("Vector<X>::at() index is out of range(Heap underflow)");

        return m_heap.at(0);
    }

    /**
     * @brief extra
     *
     */
    void reBuild() {
        // TODO: Testar
        const uint32_t halfSize = m_heap.size() / 2;
        for (uint32_t index = halfSize; index > 0; index--)
            heapify_down(index);
    }

    int height() {
        int altura = -1;
        int indice = 0;
        while (indice < m_heap.size()) {
            indice = this->leftChildIndex(indice);
            altura++;
        }
        return altura;
    }

    void preOrdem(const int& indice, std::vector<T>& v) {
        if (indice < m_heap.size()) {
            v.push_back(m_heap[indice]);
            this->preOrdem(this->leftChildIndex(indice), v);
            this->preOrdem(this->rightChildIndex(indice), v);
        }
    }

  private:
    inline const int parentIndex(const int& i) const { return (i - 1) / 2; }
    inline const int leftChildIndex(const int& i) const { return (2 * i + 1); }
    inline const int rightChildIndex(const int& i) const { return (2 * i + 2); }

    void swap(const uint32_t& i0, const uint32_t& i1) {
        const T temp = m_heap[i0];
        m_heap[i0] = m_heap[i1];
        m_heap[i1] = temp;
    }

    /**
     * @brief Recursive heapify-down algorithm
     *
     * @param indice
     */
    void heapify_down(const int& indice) {

        const int leftIndex = this->leftChildIndex(indice);
        const int rightIndex = this->rightChildIndex(indice);

        int newIndex = indice;

        if (m_max) {

            if (leftIndex < m_heap.size() && m_heap[leftIndex] > m_heap[indice])
                newIndex = leftIndex;

            if (rightIndex < m_heap.size() && m_heap[rightIndex] > m_heap[newIndex])
                newIndex = rightIndex;

        } else {

            if (leftIndex < m_heap.size() && m_heap[leftIndex] < m_heap[indice])
                newIndex = leftIndex;

            if (rightIndex < m_heap.size() && m_heap[rightIndex] < m_heap[newIndex])
                newIndex = rightIndex;
        }

        if (newIndex != indice) {
            swap(indice, newIndex);
            heapify_down(newIndex);
        }
    }

    /**
     * @brief Recursive heapify-up algorithm
     *
     * @param indice
     */
    void heapify_up(const int& indice) {

        const T& t1 = m_heap[this->parentIndex(indice)];
        const T& t2 = m_heap[indice];

        const bool doSwap = m_max ? (t1 < t2) : (t1 > t2);
        if (indice && doSwap) {
            swap(indice, this->parentIndex(indice));
            heapify_up(this->parentIndex(indice));
        }
    }
};
