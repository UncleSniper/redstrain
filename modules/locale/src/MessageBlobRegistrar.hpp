#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace locale {

	class BlobMessageMapping;

	class REDSTRAIN_LOCALE_API MessageBlobRegistrar {

	  public:
		MessageBlobRegistrar(BlobMessageMapping*&, const std::string&, const std::string&, const char*, size_t);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP */
