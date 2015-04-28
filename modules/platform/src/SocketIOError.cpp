#include "SocketIOError.hpp"

using std::ostream;

namespace redengine {
namespace platform {

	SocketIOError::SocketIOError(Direction dir) : dir(dir) {}

	SocketIOError::SocketIOError(const SocketIOError& error)
			: IOError(error), SocketError(error), dir(error.dir) {}

	void SocketIOError::printDirection(ostream& out) const {
		out << "Failed to ";
		switch(dir) {
			case SEND:
				out << "send";
				break;
			case RECEIVE:
				out << "receive";
				break;
			default:
				out << "send/receive";
				break;
		}
		out << " data";
	}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketIOError)

}}
