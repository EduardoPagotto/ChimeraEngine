#include "chimera/core/HeapQ.hpp"
#include <iostream>
#include <stdexcept>

template <class T>
HeapQ<T>::HeapQ(bool max) : max(max) {}

template <class T>
HeapQ<T>::~HeapQ() {
    heap.clear();
}

template <class T>
void HeapQ<T>::swap(const int& i0, const int& i1) {
    int temp = heap[i0];
    heap[i0] = heap[i1];
    heap[i1] = temp;
}

// Recursive heapify-down algorithm
template <class T>
void HeapQ<T>::heapify_down(const int& indice) {
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

// Recursive heapify-up algorithm
template <class T>
void HeapQ<T>::heapify_up(const int& indice) {
    const T& t1 = heap[this->parentIndex(indice)];
    const T& t2 = heap[indice];

    const bool doSwap = this->max ? (t1 < t2) : (t1 > t2);
    if (indice && doSwap) {
        swap(indice, this->parentIndex(indice));
        heapify_up(this->parentIndex(indice));
    }
}

// insert key into the heap
template <class T>
void HeapQ<T>::push(const T& key) {
    heap.push_back(key);
    heapify_up(heap.size() - 1);
}

// Function to remove an element with the highest priority (present at the root)
template <class T>
void HeapQ<T>::pop() {
    if (heap.size() == 0)
        throw std::out_of_range("Vector<X>::back() :index is out of range(Heap underflow)");
    heap[0] = heap.back();
    heap.pop_back();

    // call heapify-down on the root node
    heapify_down(0);
}

// Function to return an element with the highest priority (present at the root)
template <class T>
T HeapQ<T>::top() {
    if (heap.size() == 0)
        throw std::out_of_range("Vector<X>::at() index is out of range(Heap underflow)");

    return heap.at(0);
}

template <class T>
void HeapQ<T>::reBuild() {
    // TODO: Testar
    int halfSize = heap.size() / 2;
    for (int index = halfSize; index > 0; index--)
        heapify_down(index);
}

template <class T>
void HeapQ<T>::getRaw(std::vector<T>& v) {
    for (int index = 0; index < heap.size(); index++)
        v.push_back(heap[index]);
}

template <class T>
int HeapQ<T>::height() {

    int altura = -1;
    int indice = 0;
    while (indice < heap.size()) {
        indice = this->leftChildIndex(indice);
        altura++;
    }
    return altura;
}

template <class T>
void HeapQ<T>::preOrdem(const int& indice, std::vector<T>& v) {
    if (indice < heap.size()) {
        v.push_back(heap[indice]);
        this->preOrdem(this->leftChildIndex(indice), v);
        this->preOrdem(this->rightChildIndex(indice), v);
    }
}