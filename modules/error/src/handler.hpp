#ifndef REDSTRAIN_MOD_ERROR_HANDLER_HPP
#define REDSTRAIN_MOD_ERROR_HANDLER_HPP

#include <new>
#include <cstddef>

namespace redengine {
namespace error {

	template<typename HiddenErrorT, typename ExposedErrorT>
	void wrapError(const HiddenErrorT& error) {
		HiddenErrorT* hidden = NULL;
		try {
			hidden = error.template cloneAs<HiddenErrorT>();
		}
		catch(const std::bad_alloc&) {}
		throw ExposedErrorT(hidden);
	}

	template<typename HiddenErrorT, typename ExposedErrorT, typename PrefixArgumentT>
	void wrapErrorWithPrefix(const HiddenErrorT& error, const PrefixArgumentT& prefixArgument) {
		HiddenErrorT* hidden = NULL;
		try {
			hidden = error.template cloneAs<HiddenErrorT>();
		}
		catch(const std::bad_alloc&) {}
		throw ExposedErrorT(prefixArgument, hidden);
	}

	template<typename ReturnT, typename FunctionT, typename HiddenErrorT, typename ExposedErrorT>
	ReturnT catching(const FunctionT& body) {
		try {
			return body();
		}
		catch(const HiddenErrorT& error) {
			wrapError<HiddenErrorT, ExposedErrorT>(error);
		}
	}

}}

#endif /* REDSTRAIN_MOD_ERROR_HANDLER_HPP */
