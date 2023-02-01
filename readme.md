# Thunk a Dunk
By Darrell Wright
## A Cross Platform generic thunking library

The library comes in 2 parts.

### Part 1

The first part is a facility to convert any callable to a `void *` to the object and a function pointer that takes as it's first parameter the `void *`

### Part 2

The second parts takes a function pointer and a data pointer. It generates a thunking later that returns a funtion pointer without the `void *` parameter.

### Example usage

