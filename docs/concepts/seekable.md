# Seekable

A seekable device allows repositioning of a single read/write head, effectively
providing random-access to a sequence of characters. This is done through the
member function `seek` and `tell`.

## Notation

- `T`, a type which models `Seekable`
- `dev`, an object of type `T`
- `off`, an object of type `long long`
- `way`, an object of type `std::ios_base::seekdir`

## Valid Expressions / Semantics

`T` must model Device.

- `dev.seek(off, way)`
	- Return type: _`/* see below */_
	- Advances the read/write head to the offset specified by `off`. This is
	    calculated based on `way`:
		- The start if `way` is `std::ios_base::beg`
		- The end if `way` is `std::ios_base::end`
		- The current position if `way` is `std::ios_base::cur`
	- Behaviour is undefined if `off` is less than 0 and `way` is
	    `std::ios_base::beg`, or if `off` is greater than 0 and `way` is
	    `std::ios_base::end`. [ _Note:_ The type is not required to handle
	    these cases. _- end note_ ]
- `dev.tell()`
	- Return type: `long long`
	- Returns the current position of the read/write head. The returned
	    value is unspecified and only valid when used with the same `dev`
	    (not any object of type `T`).
	- `dev.seek(pos, std::ios_base::beg)`, where `pos` is the value of a
	    previous call to `dev.tell()`, must return the read/write head to
	    the same position as it was when `dev.tell()` was called. [ _Note:_
	    `dev.seek(dev.tell(), std::ios_base::beg)` is a no-op. _- end note_
	    ]

If `dev.seek(off, way)` returns exactly `long long`, then the return value is
returned from a `streambuf.seekoff` call. If this is the case, its value must be
equivalent to a subsequent call `dev.tell()`. Otherwise, the returned value is
ignored, and `dev.tell()` is used to determine the return value.

## Exceptions

Errors are indicated by throwing an exception, as specified by `Device`.
Seeking outside the bounds of the data stream is always an error. [ _Note:_
Seeking past the bounds is sometimes not defined. See requirements of
`dev.seek(off, way)` _- end note_ ]

## Example

``` c++
struct BasicSeekable
{
	using char_type = char;
	std::size_t read(char* s, std::size_t n)
	{
		// See concept Source
	}
	long long seek(long long off, std::ios_base::seekdir way)
	{
		// seeks to position
		// return value is not required - may be void
	}
	long long tell()
	{
		// gets current position
	}
};
