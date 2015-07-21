#ifndef REDSTRAIN_MOD_IO_INDENTINGOUTPUTSTREAMCONFIG_HPP
#define REDSTRAIN_MOD_IO_INDENTINGOUTPUTSTREAMCONFIG_HPP

#include <string>

#include "StreamConfig.hpp"

namespace redengine {
namespace io {

	enum IndentingOutputStreamAction {
		shift,
		unshift,
		indent
	};

	template<typename RecordT>
	class IndentingOutputStreamConfig {

	  public:
		typedef std::basic_string<RecordT> String;

	  private:
		static String getDefaultTabulation() {
			RecordT tabulation(static_cast<RecordT>('\t'));
			return String(&tabulation, static_cast<typename String::size_type>(1u));
		}

	  private:
		unsigned level;
		String tabulation;

	  public:
		IndentingOutputStreamConfig() : level(0u), tabulation(getDefaultTabulation()) {}

		IndentingOutputStreamConfig(const IndentingOutputStreamConfig& config)
				: level(config.level), tabulation(config.tabulation) {}

		inline unsigned getLevel() const {
			return level;
		}

		inline void shift() {
			++level;
		}

		inline void unshift() {
			if(level)
				--level;
		}

		inline const String& getTabulation() const {
			return tabulation;
		}

		void setTabulation(const String& tabulation) {
			this->tabulation = tabulation;
		}

	  public:
		class Tabulation : public StreamConfigurator<IndentingOutputStreamConfig<RecordT> > {

		  private:
			String tabulation;

		  public:
			Tabulation(const String& tabulation) : tabulation(tabulation) {}
			Tabulation(const Tabulation& tabulation) : tabulation(tabulation.tabulation) {}

			inline const String& getTabulation() {
				return tabulation;
			}

			virtual void reconfigStream(IndentingOutputStreamConfig<RecordT>& config) const {
				config.setTabulation(tabulation);
			}

		};

	};

	namespace config {

		template<typename RecordT>
		typename IndentingOutputStreamConfig<RecordT>::Tabulation tabulation(const std::basic_string<RecordT>&
				tabulation) {
			return typename IndentingOutputStreamConfig<RecordT>::Tabulation(tabulation);
		}

	}

}}

#endif /* REDSTRAIN_MOD_IO_INDENTINGOUTPUTSTREAMCONFIG_HPP */
