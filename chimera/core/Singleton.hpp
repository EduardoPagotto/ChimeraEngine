#pragma once
#include <mutex>

namespace Chimera {

template <class T>
class Singleton {
  public:
    static T* get() {
        count++;
        if (!instance) {
            std::lock_guard<std::mutex> lck(critical);
            if (!instance)
                instance = new T;
        }
        return instance;
    }

    static void release() {
        std::lock_guard<std::mutex> lck(critical);
        if (count > 0) {
            count--;
            if (count == 0) {
                delete instance;
                instance = nullptr;
            }
        }
    }

    static size_t getCount() {
        std::lock_guard<std::mutex> lck(critical);
        return count;
    }

  protected:
    Singleton<T>() = delete;
    ~Singleton<T>() = delete;
    Singleton<T>(Singleton<T> const&) = delete;

  private:
    inline static size_t count{0};
    inline static T* instance{nullptr};
    inline static std::mutex critical;
};

// int* t = Singleton<int>::get();
// *t = 1000;
// Singleton<int>::release();

} // namespace Chimera
