#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "CopyTransform.hpp"

using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CopyTransform::CopyTransform(FileArtifact& source) : OneToOneTransform<FileArtifact>(source) {}

	CopyTransform::CopyTransform(const CopyTransform& transform) : OneToOneTransform<FileArtifact>(transform) {}

	void CopyTransform::perform(BuildContext& context, Artifact& target) {
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		Delete<OutputStream<char> > out(target.getOutputStream(context));
		StreamCloser outCloser(*out);
		in->copyTo(**out);
		inCloser.close();
		outCloser.close();
		target.notifyModified(context);
	}

	void CopyTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CopyTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
