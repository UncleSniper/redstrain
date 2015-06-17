#ifndef REDSTRAIN_MOD_IO_STREAMCLOSER_HPP
#define REDSTRAIN_MOD_IO_STREAMCLOSER_HPP

#include <cstddef>

#include "api.hpp"

namespace redengine {
namespace io {

	class Stream;

	class REDSTRAIN_IO_API StreamCloser {

	  private:
		Stream* stream;

	  public:
		StreamCloser(Stream* = NULL);
		StreamCloser(Stream&);
		StreamCloser(const StreamCloser&);
		~StreamCloser();

		inline Stream* getStream() const {
			return stream;
		}

		void close();
		void release();
		Stream* set(Stream* = NULL);

		StreamCloser& operator=(Stream*);
		StreamCloser& operator=(Stream&);
		StreamCloser& operator=(const StreamCloser&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMCLOSER_HPP */
