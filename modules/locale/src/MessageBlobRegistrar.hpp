#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP

#include <string>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/io/OutputStream.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace locale {

	class BlobMessageMapping;

	class REDSTRAIN_LOCALE_API MessageBlobRegistrar {

	  public:
		class REDSTRAIN_LOCALE_API IncludeResolver {

		  public:
			IncludeResolver();
			IncludeResolver(const IncludeResolver&);
			virtual ~IncludeResolver();

			virtual void includeBlobs(const std::string&, io::OutputStream<char>&, bool&,
					const std::string&, const std::string&, unsigned&) = 0;

		};

		class REDSTRAIN_LOCALE_API FileIncludeResolver : public IncludeResolver {

		  private:
			const std::string directory;

		  public:
			FileIncludeResolver(const std::string&);
			FileIncludeResolver(const FileIncludeResolver&);
			virtual ~FileIncludeResolver();

			virtual void includeBlobs(const std::string&, io::OutputStream<char>&, bool&,
					const std::string&, const std::string&, unsigned&);

		};

	  public:
		MessageBlobRegistrar(BlobMessageMapping*&, const std::string&, const std::string&, const char*, size_t);

		static void generateBlobRegistrar(io::OutputStream<char>&, const std::string&, const std::string&,
				const std::string&, const std::string&, bool, unsigned);
		static void generateBlobAliases(io::InputStream<char>&, const std::string&, IncludeResolver&,
				io::OutputStream<char>&, bool&, const std::string&, const std::string&, unsigned&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEBLOBREGISTRAR_HPP */
