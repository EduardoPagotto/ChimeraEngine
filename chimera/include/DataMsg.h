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
    DataMsg ( KindOperation kindOperation, void* ptrSource, void* param, void* result ) : kindOperation ( kindOperation ) , ptrSource ( ptrSource ), param ( param ), result ( result ), done ( false ) { }
    virtual ~DataMsg() {}

    inline KindOperation getKindOperation() const {
        return kindOperation;
    }
    inline void setKindOperation ( KindOperation kindOperation ) {
        this->kindOperation = kindOperation;
    }
    inline void* getParam() const {
        return param;
    }
    inline void setParam ( void* param ) {
        this->param = param;
    }
    inline void* getPtrSource() const {
        return ptrSource;
    }
    inline void setPtrSource ( void* ptrSource ) {
        this->ptrSource = ptrSource;
    }
    inline void* getResult() const {
        return result;
    }
    inline void setResult ( void* result ) {
        this->result = result;
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
