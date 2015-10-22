#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP

#include <string>
#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace locale {

	class BlobMessageMapping;

	class REDSTRAIN_LOCALE_API MessageBlobRegistrar {

	  public:
		MessageBlobRegistrar(BlobMessageMapping*&, const std::string&, const std::string&, const char*, size_t);

		static void generateBlobRegistrar(io::OutputStream<char>&, const std::string&, const std::string&,
				const std::string&, const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP */
