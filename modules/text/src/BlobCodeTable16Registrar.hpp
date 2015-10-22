#ifndef REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP
#define REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP

#include <map>
#include <list>
#include <redstrain/util/MapKeyIterator.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace text {

	class CodecManager;

	class REDSTRAIN_TEXT_API BlobCodeTable16Registrar {

	  public:
		class GeneratorWriter;

		class REDSTRAIN_TEXT_API NameCache {

		  private:
			typedef std::list<std::string> Names;
			typedef std::map<std::string, Names> NameMap;
			typedef NameMap::iterator NameMapIterator;
			typedef NameMap::const_iterator NameMapConstIterator;

		  public:
			typedef util::MapKeyIterator<std::string, Names> BlobIterator;
			typedef Names::const_iterator NameIterator;

		  private:
			NameMap nameMap;

		  public:
			NameCache();
			NameCache(const NameCache&);

			void getBlobs(BlobIterator&, BlobIterator&) const;
			void getNames(const std::string&, NameIterator&, NameIterator&) const;
			void addBlobName(const std::string&, const std::string&);
			void clearNames(const std::string&);
			void clearNames();

			void generateBlobRegistrars(GeneratorWriter&) const;

		};

		class REDSTRAIN_TEXT_API GeneratorReader {

		  public:
			class REDSTRAIN_TEXT_API IncludeResolver {

			  public:
				IncludeResolver();
				IncludeResolver(const IncludeResolver&);
				virtual ~IncludeResolver();

				virtual void includeBlobs(const std::string&, const GeneratorReader&) = 0;

				static void includeBlobsFrom(io::InputStream<char>&, const std::string&,
						const GeneratorReader&, IncludeResolver&);

			};

			class REDSTRAIN_TEXT_API FileIncludeResolver : public IncludeResolver {

			  private:
				const std::string directory;

			  public:
				FileIncludeResolver(const std::string&);
				FileIncludeResolver(const FileIncludeResolver&);
				virtual ~FileIncludeResolver();

				virtual void includeBlobs(const std::string&, const GeneratorReader&);

			};

		  private:
			const std::string inputStreamName;
			io::InputStream<char>& input;
			io::FormattedInputStream<char> formattedInput;
			NameCache& nameCache;
			IncludeResolver& includeResolver;

		  public:
			GeneratorReader(io::InputStream<char>&, const std::string&, NameCache&, IncludeResolver&);
			GeneratorReader(io::InputStream<char>&, const std::string&, const GeneratorReader&, IncludeResolver&);
			GeneratorReader(const GeneratorReader&);

			inline io::InputStream<char>& getInputStream() {
				return input;
			}

			inline const io::InputStream<char>& getInputStream() const {
				return input;
			}

			inline NameCache& getNameCache() {
				return nameCache;
			}

			inline const NameCache& getNameCache() const {
				return nameCache;
			}

			inline IncludeResolver& getIncludeResolver() {
				return includeResolver;
			}

			inline const IncludeResolver& getIncludeResolver() const {
				return includeResolver;
			}

			void readBlobNames();

		};

		class REDSTRAIN_TEXT_API GeneratorWriter {

		  public:
			class REDSTRAIN_TEXT_API BlobSymbolMapper {

			  public:
				BlobSymbolMapper();
				BlobSymbolMapper(const BlobSymbolMapper&);
				virtual ~BlobSymbolMapper();

				virtual std::string mapBlobSymbol(const std::string&) = 0;

			};

			class REDSTRAIN_TEXT_API DefaultBlobSymbolMapper : public BlobSymbolMapper {

			  private:
				std::string nsPrefix;

			  public:
				DefaultBlobSymbolMapper();
				DefaultBlobSymbolMapper(const std::string&);
				DefaultBlobSymbolMapper(const DefaultBlobSymbolMapper&);
				virtual ~DefaultBlobSymbolMapper();

				inline const std::string& getNamespace() const {
					return nsPrefix;
				}

				void setNamespace(const std::string&);

				virtual std::string mapBlobSymbol(const std::string&);

			};

		  private:
			io::OutputStream<char>& output;
			io::DefaultConfiguredOutputStream<char>::Stream formattedOutput;
			BlobSymbolMapper* blobSymbolMapper;
			bool needsHeader;
			unsigned nextID;

		  public:
			GeneratorWriter(io::OutputStream<char>&, BlobSymbolMapper*);
			GeneratorWriter(const GeneratorWriter&);

			inline io::OutputStream<char>& getOutputStream() {
				return output;
			}

			inline const io::OutputStream<char>& getOutputStream() const {
				return output;
			}

			inline BlobSymbolMapper* getBlobSymbolMapper() const {
				return blobSymbolMapper;
			}

			inline void setBlobSymbolMapper(BlobSymbolMapper* mapper) {
				blobSymbolMapper = mapper;
			}

			void generateBlobRegistrar(const std::string&, NameCache::NameIterator, NameCache::NameIterator);

		};

	  public:
		BlobCodeTable16Registrar(const char*, size_t, const char *const*, const char*);

		static void registerCodecs(CodecManager&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP */
