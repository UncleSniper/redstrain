#ifndef REDSTRAIN_MOD_DAMNATION_TPARM_HPP
#define REDSTRAIN_MOD_DAMNATION_TPARM_HPP

#include <string>
#include <stdint.h>

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API TParameter {

	  public:
		enum Type {
			INTEGER,
			FOREIGN_STRING,
			PRIVATE_STRING
		};

		enum Allocation {
			NEEDS_FREEING,
			MUST_NOT_FREE
		};

		enum Ordering {
			LESS,
			EQUAL,
			GREATER
		};

	  private:
		union Value {
			int32_t integer;
			const char* string;
		};

	  private:
		Type type;
		Value value;

	  public:
		TParameter();
		TParameter(int32_t);
		TParameter(const char*, Allocation);
		TParameter(const char*);
		TParameter(const std::string&);
		TParameter(const TParameter&);
		~TParameter();

		inline Type getType() const {
			return type;
		}

		Allocation getAllocation() const;
		int32_t intValue() const;
		const char* stringValue() const;

		TParameter& operator=(const TParameter&);
		Ordering compare(const TParameter&) const;
		bool truth() const;
		void increment();

	};

	REDSTRAIN_DAMNATION_API void termParamArray(std::string&, const char*, const TParameter *const*, unsigned);

	REDSTRAIN_DAMNATION_API void termParamSink(std::string&, const char*);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API void termParamSink(
		std::string&, const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(const char*);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

	REDSTRAIN_DAMNATION_API std::string termParam(
		const char*,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&,
		const TParameter&
	);

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TPARM_HPP */
