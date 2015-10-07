#ifndef REDSTRAIN_MOD_BUILD_CODETABLECOMPILETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLECOMPILETRANSFORM_HPP

#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTableCompileTransform : public OneToOneTransform<FileArtifact> {

	  public:
		CodeTableCompileTransform(FileArtifact&);
		CodeTableCompileTransform(const CodeTableCompileTransform&);

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLECOMPILETRANSFORM_HPP */
