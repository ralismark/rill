#pragma once

#include <vector>
#include <streambuf>
#include <string>

#include "../static_if.hpp"

namespace rill {

// if unchecked writes is set, write in blocks instead of characters
template <bool UncheckedWrites, typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_streambuf_tee
{
public: // statics

	using char_type = CharT;

private: // variables

	// list of pointers
	std::vector<std::basic_streambuf<CharT, Traits>*> outputs;

public: // methods

	template <typename... Ts>
	basic_streambuf_tee(Ts&&... outs)
		: outputs({ std::forward<Ts>(outs)... })
	{
	}

	std::size_t write(const char_type* s, std::size_t count)
	{
		std::size_t ret;
		cflow::if_<UncheckedWrites>([&] (auto&&) {
			std::size_t min = count; // return minimum written size
			for(auto&& out : outputs) {
				std::size_t written = out->sputn(s, static_cast<std::streamsize>(count));
				if(written < min) {
					min = written;
				}
			}
			ret = min;
		}).else_([&] (auto&&) {
			std::size_t n_writes = 0;
			while(n_writes < count) {
				bool success = true;
				for(auto&& out : outputs) {
					auto ret = out->sputc(s[n_writes]);
					// check if ret is not eof
					success &= Traits::eq(Traits::not_eof(ret), ret);
				}
				if(!success) {
					break;
				}
				++n_writes;
			}
			ret = n_writes;
		});
		return ret;
	}
};

} // namespace rill
