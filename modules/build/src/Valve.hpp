#ifndef REDSTRAIN_MOD_BUILD_VALVE_HPP
#define REDSTRAIN_MOD_BUILD_VALVE_HPP

#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildContext;

	class REDSTRAIN_BUILD_API Valve : public util::ReferenceCounted {

	  public:
		Valve();
		Valve(const Valve&);
		virtual ~Valve();

		virtual bool isOpen(BuildContext&) = 0;

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpValve(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_VALVE_HPP */
