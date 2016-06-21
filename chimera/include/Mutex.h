#ifndef MUTEX_H
#define MUTEX_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <cstdarg>
#include <pthread.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <iterator>
#endif

//#include <ctime>
//#include <queue>
//#include <map>

namespace Chimera
{

/// <summary> Wrapper Windows-Posix para status de mutex </summary>
enum MUTEX_STATUS {
#ifdef _WIN32
    MUTEX_OK = TRUE,	//WAIT_OBJECT_0,
    MUTEX_TIME_OUT = FALSE	//WAIT_TIMEOUT
#else
    MUTEX_OK = 0,
    MUTEX_TIME_OUT = EBUSY
#endif
};

#ifdef _WIN32

/// <summary> Mutex em tempo de kernel(apenas win32) </summary>
class Mutex
{
public:

    /// <summary> Contrutor padaro mudex livre </summary>
    Mutex ( void ) {
        m_mutex = CreateMutex ( nullptr, FALSE, nullptr );
    }

    /// <summary> Destrutor padrao release de Handle </summary>
    virtual ~Mutex ( void ) {
        WaitForSingleObject ( m_mutex, INFINITE );
        CloseHandle ( m_mutex );
    }

    /// <summary> Trava Mutex </summary>
    inline DWORD lock ( void ) {
        return WaitForSingleObject ( m_mutex, INFINITE );
    }

    /// <summary> Tenta Travar Mutex </summary>
    /// <returns>
    /// MUTEX_OK (TRUE) se obteve a trava
    /// MUTEX_TIME_OUT (FALSE) se falhou na obtcao de trava
    /// </returns>
    inline DWORD tryLock ( DWORD _milisec = 0 ) {
        return WaitForSingleObject ( m_mutex, _milisec );
    }

    /// <summary> Destrava Mutex </summary>
    inline DWORD unlock ( void ) {
        if ( ReleaseMutex ( m_mutex ) != 0 ) {
            return 0;
        }
        return LONG_MAX;
    }
private:
    /// <summary> Handle de controle do Mutex </summary>
    HANDLE m_mutex;
};

#else
class Mutex
{
public:
    Mutex ( void ) {
        pthread_mutexattr_t mattr;
        pthread_mutexattr_init ( &mattr );
        pthread_mutex_init ( &m_mutex,&mattr );
    }
    virtual ~Mutex ( void ) {
        pthread_mutex_lock ( &m_mutex );
        pthread_mutex_unlock ( &m_mutex );
        pthread_mutex_destroy ( &m_mutex );
    }
    inline int lock ( void ) {
        return pthread_mutex_lock ( &this->m_mutex );
    }

    inline int tryLock ( void ) {
        return pthread_mutex_trylock ( &this->m_mutex );
    }

    inline int unlock ( void )	{
        return pthread_mutex_unlock ( &this->m_mutex );
    }
protected:
    pthread_mutex_t m_mutex;
};
#endif

/// <summary> Trava de Mutex Atraves de opercao em Stack-Pointer </summary>
class Lock
{
public:
    /// <summary> Contrutor Trava Mutex </summary>
    /// <param name=" _pMutex"> Mutex a ser travado no Stack-Pointer </param>
    Lock ( Mutex *_pMutex ) {
        m_pMutex = _pMutex;
        m_pMutex->lock();
    }

    /// <summary> Destrutor Destrava Mutex </summary>
    ~Lock() {
        m_pMutex->unlock();
    }

private:
    /// <summary> Ponteiro do Mutex </summary>
    Mutex *m_pMutex;
};

// #ifdef _WIN32
// /// <summary> classe conditional </summary>
// class Conditional {
// public:
//     /// <summary> Construtor Padrao Condiciao </summary>
//     /// <param name="_manualReset">
//     /// Default (FALSE) Condicao criada Liberada
//     /// True Condicao criada Bloqueada
//     /// </param>
//     Conditional(BOOL _manualReset = FALSE) {
//         m_cond = CreateEvent(nullptr,_manualReset ,FALSE,nullptr);
//     }
//
//     /// <summary> Destrutor Padrao </summary>
//     virtual ~Conditional() {
//         CloseHandle(m_cond);
//     }
//
//     /// <summary> Sinaliza Liberacao da thread em espera </summary>
//     /// <returns> Sinalizacao OK </returns>
//     inline BOOL signal() {
//         return SetEvent(m_cond);
//     }
//
//     /// <summary> Sinaliza Liberacao da thread em espera </summary>
//     /// <returns> Liberacao OK </returns>
//     inline BOOL unsignal() {
//         return ResetEvent(m_cond);
//     }
//
//     /// <summary> Sinaliza Liberacao de Todas as threads em espera </summary>
//     /// TODO esta errado para win32
//     inline BOOL broadcast() {
//         return SetEvent(m_cond);
//     }
//
//     /// <summary>
//     /// Espera Sinalizacao para continuar
//     /// </summary>
//     /// <param name="_milisec">
//     /// Tempo de espera em milisegundos
//     /// vazio infinito
//     /// 0 intantaneo
//     /// </param>
//     /// <returns>
//     /// Retorno 0 em caso de liberacao por sinal
//     /// Retorno ETIME em caso de estouro de tempo
//     /// </returns>
//     inline DWORD wait(DWORD _milisec = INFINITE) {
//         return WaitForSingleObject(m_cond, _milisec);
//     }
//
// private:
//     /// <summary> Handle de controle </summary>
//     HANDLE m_cond;
// };
//
// #else
// class Conditional
// {
// public:
//     /// <summary>Construtor Padrao</summary>
//     Conditional() {
//         pthread_cond_init(&m_cond,nullptr);
//     }
//
//     /// <summary>Destrutor Padrao</summary>
//     virtual ~Conditional() {
//         pthread_cond_destroy(&m_cond);
//     }
//
//     /// <summary>Sinaliza Liberacao da thread em espera</summary>
//     inline void signal() {
//         pthread_cond_signal(&m_cond);
//     }
//
//     /// <summary>Sinaliza Liberacao de Todas as threads em espera</summary>
//     inline void broadcast() {
//         pthread_cond_broadcast(&m_cond);
//     }
//
//     /// <summary>Espera Sinalizacao para continuar</summary>
//     /// <param name="_milisec">
//     /// Tempo de espera em milisegundos
//     /// vazio infinito
//     /// 0 intantaneo
//     /// </param>
//     /// <returns>Retorno 0 em caso de liberacao por sinal ou ETIME em caso de estouro de tempo</returns>
//     inline DWORD wait(DWORD _milisec = INFINITE) {
//         //TODO:TESTAR unix
//         if (_milisec == INFINITE)
//         {
//             pthread_cond_wait(&m_cond,&m_mutex);
//             return true;
//         }
//         struct timeval tv;
//         struct timespec ts;
//         gettimeofday(&tv,nullptr);
//         ts.tv_sec = tv.tv_sec + _milisec;
//         ts.tv_nsec = tv.tv_usec * 1000;
//         return pthread_cond_timedwait(&m_cond,&m_mutex,&ts);
//     }
//
// private:
//     pthread_cond_t m_cond;
//     pthread_mutex_t m_mutex;
// };
// #endif

}
#endif
