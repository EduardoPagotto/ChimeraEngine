#pragma once

#include "chimera/core/Mutex.hpp"

namespace Chimera {

/// <summary> Classe Template de Singletons </summary>
template <class C> class Singleton {
  public:
    /// <summary> Retorna ponteiro do Template do singleton </summary>
    /// <returns> Ponteiro do objeto criado ou refenciado sem controle de
    /// refencia</returns>
    static C* getSingleton() {
        if (!m_CurrentInstance) {
            m_travaSingleton.lock();
            if (!m_CurrentInstance) {
                m_CurrentInstance = new C;
            }
            m_travaSingleton.unlock();
        }
        return m_CurrentInstance;
    }

    /// <summary> Retorna ponteiro do Template do singleton con contedor de referencia
    /// para remocao</summary> <returns> Ponteiro do objeto criado ou refenciado com
    /// contador de referencia</returns>
    static C* getRefSingleton() {
        m_travaSingleton.lock();
        m_refCount++;
        m_travaSingleton.unlock();
        if (!m_CurrentInstance) {
            m_travaSingleton.lock();
            if (!m_CurrentInstance) {
                m_CurrentInstance = new C;
            }
            m_travaSingleton.unlock();
        }
        return m_CurrentInstance;
    }

    /// <summary> Remove Referencia ao objeto ate zerar quando o mesmo e DELETADO
    /// </summary>
    static void releaseRefSingleton() {
        m_travaSingleton.lock();
        m_refCount--;
        if (m_refCount == 0) {
            delete m_CurrentInstance;
            m_CurrentInstance = nullptr;
        }
        m_travaSingleton.unlock();
    }

    /// <summary> Retorna o contador de referencia </summary>
    /// <returns> Numero de referencias ao objeto </returns>
    static long getRefCount() {
        m_travaSingleton.lock();
        long retorno = m_refCount;
        m_travaSingleton.unlock();
        return retorno;
    }

    /// <summary> Deletea o objeto referenciado nao importando quantos ainda estao ativos
    /// </summary>
    static void destroy() {
        m_travaSingleton.lock();
        m_refCount = 0;
        delete m_CurrentInstance;
        m_CurrentInstance = nullptr;
        m_travaSingleton.unlock();
    }

  protected:
    /// <summary> Bloqueio ao Construtor </summary>
    Singleton();

    /// <summary> Bloqueio ao Construtor de Copia </summary>
    Singleton(Singleton const&);

  private:
    ///< summary> Contador de referncia </summary>
    static long m_refCount;

    ///< summary> Ponteiro do objeto instanciado </summary>
    static C* m_CurrentInstance;

    ///< summary> trava de acesso ao singleton </summary>
    static Mutex m_travaSingleton;
};

template <class C> C* Singleton<C>::m_CurrentInstance = nullptr;
template <class C> long Singleton<C>::m_refCount = 0;
template <class C> Mutex Singleton<C>::m_travaSingleton;
} // namespace Chimera
