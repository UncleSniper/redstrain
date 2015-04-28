#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETIOERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETIOERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketIOError : public virtual SocketError {

	  public:
		enum Direction {
			SEND,
			RECEIVE
		};

	  protected:
		const Direction dir;

	  protected:
		void printDirection(std::ostream&) const;

	  public:
		SocketIOError(Direction);
		SocketIOError(const SocketIOError&);

		inline Direction getDirection() const {
			return dir;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketIOError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETIOERROR_HPP */
