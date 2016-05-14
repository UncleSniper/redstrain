#ifndef REDSTRAIN_MOD_VFS_QUERIEDURI_HPP
#define REDSTRAIN_MOD_VFS_QUERIEDURI_HPP

#include "URI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API QueriedURI : public virtual URI {

	  protected:
		bool queryDefined;
		text::String16 query;
		std::string queryOctets;

	  protected:
		void setQueriedOctetsFromRendition(const text::String16&, util::MemorySize,
				void (*byteizeChar)(text::Char16, std::string&));

		virtual void queriedDecodeOriginal(const std::string&, std::string&) const;
		virtual void queriedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void queriedDecodeOriginal(const std::string&, text::String32&) const;

	  public:
		QueriedURI(bool);
		QueriedURI(const text::String16&);
		QueriedURI(const QueriedURI&);
		virtual ~QueriedURI();

		virtual bool hasQuery() const;
		virtual std::string getRawQuery8() const;
		virtual text::String16 getRawQuery16() const;
		virtual text::String32 getRawQuery32() const;
		virtual std::string getQuery() const;
		virtual std::string getQuery8() const;
		virtual text::String16 getQuery16() const;
		virtual text::String32 getQuery32() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_QUERIEDURI_HPP */
