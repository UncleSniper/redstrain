#ifndef REDSTRAIN_MOD_DAMNATION_TK_WIDGETFOREIGNTOLAYERERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_WIDGETFOREIGNTOLAYERERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API WidgetForeignToLayerError : public DamnError, public error::IllegalArgumentError {

	  public:
		WidgetForeignToLayerError();
		WidgetForeignToLayerError(const WidgetForeignToLayerError&);

		REDSTRAIN_DECLARE_ERROR(WidgetForeignToLayerError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_WIDGETFOREIGNTOLAYERERROR_HPP */
