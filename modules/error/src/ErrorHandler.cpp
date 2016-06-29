#include <iostream>
#include <exception>
#include <redstrain/util/Unref.hpp>

#include "ErrorHandler.hpp"
#include "ErrorHandlingError.hpp"

using std::cerr;
using std::endl;
using std::exception;
using redengine::util::Unref;

namespace redengine {
namespace error {

	ErrorHandler::ErrorHandler() {}

	ErrorHandler::ErrorHandler(const ErrorHandler&) {}

	ErrorHandler::~ErrorHandler() {}

	void ErrorHandler::handleErrorSafely(const Error& originalError) {
		Unref<Error> oecpy(originalError.clone());
		try {
			handleError(originalError);
		}
		catch(const Error& subsequentError) {
			Unref<Error> secpy(subsequentError.clone());
			throw ErrorHandlingError(*oecpy, *secpy);
		}
	}

	bool ErrorHandler::handleErrorCompletely(const Error& error) {
		try {
			handleErrorSafely(error);
			return false;
		}
		catch(const ErrorHandlingError& error) {
			error.printMessage(cerr);
			cerr << endl;
			return true;
		}
		catch(const exception& error) {
			cerr << "STL exception: " << error.what() << endl;
			return true;
		}
		catch(...) {
			cerr << "Unhandled exception" << endl;
			return true;
		}
	}

}}
