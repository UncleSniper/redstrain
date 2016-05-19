#ifndef REDSTRAIN_MOD_LOG_OUTPUTSTREAMLOGGER_HPP
#define REDSTRAIN_MOD_LOG_OUTPUTSTREAMLOGGER_HPP

#include <redstrain/text/Formatter.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/text/DefaultFormattingOptionStringEmitter.hpp>

#include "Logger.hpp"
#include "MessageMemberFormatter.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class OutputStreamLogger : public Logger<SeverityT, ComponentT, UnitT, ConcernT> {

	  private:
		typedef Logger<SeverityT, ComponentT, UnitT, ConcernT> Base;

	  public:
		typedef typename Base::Message Message;

	  public:
		static const int FL_MANAGE_STREAM              = 001;
		static const int FL_MANAGE_SEVERITY_FORMATTER  = 002;
		static const int FL_MANAGE_COMPONENT_FORMATTER = 004;
		static const int FL_MANAGE_UNIT_FORMATTER      = 010;
		static const int FL_MANAGE_CONCERN_FORMATTER   = 020;
		static const int FL_MANAGE_ALL_FORMATTERS      = 036;
		static const int FL_MANAGE_EVERYTHING          = 037;
		static const int FL_MASK                       = 037;

	  private:
		io::OutputStream<text::Char16>& stream;
		io::FormattedOutputStream<text::Char16> formatted;
		MessageMemberFormatter<SeverityT>& severityFormatter;
		MessageMemberFormatter<ComponentT>& componentFormatter;
		MessageMemberFormatter<UnitT>& unitFormatter;
		MessageMemberFormatter<ConcernT>& concernFormatter;
		text::String16 formatString;
		text::Formatter<text::Char16> formatter;
		int flags;

	  public:
		OutputStreamLogger(io::OutputStream<text::Char16>& stream,
				MessageMemberFormatter<SeverityT>& severityFormatter,
				MessageMemberFormatter<ComponentT>& componentFormatter,
				MessageMemberFormatter<UnitT>& unitFormatter, MessageMemberFormatter<ConcernT>& concernFormatter,
				int flags) : stream(stream), formatted(stream), severityFormatter(severityFormatter),
				componentFormatter(componentFormatter), unitFormatter(unitFormatter),
				concernFormatter(concernFormatter), formatString(text::Transcode::utf8ToBMP("[%s] {%s - %s} %s: %s")),
				formatter(text::FormattingOptions<text::Char16>(
				text::DefaultFormattingOptionStringEmitter<text::Char16>::instance)),
				flags(flags & OutputStreamLogger::FL_MASK) {}

		OutputStreamLogger(const OutputStreamLogger& logger) : Logger<SeverityT, ComponentT, UnitT, ConcernT>(logger),
				stream(logger.stream), formatted(stream), severityFormatter(logger.severityFormatter),
				componentFormatter(logger.componentFormatter), unitFormatter(logger.unitFormatter),
				concernFormatter(logger.concernFormatter), formatString(logger.formatString),
				formatter(logger.formatter), flags(logger.flags & ~OutputStreamLogger::FL_MANAGE_EVERYTHING) {}

		virtual ~OutputStreamLogger() {
			if(flags & OutputStreamLogger::FL_MANAGE_STREAM)
				delete &stream;
			if(flags & OutputStreamLogger::FL_MANAGE_SEVERITY_FORMATTER)
				delete &severityFormatter;
			if(flags & OutputStreamLogger::FL_MANAGE_COMPONENT_FORMATTER)
				delete &componentFormatter;
			if(flags & OutputStreamLogger::FL_MANAGE_UNIT_FORMATTER)
				delete &unitFormatter;
			if(flags & OutputStreamLogger::FL_MANAGE_CONCERN_FORMATTER)
				delete &concernFormatter;
		}

		inline io::OutputStream<text::Char16>& getStream() {
			return stream;
		}

		inline const io::OutputStream<text::Char16>& getStream() const {
			return stream;
		}

		inline MessageMemberFormatter<SeverityT>& getSeverityFormatter() {
			return severityFormatter;
		}

		inline const MessageMemberFormatter<SeverityT>& getSeverityFormatter() const {
			return severityFormatter;
		}

		inline MessageMemberFormatter<ComponentT>& getComponentFormatter() {
			return componentFormatter;
		}

		inline const MessageMemberFormatter<ComponentT>& getComponentFormatter() const {
			return componentFormatter;
		}

		inline MessageMemberFormatter<UnitT>& getUnitFormatter() {
			return unitFormatter;
		}

		inline const MessageMemberFormatter<UnitT>& getUnitFormatter() const {
			return unitFormatter;
		}

		inline MessageMemberFormatter<ConcernT>& getConcernFormatter() {
			return concernFormatter;
		}

		inline const MessageMemberFormatter<ConcernT>& getConcernFormatter() const {
			return concernFormatter;
		}

		inline const text::String16& getFormatString() const {
			return formatString;
		}

		void setFormatString(const text::String16& formatString) {
			this->formatString = formatString;
		}

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int flags) {
			this->flags = flags & OutputStreamLogger::FL_MASK;
		}

		inline void addFlags(int mask) {
			flags |= mask & OutputStreamLogger::FL_MASK;
		}

		inline void removeFlags(int mask) {
			flags &= ~mask;
		}

		virtual void log(const Message& message) {
			formatted.println(formatter.format(
				formatString,
				severityFormatter.formatMember(message.severity),
				componentFormatter.formatMember(message.component),
				unitFormatter.formatMember(message.unit),
				concernFormatter.formatMember(message.concern),
				message.message
			));
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_OUTPUTSTREAMLOGGER_HPP */
