#ifndef REDSTRAIN_MOD_ERROR_ERROR_HPP
#define REDSTRAIN_MOD_ERROR_ERROR_HPP

#include <string>
#include <iostream>
#include <redstrain/redmond/LocalizerInjector.hpp>
#include <redstrain/redmond/l10nbind.hpp>

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
		virtual void printMessage(const locale::Locale&, text::StreamSink16&) const = 0;
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
	typedef void (*type ## LocalizerFunction)( \
		const type&, \
		const ::redengine::locale::Locale&, \
		::redengine::text::StreamSink16& \
	); \
	virtual void raise() const; \
	virtual Error* clone() const; \
	virtual void printMessage(::std::ostream&) const; \
	virtual void printMessage(const ::redengine::locale::Locale&, ::redengine::text::StreamSink16&) const; \
	virtual const char* getErrorType() const; \
	static const char* getTypename(); \
	static type ## LocalizerFunction localize ## type ## 16;

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
	void type::printMessage(const ::redengine::locale::Locale& locale, \
			::redengine::text::StreamSink16& sink) const { \
		if(!type::localize ## type ## 16) \
			throw ::redengine::error::NoErrorLocalizerRegisteredError(#type); \
		type::localize ## type ## 16(*this, locale, sink); \
	} \
	type::type ## LocalizerFunction type::localize ## type ## 16 = NULL; \
	void type::printMessage(::std::ostream& out) const

#define REDSTRAIN_DECLARE_ABSTRACT_ERROR(type) \
  public: \
	static const char* getTypename();

#define REDSTRAIN_DEFINE_ABSTRACT_ERROR(type) \
	const char* type::getTypename() { \
		return #type; \
	}

#define _REDSTRAIN_LOCALIZE_ERROR16_BASE(type) \
	static void localize ## type ## 16( \
		const type&, \
		const ::redengine::locale::Locale&, \
		::redengine::text::StreamSink16& \
	); \
	static ::redengine::redmond::LocalizerInjector<type::type ## LocalizerFunction> inject ## type ## Localizer16( \
		type::localize ## type ## 16, \
		localize ## type ## 16 \
	);

#define REDSTRAIN_LOCALIZE_ERROR16(type) \
	_REDSTRAIN_LOCALIZE_ERROR16_BASE(type) \
	static void localize ## type ## 16( \
		const type& error, \
		const ::redengine::locale::Locale& locale, \
		::redengine::text::StreamSink16& sink \
	)

#define REDSTRAIN_LOCALIZE_ERROR16_NOUSE(type) \
	_REDSTRAIN_LOCALIZE_ERROR16_BASE(type) \
	static void localize ## type ## 16( \
		const type&, \
		const ::redengine::locale::Locale& locale, \
		::redengine::text::StreamSink16& sink \
	)

#include "StateError.hpp"

#endif /* REDSTRAIN_MOD_ERROR_ERROR_HPP */
