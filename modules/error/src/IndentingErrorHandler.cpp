#include "IndentingErrorHandler.hpp"

namespace redengine {
namespace error {

	// ======== Indenter ========

	IndentingErrorHandler::Indenter::Indenter() {}

	IndentingErrorHandler::Indenter::Indenter(const Indenter&) {}

	IndentingErrorHandler::Indenter::~Indenter() {}

	// ======== IndentingErrorHandler ========

	IndentingErrorHandler::IndentingErrorHandler(Indenter& indenter) : indenter(indenter) {}

	IndentingErrorHandler::IndentingErrorHandler(const IndentingErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), indenter(handler.indenter) {}

	void IndentingErrorHandler::prepareLine() {
		indenter.indent();
	}

	void IndentingErrorHandler::endLine() {
		indenter.endLine();
	}

}}
