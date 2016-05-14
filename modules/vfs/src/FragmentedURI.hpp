#ifndef REDSTRAIN_MOD_VFS_FRAGMENTEDURI_HPP
#define REDSTRAIN_MOD_VFS_FRAGMENTEDURI_HPP

#include "URI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FragmentedURI : public virtual URI {

	  protected:
		bool fragmentDefined;
		text::String16 fragment;
		std::string fragmentOctets;

	  protected:
		void setFragmentedOctetsFromRendition(const text::String16&, util::MemorySize,
				void (*byteizeChar)(text::Char16, std::string&));

		virtual void fragmentedDecodeOriginal(const std::string&, std::string&) const;
		virtual void fragmentedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void fragmentedDecodeOriginal(const std::string&, text::String32&) const;

	  public:
		FragmentedURI(bool);
		FragmentedURI(const text::String16&);
		FragmentedURI(const FragmentedURI&);
		virtual ~FragmentedURI();

		virtual bool hasFragment() const;
		virtual std::string getRawFragment8() const;
		virtual text::String16 getRawFragment16() const;
		virtual text::String32 getRawFragment32() const;
		virtual std::string getFragment() const;
		virtual std::string getFragment8() const;
		virtual text::String16 getFragment16() const;
		virtual text::String32 getFragment32() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FRAGMENTEDURI_HPP */
