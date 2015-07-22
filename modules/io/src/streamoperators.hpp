#ifndef REDSTRAIN_MOD_IO_STREAMOPERATORS_HPP
#define REDSTRAIN_MOD_IO_STREAMOPERATORS_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/IntegerBits.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/util/IntegerTypeByTraits.hpp>

#include "FormattedInputStream.hpp"
#include "IndentingOutputStreamConfig.hpp"
#include "NumberPrintingOutputStreamConfig.hpp"
#include "ConfigurableFormattedOutputStream.hpp"

namespace redengine {
namespace io {

	// simple output actions

	enum SimpleFormattedOutputStreamAction {
		endln
	};

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream,
			SimpleFormattedOutputStreamAction action) {
		switch(action) {
			case endln:
				stream.endLine();
				break;
			default:
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			SimpleFormattedOutputStreamAction action) {
		switch(action) {
			case endln:
				stream.endLine();
				break;
			default:
				break;
		}
		return stream;
	}

	// stream configurators

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigTemplateT,
		typename RequestedConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigTemplateT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigTemplateT>& stream,
			const StreamConfigurator<RequestedConfigT>& configurator) {
		configurator.reconfigStream(stream.template getConfiguration<RequestedConfigT>());
		return stream;
	}

	// std::string output

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, const std::basic_string<RecordT>& data) {
		stream.write(data.data(), static_cast<size_t>(data.length()));
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream,
			const std::basic_string<RecordT>& data) {
		stream.print(data);
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			const std::basic_string<RecordT>& data) {
		stream.print(data);
		return stream;
	}

	// raw string output

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, const RecordT* data) {
		stream.print(data);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, const RecordT* data) {
		stream.print(data);
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			const RecordT* data) {
		stream.print(data);
		return stream;
	}

	// line input

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedInputStream<RecordT, LinebreakRecordsT>&
	operator>>(FormattedInputStream<RecordT, LinebreakRecordsT>& stream,
			std::basic_string<RecordT>& variable) {
		variable.clear();
		stream.readLine(variable);
		return stream;
	}

	// number formatting

	template<typename RecordT, typename IntegerT, unsigned Base>
	void printInteger(OutputStream<RecordT>& stream, IntegerT value, unsigned width = 0u,
			RecordT padRecord = RecordT('0')) {
		const unsigned maxDigits = static_cast<unsigned>(util::IntegerLog<
			IntegerT,
			static_cast<IntegerT>(Base),
			util::IntegerBounds<IntegerT>::MAX
		>::EXPONENT) + 1u;
		RecordT buffer[maxDigits];
		bool negative = util::IntegerBits<IntegerT>::isNegative(value);
		if(width > maxDigits) {
			unsigned preCount = width - maxDigits;
			unsigned firstCount = preCount % maxDigits;
			if(!firstCount)
				firstCount = maxDigits;
			unsigned zeroes;
			for(zeroes = 1u; zeroes < firstCount; ++zeroes)
				buffer[maxDigits - zeroes] = padRecord;
			buffer[maxDigits - zeroes] = negative ? RecordT('-') : padRecord;
			stream.write(buffer + (maxDigits - zeroes), static_cast<size_t>(zeroes));
			preCount -= firstCount;
			if(preCount) {
				for(; zeroes <= maxDigits; ++zeroes)
					buffer[maxDigits - zeroes] = padRecord;
				for(; preCount; preCount -= maxDigits)
					stream.write(buffer, static_cast<size_t>(maxDigits));
			}
		}
		if(negative)
			value = -value;
		unsigned fill = 0u;
		for(; value; value /= static_cast<IntegerT>(Base))
			buffer[maxDigits - ++fill]
					= RecordT("0123456789ABCDEF"[value % static_cast<IntegerT>(Base)]);
		if(!fill)
			buffer[maxDigits - ++fill] = RecordT('0');
		if(width <= maxDigits) {
			if(width > fill) {
				while(width > fill)
					buffer[maxDigits - ++fill] = padRecord;
				if(negative)
					buffer[maxDigits - fill] = RecordT('-');
			}
			else {
				if(negative)
					buffer[maxDigits - ++fill] = RecordT('-');
			}
		}
		stream.write(buffer + (maxDigits - fill), static_cast<size_t>(fill));
	}

	// primitive integral output for OutputStream

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, char c) {
		RecordT rec(c);
		stream.write(&rec, static_cast<size_t>(1u));
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, short value) {
		printInteger<RecordT, short, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, unsigned short value) {
		printInteger<RecordT, unsigned short, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, int value) {
		printInteger<RecordT, int, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, unsigned int value) {
		printInteger<RecordT, unsigned int, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, long value) {
		printInteger<RecordT, long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, unsigned long value) {
		printInteger<RecordT, unsigned long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, long long value) {
		printInteger<RecordT, long long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, unsigned long long value) {
		printInteger<RecordT, unsigned long long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename ObjectT>
	OutputStream<RecordT>& operator<<(OutputStream<RecordT>& stream, const ObjectT* value) {
		typedef typename util::IntegerTypeByTraits<sizeof(ObjectT*), false>::StandardType IntPointer;
		printInteger<RecordT, IntPointer, 16u>(stream, reinterpret_cast<IntPointer>(value));
		return stream;
	}

	// primitive integral output for FormattedOutputStream

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, char c) {
		RecordT rec(c);
		stream.write(&rec, static_cast<size_t>(1u));
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, short value) {
		printInteger<RecordT, short, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, unsigned short value) {
		printInteger<RecordT, unsigned short, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, int value) {
		printInteger<RecordT, int, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, unsigned int value) {
		printInteger<RecordT, unsigned int, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, long value) {
		printInteger<RecordT, long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, unsigned long value) {
		printInteger<RecordT, unsigned long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, long long value) {
		printInteger<RecordT, long long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, unsigned long long value) {
		printInteger<RecordT, unsigned long long, 10u>(stream, value);
		return stream;
	}

	template<typename RecordT, typename LinebreakRecordsT, typename ObjectT>
	FormattedOutputStream<RecordT, LinebreakRecordsT>&
	operator<<(FormattedOutputStream<RecordT, LinebreakRecordsT>& stream, const ObjectT* value) {
		stream.print(RecordT('0'));
		stream.print(RecordT('x'));
		typedef typename util::IntegerTypeByTraits<sizeof(ObjectT*), false>::StandardType IntPointer;
		printInteger<RecordT, IntPointer, 16u>(stream, reinterpret_cast<IntPointer>(value));
		return stream;
	}

	// primitive integral output for ConfigurableFormattedOutputStream

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream, char c) {
		RecordT rec(c);
		stream.write(&rec, static_cast<size_t>(1u));
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream, short value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, short, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, short, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, short, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, short, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			unsigned short value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, unsigned short, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, unsigned short, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, unsigned short, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, unsigned short, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream, int value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, int, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, int, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, int, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, int, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			unsigned int value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, unsigned int, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, unsigned int, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, unsigned int, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, unsigned int, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream, long value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, long, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, long, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, long, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, long, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			unsigned long value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, unsigned long, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, unsigned long, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, unsigned long, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, unsigned long, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			long long value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, long long, 2u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, long long, 8u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, long long, 16u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, long long, 10u>(stream, value, config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			unsigned long long value) {
		const NumberPrintingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<NumberPrintingOutputStreamConfig<RecordT> >();
		switch(config.getBase()) {
			case BINARY:
				printInteger<RecordT, unsigned long long, 2u>(stream, value,
						config.getWidth(), config.getPadRecord());
				break;
			case OCTAL:
				printInteger<RecordT, unsigned long long, 8u>(stream, value,
						config.getWidth(), config.getPadRecord());
				break;
			case HEXADECIMAL:
				printInteger<RecordT, unsigned long long, 16u>(stream, value,
						config.getWidth(), config.getPadRecord());
				break;
			default:
				printInteger<RecordT, unsigned long long, 10u>(stream, value,
						config.getWidth(), config.getPadRecord());
				break;
		}
		return stream;
	}

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigT,
		typename ObjectT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigT>& stream,
			const ObjectT* value) {
		stream.print(RecordT('0'));
		stream.print(RecordT('x'));
		typedef typename util::IntegerTypeByTraits<sizeof(ObjectT*), false>::StandardType IntPointer;
		printInteger<RecordT, IntPointer, 16u>(stream, reinterpret_cast<IntPointer>(value));
		return stream;
	}

	// indentation for ConfigurableFormattedOutputStream

	template<
		typename RecordT,
		typename LinebreakRecordsT,
		template<StreamConfigSpecialization, typename> class ConfigTemplateT
	>
	ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigTemplateT>&
	operator<<(ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT, ConfigTemplateT>& stream,
			IndentingOutputStreamAction action) {
		IndentingOutputStreamConfig<RecordT>& config
				= stream.template getConfiguration<IndentingOutputStreamConfig<RecordT> >();
		switch(action) {
			case shift:
				config.shift();
				break;
			case unshift:
				config.unshift();
				break;
			case indent:
				{
					const std::basic_string<RecordT>& tabulation = config.getTabulation();
					unsigned level = config.getLevel();
					for(; level; --level)
						stream.print(tabulation);
				}
				break;
			default:
				break;
		}
		return stream;
	}

}}

#endif /* REDSTRAIN_MOD_IO_STREAMOPERATORS_HPP */
