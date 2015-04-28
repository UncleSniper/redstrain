#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETERROR_HPP

#include "NetworkSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocketError : public NetworkSocketError {

	  public:
		DatagramSocketError();
		DatagramSocketError(const DatagramSocketError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(DatagramSocketError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETERROR_HPP */
