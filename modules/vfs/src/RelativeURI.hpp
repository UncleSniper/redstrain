#ifndef REDSTRAIN_MOD_VFS_RELATIVEURI_HPP
#define REDSTRAIN_MOD_VFS_RELATIVEURI_HPP

#include "HierarchicalURI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API RelativeURI : public HierarchicalURI {

	  protected:
		virtual URI* newURIOfThisType(int, const text::String16&, const text::String16&,
				const text::String16&, const text::String16&, uint16_t,
				const text::String16&, const text::String16&, const text::String16&) const;

	  public:
		RelativeURI(const text::String16&, const Decomposition&);
		RelativeURI(const Components<text::Char16, CL_RENDITION>&);
		RelativeURI(const Components<char, CL_ORIGINAL>&);
		RelativeURI(const Components<text::Char16, CL_ORIGINAL>&);
		RelativeURI(const Components<text::Char32, CL_ORIGINAL>&);
		RelativeURI(const std::string&);
		RelativeURI(const text::String16&);
		RelativeURI(const text::String32&);
		RelativeURI(const std::string&, const std::string&, const std::string&);
		RelativeURI(const text::String16&, const text::String16&, const text::String16&);
		RelativeURI(const text::String32&, const text::String32&, const text::String32&);
		RelativeURI(const RelativeURI&);

		virtual bool isRelativeReference() const;
		virtual bool isLocator() const;
		virtual URIAcquisition<io::InputStream<char> >* acquireResource() const;
		virtual URIAcquisition<VFS>* acquireVFS() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_RELATIVEURI_HPP */
