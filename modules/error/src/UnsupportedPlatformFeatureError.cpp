#include "UnsupportedPlatformFeatureError.hpp"

using std::string;

namespace redengine {
namespace error {

	UnsupportedPlatformFeatureError::UnsupportedPlatformFeatureError(const string& feature) : feature(feature) {}

	UnsupportedPlatformFeatureError::UnsupportedPlatformFeatureError(const UnsupportedPlatformFeatureError& error)
			: Error(error), feature(error.feature) {}

	UnsupportedPlatformFeatureError::~UnsupportedPlatformFeatureError() {}

	REDSTRAIN_DEFINE_ERROR(UnsupportedPlatformFeatureError) {
		out << "Feature not supported on this platform: " << feature;
	}

}}
