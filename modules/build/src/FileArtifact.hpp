#ifndef REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP

#include "AbstractArtifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileArtifact : public AbstractArtifact {

	  private:
		const std::string path, label;

	  public:
		FileArtifact(const std::string&, const std::string&);
		FileArtifact(const FileArtifact&);

		inline const std::string& getPath() const {
			return path;
		}

		virtual bool isPresent();
		virtual void getModificationTimestamp(util::Appender<time_t>&);
		virtual void remove();

		virtual io::InputStream<char>* getInputStream();
		virtual io::OutputStream<char>* getOutputStream();
		virtual void getFileReference(const std::string&, util::Appender<std::string>&, ReferenceDirection);

		virtual bool isTransformable();
		virtual std::string getName();
		virtual std::string getLabel();

		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP */
