#ifndef REDSTRAIN_MOD_BUILD_NOSUCHCOMPONENTERROR_HPP
#define REDSTRAIN_MOD_BUILD_NOSUCHCOMPONENTERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API NoSuchComponentError : public BuildError {

	  private:
		const std::string component;

	  public:
		NoSuchComponentError(const std::string&);
		NoSuchComponentError(const NoSuchComponentError&);
		virtual ~NoSuchComponentError();

		inline const std::string& getComponentName() const {
			return component;
		}

		REDSTRAIN_DECLARE_ERROR(NoSuchComponentError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NOSUCHCOMPONENTERROR_HPP */
