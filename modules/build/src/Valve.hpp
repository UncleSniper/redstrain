#ifndef REDSTRAIN_MOD_BUILD_VALVE_HPP
#define REDSTRAIN_MOD_BUILD_VALVE_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Valve : public util::ReferenceCounted {

	  private:
		bool open;

	  public:
		Valve();
		Valve(const Valve&);

		inline bool isOpen() const {
			return open;
		}

		inline void setOpen(bool open) {
			this->open = open;
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_VALVE_HPP */
