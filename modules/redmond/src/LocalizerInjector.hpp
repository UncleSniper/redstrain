#ifndef REDSTRAIN_MOD_REDMOND_LOCALIZERINJECTOR_HPP
#define REDSTRAIN_MOD_REDMOND_LOCALIZERINJECTOR_HPP

namespace redengine {
namespace redmond {

	template<typename LocalizerFunctionT>
	class LocalizerInjector {

	  public:
		typedef LocalizerFunctionT LocalizerFunction;

	  private:
		const LocalizerFunctionT localizer;

	  public:
		LocalizerInjector(LocalizerFunctionT& hook, LocalizerFunctionT localizer) : localizer(localizer) {
			hook = localizer;
		}

		LocalizerInjector(const LocalizerInjector& injector) : localizer(injector.localizer) {}

		inline LocalizerFunctionT getLocalizerFunction() const {
			return localizer;
		}

	};

}}

#endif /* REDSTRAIN_MOD_REDMOND_LOCALIZERINJECTOR_HPP */
