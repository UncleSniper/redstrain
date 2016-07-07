#ifndef REDSTRAIN_TOOLS_MKGRISU_BLOBDEPEND_HPP
#define REDSTRAIN_TOOLS_MKGRISU_BLOBDEPEND_HPP

#if defined(REDSTRAIN_BLOBFUL_BUILD) \
		&& defined(HAVE_REDSTRAIN_STACKTRACE_L10N_DEPENDENCY) \
		&& defined(HAVE_REDSTRAIN_ERROR_L10N_DEPENDENCY) \
		&& defined(HAVE_REDSTRAIN_CHARSETS_DEPENDENCY) \
		&& defined(HAVE_REDSTRAIN_IO_L10N_DEPENDENCY) \
		&& defined(HAVE_REDSTRAIN_PLATFORM_L10N_DEPENDENCY)
#define HAVE_ALL_BLOB_DEPENDENCIES
#endif

#include <redstrain/error/StdOStreamErrorHandler.hpp>

#ifdef REDSTRAIN_BLOBFUL_BUILD
#include <redstrain/locale/LocalizingConsoleErrorHandler.hpp>
#define APPROPRIATE_CONSOLE_ERROR_HANDLER (::areAllBlobDependenciesPresent() \
		? static_cast< ::redengine::error::ErrorHandler&>( \
		::redengine::locale::LocalizingConsoleErrorHandler::getDefaultStdErrorHandler()) \
		: static_cast< ::redengine::error::ErrorHandler&>( \
		::redengine::error::StdOStreamErrorHandler::defaultStdErrorHandler))
#else
#define APPROPRIATE_CONSOLE_ERROR_HANDLER \
		static_cast< ::redengine::error::ErrorHandler&>( \
		::redengine::error::StdOStreamErrorHandler::defaultStdErrorHandler)
#endif

bool areAllBlobDependenciesPresent();

#endif /* REDSTRAIN_TOOLS_MKGRISU_BLOBDEPEND_HPP */
