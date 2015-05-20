#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "CPPArrayOutputStream.hpp"
#include "StreamArrayClosedError.hpp"

using std::string;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace io {

	const char *const CPPArrayOutputStream::DEFAULT_VARIABLE_NAME = "data";

	static const char *const HEX_DIGITS = "0123456789ABCDEF";

	CPPArrayOutputStream::BeginningAppender::BeginningAppender(FormattedOutputStream<char>& output)
			: output(output), pristine(true) {}

	CPPArrayOutputStream::BeginningAppender::BeginningAppender(const BeginningAppender& appender)
			: Appender(appender), output(appender.output), lastPart(appender.lastPart),
			pristine(appender.pristine) {}

	const string& CPPArrayOutputStream::BeginningAppender::getVariableSimpleName() const {
		return lastPart;
	}

	void CPPArrayOutputStream::BeginningAppender::append(const string& part) {
		if(part.empty())
			return;
		if(!lastPart.empty()) {
			output.print("namespace ");
			output.print(lastPart);
			output.println(" {");
			pristine = false;
		}
		lastPart = part;
	}

	void CPPArrayOutputStream::BeginningAppender::doneAppending() {
		if(!pristine)
			output.endLine();
	}

	CPPArrayOutputStream::EndingAppender::EndingAppender(FormattedOutputStream<char>& output)
			: output(output), state(PRISTINE) {}

	CPPArrayOutputStream::EndingAppender::EndingAppender(const EndingAppender& appender)
			: Appender(appender), output(appender.output), state(appender.state) {}

	void CPPArrayOutputStream::EndingAppender::append(const string& part) {
		if(part.empty())
			return;
		switch(state) {
			case PRISTINE:
				state = CACHED_ONLY;
				break;
			case CACHED_ONLY:
				output.endLine();
				state = PRINTED;
			case PRINTED:
				output.print("}");
				break;
		}
	}

	void CPPArrayOutputStream::EndingAppender::doneAppending() {
		if(state == PRINTED)
			output.endLine();
	}

	CPPArrayOutputStream::CPPArrayOutputStream(OutputStream<char>& stream, const string& variable,
			LineOriented::LinebreakPolicy linebreaks) : formatted(stream, linebreaks),
			variable(variable.empty() ? DEFAULT_VARIABLE_NAME : variable), state(EMPTY), needsIndent(false),
			columns(0u) {}

	CPPArrayOutputStream::CPPArrayOutputStream(const CPPArrayOutputStream& stream)
			: Stream(stream), OutputStream<char>(stream),
			formatted(const_cast<OutputStream<char>&>(stream.formatted.getBackingOutputStream()),
			stream.formatted.getLinebreakPolicy()), variable(stream.variable), state(stream.state),
			needsIndent(stream.needsIndent), columns(stream.columns) {}

	OutputStream<char>& CPPArrayOutputStream::getBackingOutputStream() {
		return formatted.getBackingOutputStream();
	}

	const OutputStream<char>& CPPArrayOutputStream::getBackingOutputStream() const {
		return formatted.getBackingOutputStream();
	}

	void CPPArrayOutputStream::writeBlock(const char* buffer, size_t count) {
		if(!count)
			return;
		switch(state) {
			case EMPTY:
				beginArray();
				state = FILLING;
				break;
			case FILLING:
				break;
			case ENDED:
				throw StreamArrayClosedError();
			default:
				throw ProgrammingError("Unrecognized stream state: "
						+ StringUtils::toString(static_cast<int>(state)));
		}
		char block[] = "'\\x..'";
		for(; count; ++buffer, --count) {
			if(columns == 8u) {
				formatted.println(",");
				formatted.print("\t\t" + !needsIndent);
				columns = 0u;
			}
			else if(columns)
				formatted.print(", ");
			else
				formatted.print("\t\t" + !needsIndent);
			unsigned code = static_cast<unsigned>(static_cast<unsigned char>(*buffer));
			block[3] = HEX_DIGITS[code / 16u];
			block[4] = HEX_DIGITS[code % 16u];
			formatted.write(block, sizeof(block) - static_cast<size_t>(1u));
			++columns;
		}
	}

	void CPPArrayOutputStream::beginArray() {
		BeginningAppender beginner(formatted);
		StringUtils::split(variable, "::", beginner);
		needsIndent = !beginner.isPristine();
		formatted.print("\tconst char " + !needsIndent);
		formatted.print(beginner.getVariableSimpleName());
		formatted.println("[] = {");
	}

	void CPPArrayOutputStream::endArray() {
		if(state == FILLING) {
			formatted.endLine();
			formatted.println("\t};" + !needsIndent);
			EndingAppender ender(formatted);
			StringUtils::split(variable, "::", ender);
		}
		state = ENDED;
	}

	void CPPArrayOutputStream::close() {
		endArray();
		formatted.close();
	}

	size_t CPPArrayOutputStream::tell() const {
		return formatted.tell();
	}

}}
