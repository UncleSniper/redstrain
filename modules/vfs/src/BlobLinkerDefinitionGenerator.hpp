#ifndef REDSTRAIN_MOD_VFS_BLOBLINKERDEFINITIONGENERATOR_HPP
#define REDSTRAIN_MOD_VFS_BLOBLINKERDEFINITIONGENERATOR_HPP

#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API BlobLinkerDefinitionGenerator {

	  public:
		class REDSTRAIN_VFS_API IncludeResolver {

		  public:
			IncludeResolver();
			IncludeResolver(const IncludeResolver&);
			virtual ~IncludeResolver();

			virtual void includeAliases(const std::string&, BlobLinkerDefinitionGenerator&) = 0;

			static void includeAliasesFrom(io::InputStream<char>&, const std::string&,
					BlobLinkerDefinitionGenerator&, IncludeResolver&);

		};

		class REDSTRAIN_VFS_API FileIncludeResolver : public IncludeResolver {

		  private:
			const std::string directory;

		  public:
			FileIncludeResolver(const std::string&);
			FileIncludeResolver(const FileIncludeResolver&);
			virtual ~FileIncludeResolver();

			virtual void includeAliases(const std::string&, BlobLinkerDefinitionGenerator&);

		};

	  private:
		const std::string inputStreamName;
		std::string pathPrefix, fileSuffix;
		io::InputStream<char>& input;
		io::FormattedInputStream<char> formattedInput;
		io::OutputStream<char>& output;
		io::DefaultConfiguredOutputStream<char>::Stream formattedOutput;
		IncludeResolver& includeResolver;
		unsigned nextID;
		bool needsHeader;

	  public:
		BlobLinkerDefinitionGenerator(io::InputStream<char>&, const std::string&,
				io::OutputStream<char>&, IncludeResolver&);
		BlobLinkerDefinitionGenerator(io::InputStream<char>&, const std::string&,
				const BlobLinkerDefinitionGenerator&, IncludeResolver&);
		BlobLinkerDefinitionGenerator(const BlobLinkerDefinitionGenerator&);

		inline const std::string& getPathPrefix() const {
			return pathPrefix;
		}

		void setPathPrefix(const std::string&);

		inline const std::string& getFileSuffix() const {
			return fileSuffix;
		}

		void setFileSuffix(const std::string&);

		inline io::InputStream<char>& getInputStream() {
			return input;
		}

		inline const io::InputStream<char>& getInputStream() const {
			return input;
		}

		inline io::OutputStream<char>& getOutputStream() {
			return output;
		}

		inline const io::OutputStream<char>& getOutputStream() const {
			return output;
		}

		inline IncludeResolver& getIncludeResolver() {
			return includeResolver;
		}

		inline const IncludeResolver& getIncludeResolver() const {
			return includeResolver;
		}

		inline unsigned getNextID() const {
			return nextID;
		}

		inline void setNextID(unsigned id) {
			nextID = id;
		}

		inline bool isNeedsHeader() const {
			return needsHeader;
		}

		inline bool setNeedsHeader(bool needsHeader) {
			this->needsHeader = needsHeader;
		}

		void defineBlobLinkers();

	};

}}

#endif /* REDSTRAIN_MOD_VFS_BLOBLINKERDEFINITIONGENERATOR_HPP */
