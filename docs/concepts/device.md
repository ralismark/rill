# Device

A device provides access to data streams (which are character sequences). This
is normally done by modelling Source (for input) or Sink (for output). Without
either of these, the device is effectively useless, as there is no way to access
the abstracted data stream.

## Notation

- `T`, a type which models `Source`
- `Ch`, the underlying character type of `T`

## Valid Expressions / Semantics

- `typename T::char_type`
	- Is the same as `Ch`

## Example

``` c++
struct BasicIODevice
{
	using char_type = char; // alternatively, wchar_t
	std::size_t read(char* s, std::size_t n)
	{
		// see Source
	}
	std::size_t write(const char* s, std::size_t n)
	{
		// see Sink
	}
};
```
