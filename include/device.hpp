#pragma once

#include <streambuf>
#include <string>

namespace rill {

template <typename Streambuf>
struct stream_buffer_device
{
public:

	using char_type = typename Streambuf::char_type;

private:

	Streambuf* buf;

public:

	// constructors and such {{{

	streambuf_device()
		: buf(nullptr)
	{
	}

	streambuf_device(Streambuf& sbuf)
		: buf(&sbuf)
	{
	}

	void open(Streambuf& s)
	{
		buf = &s;
	}

	bool is_open() const
	{
		return buf == nullptr;
	}

	void close() const
	{
		buf = nullptr;
	}

	// }}}

	std::size_t read(char_type* s, std::size_t count)
	{
		assert(this->is_open() && "Buffer is not null");
		auto readsize = buf->sgetn(s, static_cast<std::streamsize>(count));
		return static_cast<std::size_t>(readsize);
	}

	std::size_t write(const char_type* s, std::size_t count)
	{
		assert(this->is_open() && "Buffer is not null");
		auto writesize = buf->sputn(s, static_cast<std::streamsize>(count));
		return static_cast<std::size_t>(writesize);
	}
};

// aliases for std::streambuf
template <typename CharT, typename Traits = std::char_traits<CharT>>
using basic_streambuf_device = stream_buffer_device<std::streambuf<CharT, Traits>>;

using streambuf_device = basic_streambuf_device<char>;
using wstreambuf_device = basic_streambuf_device<wchar_t>;

} // namespace rill
