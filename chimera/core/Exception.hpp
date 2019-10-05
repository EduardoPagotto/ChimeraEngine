#ifndef __CHIMERA_EXCEPTION__HPP
#define __CHIMERA_EXCEPTION__HPP

#include <exception>
#include <sstream>
#include <string>

namespace Chimera {

/**
 * Class Exception
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Exception : public std::exception {
  public:
    /**
     * Construtor Entity Limpa protegida
     */
    Exception() : std::exception() { message = "UNKNOWN_EXCEPTION"; }

    /**
     * Construtor de copia
     * @param exception
     */
    Exception(const Exception& _exception) { message = _exception.message; }

    /**
     * Construtor tipificado
     * @param _exceptionType codigo de tipode excessao
     * @param _code codigo do erro
     * @param _message messagem de erro
     */
    Exception(std::string _message) { message = _message; }

    /**
     * Destrutor Padrao
     */
    virtual ~Exception() throw() {}

    /**
     * Retorna mensagem de erro
     * @return
     */
    virtual const char* what() const throw() { return message.c_str(); }

    /**
     * Retorna mensagem de erro
     * @return String com mensagem de erro
     *
     */
    virtual std::string getMessage() const throw() { return message; }

  private:
    std::string message;
};

} // namespace Chimera
#endif
