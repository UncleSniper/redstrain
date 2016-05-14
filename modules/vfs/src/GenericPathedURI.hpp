#ifndef REDSTRAIN_MOD_VFS_GENERICPATHEDURI_HPP
#define REDSTRAIN_MOD_VFS_GENERICPATHEDURI_HPP

#include "PathedURI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API GenericPathedURI : public PathedURI {

	  protected:
		virtual URI* makeNormalizedURI(const text::String16&) const = 0;
		virtual bool isSchemeAndAuthorityEqual(const URI&) const = 0;
		virtual URI* makeRelativizedURI(const URI&, const text::String16&) const = 0;
		virtual URI* withFragment(const text::String16&) const = 0;
		virtual URI* makeResolvedURI(const URI&, bool, const text::String16&) const = 0;

	  public:
		GenericPathedURI();
		GenericPathedURI(const text::String16&);
		GenericPathedURI(const GenericPathedURI&);

		virtual URI* normalize() const;
		virtual URI* relativize(const URI&) const;
		virtual URI* resolve(const URI&) const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_GENERICPATHEDURI_HPP */
