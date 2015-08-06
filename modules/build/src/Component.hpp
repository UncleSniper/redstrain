#ifndef REDSTRAIN_MOD_BUILD_COMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENT_HPP

#include <string>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Component : public util::ReferenceCounted {

	  public:
		enum REDSTRAIN_BUILD_API Type {
			LIBRARY,
			EXECUTABLE,
			DATA,
			BLOB
		};

	  private:
		const Type type;
		const std::string name, baseDirectory;

	  public:
		Component(Type, const std::string&, const std::string&);
		Component(const Component&);

		inline Type getType() const {
			return type;
		}

		inline const std::string& getName() const {
			return name;
		}

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENT_HPP */
