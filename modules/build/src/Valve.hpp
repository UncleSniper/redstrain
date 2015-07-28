#ifndef REDSTRAIN_MOD_BUILD_VALVE_HPP
#define REDSTRAIN_MOD_BUILD_VALVE_HPP

#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

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

		virtual void dumpValve(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_VALVE_HPP */
