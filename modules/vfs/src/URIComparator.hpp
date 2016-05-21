#ifndef REDSTRAIN_MOD_VFS_URICOMPARATOR_HPP
#define REDSTRAIN_MOD_VFS_URICOMPARATOR_HPP

#include "api.hpp"

namespace redengine {
namespace vfs {

	class URI;

	class REDSTRAIN_VFS_API URIComparator {

	  private:
		const URI& uri;

	  public:
		URIComparator(const URI&);
		URIComparator(const URIComparator&);

		inline const URI& getURI() const {
			return uri;
		}

		bool operator==(const URIComparator&);
		bool operator!=(const URIComparator&);
		bool operator<(const URIComparator&);
		bool operator<=(const URIComparator&);
		bool operator>(const URIComparator&);
		bool operator>=(const URIComparator&);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URICOMPARATOR_HPP */
