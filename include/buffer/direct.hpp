#pragma once

#include <array>
#include <type_traits>
#include <cassert>

#include "device.hpp"
#include "../static_if.hpp"
#include "../concepts/concepts.hpp"

namespace rill {

template <typename Device, typename CharT, typename Traits = std::char_traits<CharT>>
class basic_direct_buffer
	: public basic_device_buffer<Device, CharT, Traits>
	, private std::array<CharT, concept::Source<Device>() ? 1 : 0> // required for input, use empty base optimisation if not Source
{
private: // internal statics

	using super = basic_device_buffer<Device, CharT, Traits>;

public: // statics

	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

protected: // internal methods

	virtual int_type underflow() override
	{
		int_type ret;
		cflow::if_<concept::Source<Device>()>([&] (auto&& id) {
			std::size_t length = id(this)->device().read(id(this)->data(), 1);
			assert(length <= 1 && "Read reports overrunning the end");

			if(length > 0) { // filled buffer
				this->setg(this->data(), this->data(), this->data() + 1);
				ret = Traits::to_int_type(*this->data());
			} else { // end of sequence
				ret = Traits::eof();
			}
		}).else_([&] (auto&& id) {
			ret = id(this)->super::underflow();
		});
		return ret;
	}

	virtual std::streamsize xsgetn(char_type* s, std::streamsize count) override
	{
		std::streamsize ret;
		cflow::if_<concept::Source<Device>()>([&] (auto&& id) {
			// empty the get area before actually calling read
			if(this->in_avail() >= 1 && count >= 1) {
				assert(this->gptr() == id(this)->data()
				       && "Get pointer must at beginning of holding area if not empty");

				*s++ = this->sbumpc(); // get one char, we know one exists
				--count;

				if(count == 0) { // no need for read
					ret = 1;
					return;
				}
			}

			auto length = id(this)->device().read(s, static_cast<std::size_t>(count));
			assert(length <= static_cast<std::size_t>(count) && "Read reports overrunning the end");
			ret = length;
		}).else_([&] (auto&& id) {
			ret = id(this)->super::xsgetn(s, count);
		});
		return ret;
	}

	virtual int_type overflow(int_type ch) override
	{
		int_type ret;
		cflow::if_<concept::Sink<Device>()>([&] (auto&& id) {
			if(ch != Traits::eof()) {
				auto c = static_cast<char_type>(ch);
				auto length = id(this)->device().write(&c, 1);
				assert(length <= 1 && "Write reports overrunning the end");

				if(length < 1) {
					ret = Traits::eof();
				}
			}
			ret = Traits::to_int_type(ch);
		}).else_([&] (auto&& id) {
			ret = id(this)->super::overflow(ch);
		});
		return ret;
	}

	virtual std::streamsize xsputn(const char_type* s, std::streamsize count) override
	{
		std::streamsize ret;
		cflow::if_<concept::Sink<Device>()>([&] (auto&& id) {
			auto length = id(this)->device().write(s, static_cast<std::size_t>(count));
			assert(length <= static_cast<std::size_t>(count) && "Write reports overrunning the end");
			ret = length;
		}).else_([&] (auto&& id) {
			ret = id(this)->super::xsputn(s, count);
		});
		return ret;
	}

public: // methods

	virtual ~basic_direct_buffer() = default;

	basic_direct_buffer()
	{
		cflow::if_<concept::Source<Device>()>([&] (auto&& id) {
			// start out full, don't take items yet
			this->setg(id(this)->data(), id(this)->data() + 1, id(this)->data() + 1);
		});
		cflow::if_<concept::Sink<Device>()>([&] (auto&&) {
			// we can do actual no buffering
			this->setp(nullptr, nullptr);
		});
	}

	// move permitted
	basic_direct_buffer(basic_direct_buffer&&) = default;
	basic_direct_buffer& operator=(basic_direct_buffer&&) = default;

};

template <typename Device>
using direct_buffer = basic_direct_buffer<Device, char>;

template <typename Device>
using wdirect_buffer = basic_direct_buffer<Device, char>;

} // namespace rill
