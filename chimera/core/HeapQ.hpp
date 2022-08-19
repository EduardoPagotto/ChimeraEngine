#pragma once
#include <cstdint>
#include <vector>

template <class T>
class HeapQ {
  public:
    HeapQ(bool max = true);
    virtual ~HeapQ();
    inline const unsigned int size() const { return heap.size(); }
    inline const bool empty() const { return heap.size() == 0; }

    void push(const T& key);
    void pop();
    T top();

    // extra
    void reBuild();
    void getRaw(std::vector<T>& v);
    int height();
    void preOrdem(const int& indice, std::vector<T>& v);

  private:
    inline const int parentIndex(const int& i) const { return (i - 1) / 2; }
    inline const int leftChildIndex(const int& i) const { return (2 * i + 1); }
    inline const int rightChildIndex(const int& i) const { return (2 * i + 2); }
    void swap(const int& i0, const int& i1);
    void heapify_down(const int& indice);
    void heapify_up(const int& indice);

    std::vector<T> heap;
    bool max;
};
