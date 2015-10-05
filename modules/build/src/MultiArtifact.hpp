#ifndef REDSTRAIN_MOD_BUILD_MULTIARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_MULTIARTIFACT_HPP

#include <redstrain/util/UniqueList.hpp>

#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MultiArtifact : public Artifact {

	  private:
		typedef util::UniqueList<Artifact*> Artifacts;

	  public:
		typedef Artifacts::Iterator ArtifactIterator;

	  private:
		Artifacts artifacts;
		std::string label;

	  private:
		Artifact& checkSingleChild(const char*);

	  public:
		MultiArtifact(const std::string& = "");
		MultiArtifact(const MultiArtifact&);
		virtual ~MultiArtifact();

		bool addArtifact(Artifact&);
		bool removeArtifact(Artifact&);
		void clearArtifacts();
		void getArtifacts(ArtifactIterator&, ArtifactIterator&) const;

		void setLabel(const std::string&);

		virtual bool wouldBePresent();
		virtual void getPredictedModificationTimestamp(util::Appender<time_t>&);
		virtual void getPredictedVirtualClock(util::Appender<time_t>&);
		virtual void wouldModify(BuildContext&);
		virtual void wouldRemove();

		virtual bool isPresent();
		virtual void getModificationTimestamp(util::Appender<time_t>&);
		virtual void getVirtualClock(util::Appender<time_t>&);
		virtual void notifyModified(BuildContext&);
		virtual void remove();

		virtual io::InputStream<char>* getInputStream(BuildContext&);
		virtual io::OutputStream<char>* getOutputStream(BuildContext&, ReferenceMood);
		virtual void getFileReference(const std::string&, util::Appender<std::string>&, ReferenceDirection,
				ReferenceMood, BuildContext&);

		virtual bool isTransformable();
		virtual std::string getName();
		virtual std::string getLabel();

		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MULTIARTIFACT_HPP */
