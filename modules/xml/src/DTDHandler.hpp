#ifndef REDSTRAIN_MOD_XML_DTDHANDLER_HPP
#define REDSTRAIN_MOD_XML_DTDHANDLER_HPP

#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace xml {

	class REDSTRAIN_XML_API DTDHandler {

	  public:
		DTDHandler();
		DTDHandler(const DTDHandler&);
		virtual ~DTDHandler();

		virtual void notationDecl(const text::String32&, const text::String32&, const text::String32&) = 0;
		virtual void unparsedEntityDecl(const text::String32&, const text::String32&,
				const text::String32&, const text::String32&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_XML_DTDHANDLER_HPP */
