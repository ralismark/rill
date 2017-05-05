# Sink

A sink provides write-access to an output sequence, allowing data to be
outputted to a data stream. This is done by defining a member function `read`.

## Notation

- `T`, a type which models `Sink`
- `Ch`, the underlying character type of `T`
- `sink`, an object of type `T`
- `s`, an object of type `const Ch*`
- `n`, an object of type `std::size_t`

## Valid Expressions / Semantics

`T` must model Device.

- `sink.write(s, n)`
	- Return type: `std::size_t`
	- Writes up to `n` characters from `s` into the output stream, returning
	    the number of characters written.
	- This must model `Blocking`.

## Exceptions

Errors are indicated by throwing an exception, as specified by `Device`.
Reaching the end of the output sequence is not an error.

## Example

``` c++
struct BasicSink
{
	using char_type = char;
	std::size_t write(const char* s, std::size_t n)
	{
		// Writes n characters from s to the output
	}
};
```
