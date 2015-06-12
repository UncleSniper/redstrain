#include "StopExecution.hpp"

namespace redengine {
namespace cmdline {

	StopExecution::StopExecution(int status) : status(status) {}

	StopExecution::StopExecution(const StopExecution& stop) : status(stop.status) {}

}}
