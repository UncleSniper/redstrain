#ifndef REDSTRAIN_MOD_BUILD_CYCLICDEPENDENCYERROR_HPP
#define REDSTRAIN_MOD_BUILD_CYCLICDEPENDENCYERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class Component;

	class REDSTRAIN_BUILD_API CyclicDependencyError : public BuildError {

	  private:
		Component* component;

	  public:
		CyclicDependencyError(Component&);
		CyclicDependencyError(const CyclicDependencyError&);
		virtual ~CyclicDependencyError();

		inline const Component& getComponent() const {
			return *component;
		}

		REDSTRAIN_DECLARE_ERROR(CyclicDependencyError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CYCLICDEPENDENCYERROR_HPP */
