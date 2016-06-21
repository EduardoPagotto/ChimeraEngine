#ifndef EXCEPTIONSDL_H_
#define EXCEPTIONSDL_H_

#include <Exception.h>

namespace Chimera
{

/**
 * Class ExceptionSDL
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class ExceptionSDL : public Exception
{
public:

    /**
     * Construtor de copia
     * @param ex
     */
    ExceptionSDL ( const ExceptionSDL &_ex ) : Exception ( _ex ) {}

    /**
     * Construtor com dados
     * @param _code
     * @param _message
     */
    ExceptionSDL ( ExceptionCode _code, std::string _msg ) : Exception ( ExceptionType::SDL_EXCEPTION, _code, _msg ) {}

    /**
     * Destrutor Padrao
     */
    //virtual ~ExceptionSDL(){}

};

} /* namespace Chimera */
#endif /* EXCEPTIONSDL_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
