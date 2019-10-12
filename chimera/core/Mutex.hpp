#ifndef __CHIMERA_MUTEX__HPP
#define __CHIMERA_MUTEX__HPP

#include <cstdarg>
#include <iterator>
#include <pthread.h>
#include <sys/errno.h>
#include <sys/time.h>

//#include <ctime>
//#include <queue>
//#include <map>

namespace Chimera {

/// <summary> Wrapper Windows-Posix para status de mutex </summary>
enum MUTEX_STATUS {
#ifdef _WIN32
    MUTEX_OK = TRUE,       // WAIT_OBJECT_0,
    MUTEX_TIME_OUT = FALSE // WAIT_TIMEOUT
#else
    MUTEX_OK = 0,
    MUTEX_TIME_OUT = EBUSY
#endif
};

#ifdef _WIN32

/// <summary> Mutex em tempo de kernel(apenas win32) </summary>
class Mutex {
  public:
    /// <summary> Contrutor padaro mudex livre </summary>
    Mutex(void) { m_mutex = CreateMutex(nullptr, FALSE, nullptr); }

    /// <summary> Destrutor padrao release de Handle </summary>
    virtual ~Mutex(void) {
        WaitForSingleObject(m_mutex, INFINITE);
        CloseHandle(m_mutex);
    }

    /// <summary> Trava Mutex </summary>
    inline DWORD lock(void) { return WaitForSingleObject(m_mutex, INFINITE); }

    /// <summary> Tenta Travar Mutex </summary>
    /// <returns>
    /// MUTEX_OK (TRUE) se obteve a trava
    /// MUTEX_TIME_OUT (FALSE) se falhou na obtcao de trava
    /// </returns>
    inline DWORD tryLock(DWORD _milisec = 0) { return WaitForSingleObject(m_mutex, _milisec); }

    /// <summary> Destrava Mutex </summary>
    inline DWORD unlock(void) {
        if (ReleaseMutex(m_mutex) != 0) {
            return 0;
        }
        return LONG_MAX;
    }

  private:
    /// <summary> Handle de controle do Mutex </summary>
    HANDLE m_mutex;
};

#else
class Mutex {
  public:
    Mutex(void) {
        pthread_mutexattr_t mattr;
        pthread_mutexattr_init(&mattr);
        pthread_mutex_init(&m_mutex, &mattr);
    }
    virtual ~Mutex(void) {
        pthread_mutex_lock(&m_mutex);
        pthread_mutex_unlock(&m_mutex);
        pthread_mutex_destroy(&m_mutex);
    }
    inline int lock(void) { return pthread_mutex_lock(&this->m_mutex); }

    inline int tryLock(void) { return pthread_mutex_trylock(&this->m_mutex); }

    inline int unlock(void) { return pthread_mutex_unlock(&this->m_mutex); }

  protected:
    pthread_mutex_t m_mutex;
};
#endif

/// <summary> Trava de Mutex Atraves de opercao em Stack-Pointer </summary>
class Lock {
  public:
    /// <summary> Contrutor Trava Mutex </summary>
    /// <param name=" _pMutex"> Mutex a ser travado no Stack-Pointer </param>
    Lock(Mutex* _pMutex) {
        m_pMutex = _pMutex;
        m_pMutex->lock();
    }

    /// <summary> Destrutor Destrava Mutex </summary>
    ~Lock() { m_pMutex->unlock(); }

  private:
    /// <summary> Ponteiro do Mutex </summary>
    Mutex* m_pMutex;
};

} // namespace Chimera
#endif
