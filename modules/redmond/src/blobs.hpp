#ifndef REDSTRAIN_MOD_REDMOND_BLOBS_HPP
#define REDSTRAIN_MOD_REDMOND_BLOBS_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace redmond {

	REDSTRAIN_REDMOND_API void registerBlobModule(const std::string&);
	REDSTRAIN_REDMOND_API bool hasBlobModule(const std::string&);

	class REDSTRAIN_REDMOND_API BlobModuleRegistrar {

	  private:
		const std::string name;

	  public:
		BlobModuleRegistrar(const std::string&);
		BlobModuleRegistrar(const BlobModuleRegistrar&);

		inline const std::string& getName() const {
			return name;
		}

	};

}}

#endif /* REDSTRAIN_MOD_REDMOND_BLOBS_HPP */
