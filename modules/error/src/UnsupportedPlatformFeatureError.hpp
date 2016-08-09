#ifndef REDSTRAIN_MOD_ERROR_UNSUPPORTEDPLATFORMFEATUREERROR_HPP
#define REDSTRAIN_MOD_ERROR_UNSUPPORTEDPLATFORMFEATUREERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API UnsupportedPlatformFeatureError : public Error {

	  private:
		const std::string feature;

	  public:
		UnsupportedPlatformFeatureError(const std::string&);
		UnsupportedPlatformFeatureError(const UnsupportedPlatformFeatureError&);
		virtual ~UnsupportedPlatformFeatureError();

		inline const std::string& getFeature() const {
			return feature;
		}

		REDSTRAIN_DECLARE_ERROR(UnsupportedPlatformFeatureError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_UNSUPPORTEDPLATFORMFEATUREERROR_HPP */
