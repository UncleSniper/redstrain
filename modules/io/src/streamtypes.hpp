#ifndef REDSTRAIN_MOD_IO_STREAMTYPES_HPP
#define REDSTRAIN_MOD_IO_STREAMTYPES_HPP

#include "IndentingOutputStreamConfig.hpp"
#include "NumberPrintingOutputStreamConfig.hpp"
#include "ConfigurableFormattedOutputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	struct DefaultConfiguredOutputStream {

		typedef StreamConfig2<
			NumberPrintingOutputStreamConfig<RecordT>,
			IndentingOutputStreamConfig<RecordT>
		> Configuration;

		typedef ConfigurableFormattedOutputStream<
			RecordT,
			DefaultLinebreakRecords<RecordT>,
			Configuration::template ConfigTemplate
		> Stream;

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMTYPES_HPP */
