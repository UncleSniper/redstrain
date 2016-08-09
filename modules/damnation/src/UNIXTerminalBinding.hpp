#ifndef REDSTRAIN_MOD_DAMNATION_UNIXTERMINALBINDING_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXTERMINALBINDING_HPP

#include <redstrain/text/CodecManager.hpp>
#include <redstrain/redmond/environment.hpp>

#include "TermSpec.hpp"
#include "TerminalBinding.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API UNIXTerminalBinding : public TerminalBinding {

	  private:
		int readfd, writefd;
		const TermSpec& spec;
		text::CodecManager* codecManager;
		std::string encname;
		text::CodecFactory<text::Encoder16>* encoderFactory;
		text::CodecFactory<text::Decoder16>* decoderFactory;
		void* oldTermios;
		bool usingUTF8;

	  private:
		void setEncoderFactoryFromEncoding();
		void setDecoderFactoryFromEncoding();
		void catchSIGWINCH();
		void setRaw();
		void setCooked();

	  public:
		UNIXTerminalBinding(const TermSpec&);
		UNIXTerminalBinding(const UNIXTerminalBinding&);
		virtual ~UNIXTerminalBinding();

		inline int getReadFD() const {
			return readfd;
		}

		inline void setReadFD(int fd) {
			readfd = fd;
		}

		inline int getWriteFD() const {
			return writefd;
		}

		inline void setWriteFD(int fd) {
			writefd = fd;
		}

		inline const TermSpec& getTerminalInfo() const {
			return spec;
		}

		inline text::CodecManager* getCodecManager() const {
			return codecManager;
		}

		inline void setCodecManager(text::CodecManager* manager) {
			codecManager = manager;
		}

		inline const std::string& getEncoding() const {
			return encname;
		}

		void setEncoding(const std::string&);

		inline text::CodecFactory<text::Encoder16>& getEncoderFactory() const {
			return *encoderFactory;
		}

		void setEncoderFactory(text::CodecFactory<text::Encoder16>*);

		inline text::CodecFactory<text::Decoder16>& getDecoderFactory() const {
			return *decoderFactory;
		}

		void setDecoderFactory(text::CodecFactory<text::Decoder16>*);

		virtual InputMode getInputMode();
		virtual void setInputMode(InputMode);
		virtual bool hasSizeChanged();
		virtual Size getSize();

		virtual bool supportsOperation(OptionalOperation);
		virtual bool carriageReturn();
		virtual bool newLine();
		virtual bool clearScreen();
		virtual bool moveTo(unsigned, unsigned);
		virtual bool moveBy(int, int);
		virtual bool cursorDown();
		virtual bool cursorDownBy(unsigned);
		virtual bool cursorUp();
		virtual bool cursorUpBy(unsigned);
		virtual bool cursorLeft();
		virtual bool cursorLeftBy(unsigned);
		virtual bool cursorRight();
		virtual bool cursorRightBy(unsigned);

		virtual unsigned getColorCount();
		virtual unsigned setForegroundColor(unsigned);
		virtual unsigned setBackgroundColor(unsigned);
		virtual int setTextAttributes(int);
		virtual int addTextAttributes(int);
		virtual int removeTextAttributes(int);
		virtual void resetTextAttributes(int);

		virtual KeySym read(bool);
		virtual void write(char);
		virtual void write(const std::string&);
		virtual void write(text::Char16);
		virtual void write(const text::String16&);
		virtual void write(BoxSymbol);
		virtual void flush();

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXTERMINALBINDING_HPP */
