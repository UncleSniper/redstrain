#ifndef REDSTRAIN_MOD_BUILD_COPYTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_COPYTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CopyTransform : public OneToOneTransform<FileArtifact> {

	  public:
		CopyTransform(FileArtifact&);
		CopyTransform(const CopyTransform&);

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COPYTRANSFORM_HPP */
