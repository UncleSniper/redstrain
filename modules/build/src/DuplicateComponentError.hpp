#ifndef REDSTRAIN_MOD_BUILD_DUPLICATECOMPONENTERROR_HPP
#define REDSTRAIN_MOD_BUILD_DUPLICATECOMPONENTERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DuplicateComponentError : public BuildError {

	  private:
		const std::string component;

	  public:
		DuplicateComponentError(const std::string&);
		DuplicateComponentError(const DuplicateComponentError&);
		virtual ~DuplicateComponentError();

		inline const std::string& getComponentName() const {
			return component;
		}

		REDSTRAIN_DECLARE_ERROR(DuplicateComponentError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DUPLICATECOMPONENTERROR_HPP */
