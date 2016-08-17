#ifndef REDSTRAIN_MOD_DAMNATION_UNIXTERMINALBINDING_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXTERMINALBINDING_HPP

#include <redstrain/text/CodecManager.hpp>
#include <redstrain/util/StringBuilder.hpp>
#include <redstrain/util/types.hpp>
#include <redstrain/redmond/environment.hpp>

#include "TermSpec.hpp"
#include "TerminalBinding.hpp"
#include "unixutils/BinaryToKeySymConverter.hpp"
#include "unixutils/SequenceMapInputConverter.hpp"

namespace redengine {
namespace damnation {

	class ColorMap;

	class REDSTRAIN_DAMNATION_API UNIXTerminalBinding : public TerminalBinding {

	  public:
		enum ColorMode {
			CM_MONOCHROME,
			CM_LOW_COLOR,
			CM_HIGH_COLOR
		};

		enum WriteMode {
			WM_DIRECT,
			WM_BLOCKED,
			WM_BUFFERED
		};

	  private:
		int readfd, writefd;
		const TermSpec& spec;
		text::CodecManager* codecManager;
		std::string encname;
		text::CodecFactory<text::Encoder16>* encoderFactory;
		text::CodecFactory<text::Decoder16>* decoderFactory;
		void* oldTermios;
		bool usingUTF8;
		unsigned prevWidth, prevHeight;
		const ColorMode colorMode;
		const ColorMap* colorMap;
		int currentAttributes;
		WriteMode writeMode;
		char* writeBlock;
		util::MemorySize blockFill;
		util::StringBuilder<char> writeBuffer;
		bool inCAMode;
		unixutils::BinaryToKeySymConverter inputConverter;
		unixutils::InputConverter* decoderInputConverter;
		unixutils::SequenceMapInputConverter::Map specialKeyMap;
		unixutils::SequenceMapInputConverter specialKeyConverter;
		bool currentlyBlocking;
		uint64_t specialKeyFlushDelay;

	  private:
		void setEncoderFactoryFromEncoding();
		void setDecoderFactoryFromEncoding();
		void catchSIGWINCH();
		void setRaw();
		void setCooked();
		void wrappedWrite(const char*, util::MemorySize);
		void wrappedWrite(const std::string&);
		void doWrite(const char*, util::MemorySize);
		unsigned setColor(unsigned, unsigned);
		void enterUnderline();
		void leaveUnderline();
		void registerSpecialKeys();
		void registerSpecialKey(const char*, KeySym::Type, KeySym::Modifier, unsigned);
		bool hasSpecialKey(const char*) const;

		static ColorMode colorModeFromColorCount(uint32_t);

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

		inline ColorMode getColorMode() const {
			return colorMode;
		}

		inline const ColorMap* getColorMap() const {
			return colorMap;
		}

		inline void setColorMap(const ColorMap* colorMap) {
			this->colorMap = colorMap;
		}

		inline WriteMode getWriteMode() const {
			return writeMode;
		}

		void setWriteMode(WriteMode);

		inline unixutils::BinaryToKeySymConverter& getInputConverter() {
			return inputConverter;
		}

		inline const unixutils::BinaryToKeySymConverter& getInputConverter() const {
			return inputConverter;
		}

		inline unixutils::SequenceMapInputConverter::Map& getSpecialKeyMap() {
			return specialKeyMap;
		}

		inline const unixutils::SequenceMapInputConverter::Map& getSpecialKeyMap() const {
			return specialKeyMap;
		}

		inline uint64_t getSpecialKeyFlushDelay() const {
			return specialKeyFlushDelay;
		}

		inline void setSpecialKeyFlushDelay(uint64_t delay) {
			specialKeyFlushDelay = delay;
		}

		void dumpCapabilities() const;

		virtual void startTerminalUse(bool);
		virtual void stopTerminalUse();
		virtual InputMode getInputMode();
		virtual void setInputMode(InputMode);
		virtual bool hasSizeChanged();
		virtual Size getSize();

		virtual bool supportsOperation(OptionalOperation);
		virtual bool hasAutoRightMargin();
		virtual unsigned getTabSpacing();
		virtual bool carriageReturn();
		virtual void newLine();
		virtual bool clearScreen();
		virtual bool moveTo(unsigned, unsigned);
		virtual bool rowTo(unsigned);
		virtual bool columnTo(unsigned);
		virtual bool moveBy(int, int);
		virtual bool rowBy(int);
		virtual bool columnBy(int);
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
		virtual void resetTextAttributes();

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
