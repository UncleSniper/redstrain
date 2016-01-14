#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/ObjectLocker.hpp>

#include "CodecFactory.hpp"
#include "BlobCodeTable.hpp"

namespace redengine {
namespace text {

	template<typename CharT, typename CodecT, typename TableCodecT>
	class BlobTableCodecFactory : public CodecFactory<CodecT> {

	  private:
		BlobCodeTable<CharT>& table;

	  public:
		BlobTableCodecFactory(const char* data, util::MemorySize size)
				: table(*(new BlobCodeTable<CharT>(data, size))) {}

		BlobTableCodecFactory(const BlobTableCodecFactory& factory)
				: CodecFactory<CodecT>(factory), table(factory.table) {
			platform::ObjectLocker<BlobCodeTable<CharT> > locker(&table);
			table.ref();
			locker.release();
		}

		virtual ~BlobTableCodecFactory() {
			platform::ObjectLocker<BlobCodeTable<CharT> > locker(&table);
			table.unref();
		}

		inline BlobCodeTable<CharT>& getTable() {
			return table;
		}

		inline const BlobCodeTable<CharT>& getTable() const {
			return table;
		}

		virtual CodecT* newCodec() {
			return new TableCodecT(table);
		}

	};

}}
