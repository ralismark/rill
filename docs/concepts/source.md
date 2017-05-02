# Source

A source provides a way to read data (as characters) from an input sequence.
This is done by defining a member function `read`.

## Notation

- `T`, a type which models `Source`
- `Ch`, the underlying character type of `T`
- `src`, an object of type `T`
- `s`, an object of type `Ch*`
- `n`, an object of type `std::size_t`

## Valid Expressions / Semantics

`T` must model Device.

- `src.read(s, n)`
	- Return type: `std::size_t`
	- Reads up to `n` characters from the input sequence into `s`,
	    returning the number of characters read. This is 0 if the end of the
	    sequence has been previously reached.
	- This must model `Blocking`.

## Exceptions

Errors which occur during execution of `read` are indicated by throwing an
exception. Reaching the end of the sequence is not an error. After an exception
is thrown, `src` must be in a consistent state; further calls to `read` may
throw, but must have well-defined behaviour.

## Example

``` c++
struct BasicSource
{
	using char_type = char;
	std::size_t read(char* s, std::size_t n)
	{
		// Reads characters and writes them to s
	}
};
```
