#ifndef REDSTRAIN_MOD_IO_CPPARRAYOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_CPPARRAYOUTPUTSTREAM_HPP

#include <redstrain/util/Appender.hpp>

#include "FormattedOutputStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API CPPArrayOutputStream : public OutputStream<char> {

	  private:
		enum State {
			EMPTY,
			FILLING,
			ENDED
		};

		class REDSTRAIN_IO_API BeginningAppender : public util::Appender<std::string> {

		  private:
			FormattedOutputStream<char>& output;
			std::string lastPart;
			bool pristine;

		  public:
			BeginningAppender(FormattedOutputStream<char>&);
			BeginningAppender(const BeginningAppender&);

			inline bool isPristine() const {
				return pristine;
			}

			const std::string& getVariableSimpleName() const;

			virtual void append(const std::string&);
			virtual void doneAppending();

		};

		class REDSTRAIN_IO_API EndingAppender : public util::Appender<std::string> {

		  private:
			enum State {
				PRISTINE,
				CACHED_ONLY,
				PRINTED
			};

		  private:
			FormattedOutputStream<char>& output;
			State state;

		  public:
			EndingAppender(FormattedOutputStream<char>&);
			EndingAppender(const EndingAppender&);

			virtual void append(const std::string&);
			virtual void doneAppending();

		};

	  private:
		FormattedOutputStream<char> formatted;
		std::string variable;
		State state;
		bool needsIndent;
		unsigned columns;

	  public:
		static const char *const DEFAULT_VARIABLE_NAME;

	  private:
		void beginArray();

	  protected:
		CPPArrayOutputStream(const CPPArrayOutputStream&);

		virtual void writeBlock(const char*, size_t);

	  public:
		CPPArrayOutputStream(OutputStream<char>&, const std::string& = DEFAULT_VARIABLE_NAME,
				LineOriented::LinebreakPolicy = LineOriented::AUTO_LINEBREAKS);

		inline const std::string& getVariableName() const {
			return variable;
		}

		OutputStream<char>& getBackingOutputStream();
		const OutputStream<char>& getBackingOutputStream() const;
		void endArray();

		virtual void close();
		virtual size_t tell() const;

	};

}}

#endif /* REDSTRAIN_MOD_IO_CPPARRAYOUTPUTSTREAM_HPP */
