#ifndef DATA_MSG_H_
#define DATA_MSG_H_

namespace Chimera {

/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class KindOp {
    START,
    STOP,
    EVENT,
    DRAW3D,
    SDL
};

/**
 * Class DataMsg
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class DataMsg {
public:
    /**
     * Contrutor da Classe de mesnagem de dados
     * @param _kindOp tipo de operacao
     * @param _ptrSource ponteiro de origem da chamada
     * @param _param ponteiro com o parametro
     * @param _result ponteiro com o resultado
     */
    DataMsg ( KindOp _kindOp, void* _ptrSource, void* _param, void* _result ) : kindOp ( _kindOp ) , ptrSource ( _ptrSource ), param ( _param ), result ( _result ), done ( false ) { }
    
    /**
     * Destrutor
     */
    virtual ~DataMsg() {}

    /**
     * Retorna tipo de operacao
     * @return O tipo de operaao que esta sendo realizado 
     */
    inline KindOp getKindOp() const {
        return kindOp;
    }
    
    /**
     * Define o tipo de operacao
     * @param _kindOp tipo de operacao
     */
    inline void setKindOp ( KindOp _kindOp ) {
        this->kindOp = _kindOp;
    }
    
    inline void* getParam() const {
        return param;
    }
    inline void setParam ( void* _param ) {
        this->param = _param;
    }
    inline void* getPtrSource() const {
        return ptrSource;
    }
    inline void setPtrSource ( void* _ptrSource ) {
        this->ptrSource = _ptrSource;
    }
    inline void* getResult() const {
        return result;
    }
    inline void setResult ( void* _result ) {
        this->result = _result;
    }

    inline bool isDone() const {
        return done;
    }

    inline void setDone ( bool _done ) {
        done = _done;
    }

private:
    KindOp kindOp;
    void* ptrSource;
    void* param;
    void* result;
    bool done;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
