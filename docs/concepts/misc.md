# Miscellaneous

There are several smaller requirements on types and operations. These are not
sufficient to fill an entire page, but must still be noted.

## Blocking

Operations performed with sinks and sources must be blocking. This means that
the operation will never produce fewer characters than requested except in the
case of reaching the end of the sequence or encountering errors. If the end of
sequence is reached, the stream will never have more data to read unless it is
modified in other ways.
