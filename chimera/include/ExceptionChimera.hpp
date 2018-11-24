#ifndef __CHIMERA_EXCEPTION__HPP
#define __CHIMERA_EXCEPTION__HPP

#include <ExceptionBase.h>

namespace Chimera {

/**
 * Class ExceptionSDL
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class ExceptionChimera : public ExceptionBase {
  public:
    /**
     * Construtor de copia
     * @param ex
     */
    ExceptionChimera(const ExceptionChimera& _ex) : ExceptionBase(_ex) {}

    /**
     * Construtor com dados
     * @param _code
     * @param _message
     */
    ExceptionChimera(ExceptionCode _code, std::string _message)
        : ExceptionBase(ExceptionType::CHIMERA_EXCEPTION, _code, _message) {}

    /**
     * Destrutor Padrao
     */
    // virtual ~ExceptionSDL(){}
};

} // namespace Chimera
#endif
