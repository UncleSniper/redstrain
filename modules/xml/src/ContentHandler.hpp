#ifndef REDSTRAIN_MOD_XML_CONTENTHANDLER_HPP
#define REDSTRAIN_MOD_XML_CONTENTHANDLER_HPP

#include <redstrain/text/types.hpp>
#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace xml {

	class Attributes;

	class REDSTRAIN_XML_API ContentHandler {

	  public:
		ContentHandler();
		ContentHandler(const ContentHandler&);
		virtual ~ContentHandler();

		virtual void startDocument() = 0;
		virtual void processingInstruction(const text::String32&, const text::String32&) = 0;
		virtual void characters(const text::Char32*, util::MemorySize) = 0;
		virtual void ignorableWhitespace(const text::Char32*, util::MemorySize) = 0;
		virtual void startPrefixMapping(const text::String32&, const text::String32&) = 0;
		virtual void endPrefixMapping(const text::String32&) = 0;
		virtual void startElement(const text::String32&, const text::String32&, const text::String32&,
				const Attributes&) = 0;
		virtual void endElement(const text::String32&, const text::String32&, const text::String32&) = 0;
		virtual void skippedEntity(const text::String32&) = 0;
		virtual void endDocument() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_XML_CONTENTHANDLER_HPP */
