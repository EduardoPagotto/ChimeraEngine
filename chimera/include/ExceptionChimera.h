#ifndef EXCEPTION_CHIMERA_H_
#define EXCEPTION_CHIMERA_H_

#include <Exception.h>

namespace Chimera
{

/**
 * Class ExceptionSDL
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class ExceptionChimera : public Exception
{
public:

    /**
     * Construtor de copia
     * @param ex
     */
    ExceptionChimera ( const ExceptionChimera &_ex ) : Exception ( _ex ) {}

    /**
     * Construtor com dados
     * @param _code
     * @param _message
     */
    ExceptionChimera ( ExceptionCode _code, std::string _message ) : Exception ( ExceptionType::CHIMERA_EXCEPTION, _code, _message ) {}

    /**
     * Destrutor Padrao
     */
    //virtual ~ExceptionSDL(){}

};

} /* namespace Chimera */
#endif /* EXCEPTIONSDL_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
