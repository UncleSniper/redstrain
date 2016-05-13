#ifndef REDSTRAIN_MOD_VFS_ABSTRACTURI_HPP
#define REDSTRAIN_MOD_VFS_ABSTRACTURI_HPP

#include "URI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AbstractURI : public virtual URI {

	  private:
		const std::string scheme;

	  public:
		AbstractURI(const std::string&);
		AbstractURI(const AbstractURI&);
		virtual ~AbstractURI();

		virtual std::string getScheme8() const;
		virtual text::String16 getScheme16() const;
		virtual text::String32 getScheme32() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ABSTRACTURI_HPP */
