#pragma once

#include <streambuf>
#include <string>
#include <optional>

namespace rill {

/**
 * \class basic_device_buffer
 * \brief null buffer to hold a device
 *
 * This class exists to provide a unified interface to bind a device to a
 * buffer. Custom stream buffers provide a device template parameter and
 * inherit from this, allowing all stream buffers to be used the same way.
 */

template <typename Device, typename CharT, typename Traits = std::char_traits<CharT>>
class basic_device_buffer
	: public std::basic_streambuf<CharT, Traits>
{
public: // statics

	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

private: // variables

	std::optional<Device> m_device;

protected: // internal methods

	basic_device_buffer()
		: m_device()
	{
	}

public: // methds

	virtual ~basic_device_buffer() = default;

	// move permitted
	basic_device_buffer(basic_device_buffer&&) = default;
	basic_device_buffer& operator=(basic_device_buffer&&) = default;

	template <typename... Ts>
	void open(Ts&&... args)
	{
		m_device.emplace(std::forward<Ts>(args)...);
	}

	bool is_open() const
	{
		return m_device.has_value();
	}

	void close()
	{
		m_device.reset();
	}

	// device {{{

	Device& device()
	{
		return *m_device;
	}

	const Device& device() const
	{
		return *m_device;
	}

	Device* operator->()
	{
		return &this->device();
	}

	const Device* operator->() const
	{
		return &this->device();
	}

	const Device& operator*() const
	{
		return this->device();
	}

	Device& operator*()
	{
		return this->device();
	}

	// }}}

};

/**
 * \fn open
 * \brief open anything openable
 *
 * This opens any with the member function open, such as stream buffers and
 * actual streams.
 */
template <typename T, typename... Ts>
auto open(Ts&&... args)
{
	T buf;
	buf.open(std::forward<Ts>(args)...);
	return buf;
}

} // namespace rill
