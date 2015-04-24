#ifndef REDSTRAIN_MOD_ERROR_ERROR_HPP
#define REDSTRAIN_MOD_ERROR_ERROR_HPP

#include <string>
#include <iostream>

#include "api.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API Error {

	  private:
		unsigned refCount;

	  public:
		Error();
		Error(const Error&);
		virtual ~Error();

		void ref();
		void unref();

		virtual std::string getMessage() const;

		virtual void raise() const = 0;
		virtual Error* clone() const = 0;
		virtual void printMessage(std::ostream&) const = 0;
		virtual const char* getErrorType() const = 0;

		template<typename ErrorT>
		ErrorT* cloneAs() const {
			Error* error = clone();
			ErrorT* downcast = dynamic_cast<ErrorT*>(error);
			if(downcast)
				return downcast;
			delete error;
			return NULL;
		}

		static const char* getTypename();

	};

}}

#define REDSTRAIN_DECLARE_ERROR(type) \
  public: \
	virtual void raise() const; \
	virtual Error* clone() const; \
	virtual void printMessage(::std::ostream&) const; \
	virtual const char* getErrorType() const; \
	static const char* getTypename();
#define REDSTRAIN_DEFINE_ERROR(type) \
	void type::raise() const { \
		throw *this; \
	} \
	::redengine::error::Error* type::clone() const { \
		return new type(*this); \
	} \
	const char* type::getErrorType() const { \
		return #type; \
	} \
	const char* type::getTypename() { \
		return #type; \
	} \
	void type::printMessage(::std::ostream& out) const
#define REDSTRAIN_DECLARE_ABSTRACT_ERROR(type) \
  public: \
	static const char* getTypename();
#define REDSTRAIN_DEFINE_ABSTRACT_ERROR(type) \
	const char* type::getTypename() { \
		return #type; \
	}

#endif /* REDSTRAIN_MOD_ERROR_ERROR_HPP */
