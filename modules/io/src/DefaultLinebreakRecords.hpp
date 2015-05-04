#ifndef REDSTRAIN_MOD_IO_DEFAULTLINEBREAKRECORDS_HPP
#define REDSTRAIN_MOD_IO_DEFAULTLINEBREAKRECORDS_HPP

namespace redengine {
namespace io {

	template<typename RecordT>
	class DefaultLinebreakRecords {

	  public:
		static const RecordT NEWLINE = static_cast<RecordT>('\n');
		static const RecordT CARRIAGE_RETURN = static_cast<RecordT>('\r');

	};

}}

#endif /* REDSTRAIN_MOD_IO_DEFAULTLINEBREAKRECORDS_HPP */
