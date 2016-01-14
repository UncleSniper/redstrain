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

		class REDSTRAIN_IO_API SizeAppender : public util::Appender<std::string> {

		  private:
			FormattedOutputStream<char>& output;
			util::FileSize size;
			std::string lastPart;
			unsigned partCount;
			const std::string& exportMacro;

		  public:
			SizeAppender(FormattedOutputStream<char>&, util::FileSize, const std::string&);
			SizeAppender(const SizeAppender&);

			virtual void append(const std::string&);
			virtual void doneAppending();

		};

		class REDSTRAIN_IO_API BlobAppender : public util::Appender<std::string> {

		  private:
			FormattedOutputStream<char>& output;
			const std::string& blobPath;
			std::string lastPart;
			unsigned partCount;

		  public:
			BlobAppender(FormattedOutputStream<char>&, const std::string&);
			BlobAppender(const BlobAppender&);

			virtual void append(const std::string&);
			virtual void doneAppending();

		};

	  private:
		FormattedOutputStream<char> formatted;
		std::string variable;
		State state;
		bool needsIndent;
		unsigned columns;
		util::FileSize arraySize;
		std::string exportMacro, extraInclude, blobPath, guardMacro;

	  public:
		static const char *const DEFAULT_VARIABLE_NAME;

	  private:
		void beginArray();
		void putIncludes(bool);

	  protected:
		CPPArrayOutputStream(const CPPArrayOutputStream&);

		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		CPPArrayOutputStream(OutputStream<char>&, const std::string& = DEFAULT_VARIABLE_NAME,
				LineOriented::LinebreakPolicy = LineOriented::AUTO_LINEBREAKS);

		inline const std::string& getVariableName() const {
			return variable;
		}

		inline const std::string& getExportMacro() const {
			return exportMacro;
		}

		inline const std::string& getExtraInclude() const {
			return extraInclude;
		}

		inline const std::string& getBlobPath() const {
			return blobPath;
		}

		inline const std::string& getGuardMacro() const {
			return guardMacro;
		}

		OutputStream<char>& getBackingOutputStream();
		const OutputStream<char>& getBackingOutputStream() const;
		void setVariableName(const std::string&);
		void setExportMacro(const std::string&);
		void setExtraInclude(const std::string&);
		void setBlobPath(const std::string&);
		void setGuardMacro(const std::string&);
		void endArray();
		void writeHeader();

		virtual void close();
		virtual util::FileSize tell() const;

	};

}}

#endif /* REDSTRAIN_MOD_IO_CPPARRAYOUTPUTSTREAM_HPP */
