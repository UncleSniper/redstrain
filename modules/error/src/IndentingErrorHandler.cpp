#include "IndentingErrorHandler.hpp"

namespace redengine {
namespace error {

	// ======== Indenter ========

	IndentingErrorHandler::Indenter::Indenter() {}

	IndentingErrorHandler::Indenter::Indenter(const Indenter&) {}

	IndentingErrorHandler::Indenter::~Indenter() {}

	// ======== IndentingErrorHandler ========

	IndentingErrorHandler::IndentingErrorHandler(Indenter& indenter) : indenter(indenter), currentColumn(0u) {}

	IndentingErrorHandler::IndentingErrorHandler(const IndentingErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), IndentationChain(handler),
			indenter(handler.indenter), currentColumn(handler.currentColumn) {}

	void IndentingErrorHandler::advanceCurrentColumn(unsigned columnDelta) {
		currentColumn += columnDelta;
	}

	void IndentingErrorHandler::resetCurrentColumn() {
		currentColumn = 0u;
	}

	void IndentingErrorHandler::prepareLine() {
		currentColumn += indenter.indent();
	}

	void IndentingErrorHandler::endLine() {
		indenter.endLine();
		currentColumn = 0u;
	}

	unsigned IndentingErrorHandler::indentInherited() {
		unsigned columns = indenter.indent();
		currentColumn += columns;
		return columns;
	}

}}
