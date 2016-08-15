#ifndef REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP

#include <redstrain/util/UniqueList.hpp>

#include "AbstractArtifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileArtifact : public AbstractArtifact {

	  private:
		typedef util::UniqueList<FileArtifact*> FileArtifacts;
		typedef FileArtifacts::Iterator FileArtifactIterator;

	  private:
		const std::string path, label;
		FileArtifacts intermediateDirectories;

	  private:
		void notifyIntermediateDirectoriesCreated(const Mood&, BuildContext&) const;
		void getFileReferenceForOutput(ReferenceMood, BuildContext&, const ComponentUIInfo*) const;

	  public:
		FileArtifact(const std::string&, const std::string&);
		FileArtifact(const FileArtifact&);
		virtual ~FileArtifact();

		inline const std::string& getPath() const {
			return path;
		}

		void addIntermediateDirectories(BuildContext&, const std::string&);

		virtual void wouldModify(BuildContext&);

		virtual bool isPresent();
		virtual void getModificationTimestamp(util::Appender<time_t>&);
		virtual void notifyModified(BuildContext&);
		virtual void remove();

		virtual io::InputStream<char>* getInputStream(BuildContext&);
		virtual io::OutputStream<char>* getOutputStream(BuildContext&, ReferenceMood, const ComponentUIInfo*);
		virtual void getFileReference(const std::string&, util::Appender<std::string>&, ReferenceDirection,
				ReferenceMood, BuildContext&, const ComponentUIInfo*);

		virtual bool isTransformable();
		virtual std::string getName();
		virtual std::string getLabel();
		virtual std::string getCanonicalReference();

		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP */
