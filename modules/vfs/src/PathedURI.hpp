#ifndef REDSTRAIN_MOD_VFS_PATHEDURI_HPP
#define REDSTRAIN_MOD_VFS_PATHEDURI_HPP

#include "URI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API PathedURI : public virtual URI {

	  protected:
		text::String16 path;
		std::string pathOctets;

	  protected:
		void setPathedOctetsFromRendition(const text::String16&, util::MemorySize,
				void (*byteizeChar)(text::Char16, std::string&));

		virtual void pathedDecodeOriginal(const std::string&, std::string&) const;
		virtual void pathedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void pathedDecodeOriginal(const std::string&, text::String32&) const;

	  public:
		PathedURI();
		PathedURI(const text::String16&);
		PathedURI(const PathedURI&);
		virtual ~PathedURI();

		virtual std::string getRawPath8() const;
		virtual text::String16 getRawPath16() const;
		virtual text::String32 getRawPath32() const;
		virtual std::string getPath() const;
		virtual std::string getPath8() const;
		virtual text::String16 getPath16() const;
		virtual text::String32 getPath32() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_PATHEDURI_HPP */
