#ifndef REDSTRAIN_MOD_REDMOND_INDENTATIONCHAIN_HPP
#define REDSTRAIN_MOD_REDMOND_INDENTATIONCHAIN_HPP

#include "api.hpp"

namespace redengine {
namespace redmond {

	class REDSTRAIN_REDMOND_API IndentationChain {

	  public:
		IndentationChain();
		IndentationChain(const IndentationChain&);
		virtual ~IndentationChain();

		virtual unsigned indentInherited() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_REDMOND_INDENTATIONCHAIN_HPP */
