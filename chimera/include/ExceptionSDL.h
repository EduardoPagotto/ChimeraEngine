#ifndef EXCEPTIONSDL_H_
#define EXCEPTIONSDL_H_

#include <Exception.h>

namespace Chimera {

/**
 * Class ExceptionSDL
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class ExceptionSDL: public Exception {
public:

    /**
     * Construtor de copia
     * @param ex
     */
    ExceptionSDL ( const ExceptionSDL &ex ) : Exception ( ex ) {}

    /**
     * Construtor com dados
     * @param _code
     * @param _message
     */
    ExceptionSDL ( int _code, std::string _message ) : Exception ( SDL_EXCEPTION, _code, _message ) {}

    /**
     * Destrutor Padrao
     */
    //virtual ~ExceptionSDL(){}

};

} /* namespace Chimera */
#endif /* EXCEPTIONSDL_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
