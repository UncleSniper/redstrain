#ifndef REDSTRAIN_MOD_IO_NUMBERPRINTINGOUTPUTSTREAMCONFIG_HPP
#define REDSTRAIN_MOD_IO_NUMBERPRINTINGOUTPUTSTREAMCONFIG_HPP

#include "StreamConfig.hpp"

namespace redengine {
namespace io {

	enum NumberBase {
		BINARY,
		OCTAL,
		DECIMAL,
		HEXADECIMAL
	};

	template<typename RecordT>
	class NumberPrintingOutputStreamConfig {

	  private:
		NumberBase base;
		unsigned width;
		RecordT padRecord;

	  public:
		NumberPrintingOutputStreamConfig()
				: base(DECIMAL), width(0u), padRecord('0') {}
		NumberPrintingOutputStreamConfig(const NumberPrintingOutputStreamConfig& config)
				: base(config.base), width(config.width), padRecord(config.padRecord) {}

		inline NumberBase getBase() const {
			return base;
		}

		inline void setBase(NumberBase newBase) {
			base = newBase;
		}

		inline unsigned getWidth() const {
			return width;
		}

		inline void setWidth(unsigned newWidth) {
			width = newWidth;
		}

		inline const RecordT& getPadRecord() const {
			return padRecord;
		}

		inline void setPadRecord(const RecordT& newPadRecord) {
			padRecord = newPadRecord;
		}

	  public:
		class Base : public StreamConfigurator<NumberPrintingOutputStreamConfig<RecordT> > {

		  private:
			NumberBase base;

		  public:
			Base(NumberBase base) : base(base) {}
			Base(const Base& base) : base(base.base) {}

			inline NumberBase getBase() const {
				return base;
			}

			virtual void reconfigStream(NumberPrintingOutputStreamConfig<RecordT>& config) const {
				config.setBase(base);
			}

		};

		class Padding : public StreamConfigurator<NumberPrintingOutputStreamConfig<RecordT> > {

		  private:
			unsigned width;
			RecordT padRecord;

		  public:
			Padding(unsigned width, const RecordT& padRecord) : width(width), padRecord(padRecord) {}
			Padding(const Padding& padding) : width(padding.width), padRecord(padding.padRecord) {}

			inline unsigned getWidth() const {
				return width;
			}

			inline const RecordT& getPadRecord() const {
				return padRecord;
			}

			virtual void reconfigStream(NumberPrintingOutputStreamConfig<RecordT>& config) const {
				config.setWidth(width);
				config.setPadRecord(padRecord);
			}

		};

	};

	namespace config {

		template<typename RecordT>
		typename NumberPrintingOutputStreamConfig<RecordT>::Base base(NumberBase base) {
			return typename NumberPrintingOutputStreamConfig<RecordT>::Base(base);
		}

		template<typename RecordT>
		typename NumberPrintingOutputStreamConfig<RecordT>::Padding
		pad(unsigned width = 0u, const RecordT& padRecord = RecordT('0')) {
			return typename NumberPrintingOutputStreamConfig<RecordT>::Padding(width, padRecord);
		}

	}

}}

#endif /* REDSTRAIN_MOD_IO_NUMBERPRINTINGOUTPUTSTREAMCONFIG_HPP */
