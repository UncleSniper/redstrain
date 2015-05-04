#ifndef REDSTRAIN_MOD_IO_CONFIGURABLEFORMATTEDOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_CONFIGURABLEFORMATTEDOUTPUTSTREAM_HPP

#include "StreamConfig.hpp"
#include "FormattedOutputStream.hpp"

namespace redengine {
namespace io {

	template<
		typename RecordT,
		typename LinebreakRecordsT = DefaultLinebreakRecords<RecordT>,
		template<StreamConfigSpecialization, typename> class ConfigT = StreamConfig0::ConfigTemplate
	>
	class ConfigurableFormattedOutputStream : public FormattedOutputStream<RecordT, LinebreakRecordsT> {

	  public:
		typedef typename ConfigT<COMPOUND_CONFIG, void>::Config Config;

	  private:
		Config config;

	  protected:
		ConfigurableFormattedOutputStream(const ConfigurableFormattedOutputStream& stream)
				: Stream(stream), FormattedOutputStream<RecordT, LinebreakRecordsT>(stream),
				config(stream.config) {}

	  public:
		ConfigurableFormattedOutputStream(OutputStream<RecordT>& output,
				LineOriented::LinebreakPolicy linebreaks = LineOriented::AUTO_LINEBREAKS)
				: FormattedOutputStream<RecordT, LinebreakRecordsT>(output, linebreaks) {}

		template<typename ConfigComponentT>
		ConfigComponentT& getConfiguration() {
			return ConfigT<CONFIG_COMPONENT, ConfigComponentT>::getConfig(config);
		}

		template<typename ConfigComponentT>
		const ConfigComponentT& getConfiguration() const {
			return ConfigT<CONFIG_COMPONENT, ConfigComponentT>::getConfig(config);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONFIGURABLEFORMATTEDOUTPUTSTREAM_HPP */
