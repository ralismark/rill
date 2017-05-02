#pragma once

#include <istream>
#include <ostream>

namespace rill {

template <typename StdStream, typename Streambuf, typename CharT, typename Traits = std::char_traits<CharT>>
class generic_stream_base
	: public Streambuf
	, public StdStream
{
public: // statics

	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

public: // methods

	virtual ~generic_stream_base() = default;

	template <typename... Ts>
	generic_stream_base(Ts&&... args)
		: Streambuf(std::forward<Ts>(args)...)
		, StdStream(static_cast<Streambuf*>(this))
	{
	}

	// permit move
	generic_stream_base(generic_stream_base&&) = default;
	generic_stream_base& operator=(generic_stream_base&&) = default;

};


template <typename IStreambuf, typename CharT, typename Traits = std::char_traits<CharT>>
using basic_istream_base = generic_stream_base<std::basic_istream<CharT, Traits>, IStreambuf, CharT, Traits>;

template <typename IStreambuf>
using istream_base = basic_istream_base<IStreambuf, char>;

template <typename IStreambuf>
using wistream_base = basic_istream_base<IStreambuf, wchar_t>;


template <typename OStreambuf, typename CharT, typename Traits = std::char_traits<CharT>>
using basic_ostream_base = generic_stream_base<std::basic_ostream<CharT, Traits>, OStreambuf, CharT, Traits>;

template <typename OStreambuf>
using ostream_base = basic_ostream_base<OStreambuf, char>;

template <typename OStreambuf>
using wostream_base = basic_ostream_base<OStreambuf, wchar_t>;


template <typename Streambuf, typename CharT, typename Traits = std::char_traits<CharT>>
using basic_stream_base = generic_stream_base<std::basic_iostream<CharT, Traits>, Streambuf, CharT, Traits>;

template <typename Streambuf>
using stream_base = basic_stream_base<Streambuf, char>;

template <typename Streambuf>
using wstream_base = basic_stream_base<Streambuf, wchar_t>;

} // namespace rill
