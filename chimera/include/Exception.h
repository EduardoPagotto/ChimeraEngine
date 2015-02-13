#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>

namespace Chimera {

	/**
	 * Enum ExceptionType
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	enum class ExceptionType {
		UNKNOWN_EXCEPTION = 0,
		UNKNOWN_EXCEPTION_STR,
		SDL_EXCEPTION,
		CHIMERA_EXCEPTION
	};

	enum class ExceptionCode {
		UNKNOWN,
		CREATE,
		DESTROY,
		OPEN,
		CLOSE,
		ALLOC,
		DEALLOC,
		SEND,
		RECEIVE,
		READ,
		WRITE,
		TIME_OUT,
		MESSAGE
	};

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
		Exception() : std::exception() {
			message = "UNKNOWN_EXCEPTION";
			code = ExceptionCode::UNKNOWN;
			exceptionType = ExceptionType::UNKNOWN_EXCEPTION;
		}

		/**
		 * Construtor de copia
		 * @param exception
		 */
		Exception(const Exception &_exception) {
			message = _exception.message;
			code = _exception.code;
			exceptionType = _exception.exceptionType;
		}

		/**
		 * Construtor tipificado
		 * @param _exceptionType codigo de tipode excessao
		 * @param _code codigo do erro
		 * @param _message messagem de erro
		 */
		Exception(ExceptionType _exceptionType, ExceptionCode _code, std::string _message) {
			message = _message;
			code = _code;
			exceptionType = _exceptionType;
		}

		/**
		 * Destrutor Padrao
		 */
		virtual ~Exception() throw() {}

		/**
		 * Retorna mensagem de erro
		 * @return
		 */
		virtual const char* what() const throw() {
			return message.c_str();
		}

		/**
		 * Retorna tipo de excessao
		 * @return
		 */
		virtual ExceptionType getExceptionType() const throw() {
			return exceptionType;
		}

		/**
		 * Retorna codigo de erro
		 * @return
		 */
		virtual ExceptionCode getCode() const throw() {
			return code;
		}

		/**
		 * Retorna mensagem de erro
		 * @return
		 */
		virtual std::string getMessage() const throw() {
			return message;
		}

	private:

		std::string message;
		ExceptionCode code;
		ExceptionType exceptionType;

	};

} /* namespace Chimera */
#endif /* EXCEPTION_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
