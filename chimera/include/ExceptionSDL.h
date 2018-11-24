#ifndef __CHIMERA_EXCEPTION_SDL__HPP
#define __CHIMERA_EXCEPTION_SDL__HPP

#include <ExceptionBase.h>

namespace Chimera {

/**
 * Class ExceptionSDL
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class ExceptionSDL : public ExceptionBase {
  public:
    /**
     * Construtor de copia
     * @param ex
     */
    ExceptionSDL(const ExceptionSDL& _ex) : ExceptionBase(_ex) {}

    /**
     * Construtor com dados
     * @param _code
     * @param _message
     */
    ExceptionSDL(ExceptionCode _code, std::string _msg)
        : ExceptionBase(ExceptionType::SDL_EXCEPTION, _code, _msg) {}

    /**
     * Destrutor Padrao
     */
    // virtual ~ExceptionSDL(){}
};
} // namespace Chimera
#endif
