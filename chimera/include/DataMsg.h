#ifndef DATA_MSG_H_
#define DATA_MSG_H_

namespace Chimera {

/**
 * Enum KindOperation
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class KindOperation {
    NONE,
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
    DataMsg ( KindOperation _kindOperation, void* _ptrSource, void* _param, void* _result ) : kindOperation ( _kindOperation ) , ptrSource ( _ptrSource ), param ( _param ), result ( _result ), done ( false ) { }
    virtual ~DataMsg() {}

    inline KindOperation getKindOperation() const {
        return kindOperation;
    }
    inline void setKindOperation ( KindOperation _kindOperation ) {
        this->kindOperation = _kindOperation;
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
    KindOperation kindOperation;
    void* ptrSource;
    void* param;
    void* result;
    bool done;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
