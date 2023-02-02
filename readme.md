# Thunk a Dunk

By Darrell Wright

## A Cross Platform generic thunking library

The library comes in 2 parts.

### Part 1

The first part is a facility to convert any callable to a `void *` to the object and a function pointer that takes as it's first parameter the `void *`

### Part 2

The second parts takes a function pointer and a data pointer. It generates a thunking later that returns a funtion pointer without the `void *` parameter.

### Example usage

```cpp
int x = 0;
auto lambda = [&x]( ) -> void {
  ++x;
};
```

Create a type erased callable, that gives us a `void *` state pointer to the lamba class and a `void(*)( void * )` function pointer to a function that will call convert the lambda back safely to it's proper type and call it.

```c++
auto fp = daw::erased_callable<void( )>{ lambda };
```

Create a Thunk type that will give us a `void(*)( )` function pointer that the callback requires. The `Thunk` object holds the state and the `get( )` method returns a function pointer to the reinterpreted and executable new function.

```c++
auto th = daw::Thunk<void( )>( fp.data, fp.fp );
auto th_fp = th.get( );
```

Call the callback

```c++
th_fp( );
```

### How it works

The important part is the `thunk` structure. When it is interpreted as bytes, it matches machine code instructions that the compilers/assemblers generate. JIT'ing systems use similar techniques. At a high level, what the code does is insert an extra parameter as the first argument. This often works well because many systems treat the parametrs, even if using registers, as a stack. We are pushing a new parameter onto the callstack.

For example. We have a callback that takes a function pointer with the signature `void(*)( )`. This does not allow for the caller to supply state(this is common in capturing lambda's in C++), `erased_callable` helps with creating a `void * state` and a function pointer like `void(*)( void * )`. We need to call this, but the system will only call a `void(*)( )`. So we use a structure like

```c++
struct __attribute__( ( packed ) ) thunk {
  unsigned char mov[2] = { 0x48, 0xBF };
  void *state = nullptr;
  unsigned char movrax[2] = { 0x48, 0xB8 };
  void *function_pointer = nullptr;
  unsigned char jmp[2] = { 0xFF, 0xE0 };
};
```

The above `thunk` is for the Itanium ABI. It is like the following assembler. `RAX` is the location of the first parameter in a function call.

```asm
mov rdi, [state]
mov rax, [function_ptr]
jmp rax
```