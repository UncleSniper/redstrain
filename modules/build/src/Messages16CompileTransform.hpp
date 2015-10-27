#ifndef REDSTRAIN_MOD_BUILD_MESSAGES16COMPILETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGES16COMPILETRANSFORM_HPP

#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Messages16CompileTransform : public OneToOneTransform<FileArtifact> {

	  private:
		FileArtifact* specification;

	  public:
		Messages16CompileTransform(FileArtifact&);
		Messages16CompileTransform(const Messages16CompileTransform&);
		virtual ~Messages16CompileTransform();

		inline FileArtifact* getSpecificationArtifact() const {
			return specification;
		}

		void setSpecificationArtifact(FileArtifact*);

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGES16COMPILETRANSFORM_HPP */
