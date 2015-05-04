#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETCLOSER_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETCLOSER_HPP

#include "Socket.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketCloser {

	  private:
		Socket& socket;
		bool closed;

	  private:
		SocketCloser(const SocketCloser&);

	  public:
		SocketCloser(Socket&);
		~SocketCloser();

		void close();

		inline bool isClosed() const {
			return closed;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETCLOSER_HPP */
