# DAW Thunk

By Darrell Wright

This C++ library employs a thunk to enable the seamless integration of local state with legacy C callbacks that support only stateless function pointers.
Thunks are small, intermediary functions, generated at runtime, that capture and store the local state, allowing it to be passed implicitly to the original callback function.
By leveraging thunks, this library efficiently bridges the gap between modern C++ programming paradigms and legacy C codebases. It provides an intuitive, type-safe interface for users to work with, ensuring the smooth and reliable operation of stateful C++ functions/callables within the context of stateless C callbacks.

The library works by creating and area in memory that has the opcodes that inserts a new pointer parameter as the first param and then calls the intended function.  This area is then made non-writable and executable.  This is similar to a JIT.

There are two parts to the library.

### Part 1:
The first component of the library enables the safe creation of a new object, erased_callable, which accepts a lambda or callable class as input. It then generates a pointer to the local state or class, along with a function pointer that has the same original parameters, with the addition of a new parameter for passing the state pointer. This mechanism ensures that the local state is seamlessly and securely incorporated into the callback function while maintaining compatibility with the legacy C codebase. This component can be used for the creation of the thunk later on, or to provide a callback function that also allows for user state.

### Part 2:
The second component generates a thunk object, daw_thunk, from a function pointer and a data pointer, or the erased_callable from Part 1. It provides a function pointer that can call the thunk, which in turn will call the provided function with the user data pointer as its first parameter. This component facilitates the efficient and secure execution of the original function while allowing for the local state to be accessed and modified as needed.

### Example usage

To quickly get started and if your lambda/callable class only has one overload, the return type/parameters can be deduced.  This allows for a quick construction like below.

```cpp
void foo( void(*)( ) );

int x = 0;
auto lamba = [&x] {
    ++x;
};
auto th = daw::make_thunk( lambda ) );
foo( th );
foo( th );
assert( x == 2 );
```

Or one can break it down into several steps.
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
th( );
// or
th_fp( );
```
The Thunk object has a conversion to the function pointer get( ) returns.

### How it works

The important part is the `thunk` structure. When it is interpreted as bytes, it matches machine code instructions that the compilers/assemblers generate. JIT'ing systems use similar techniques. At a high level, what the code does is insert an extra parameter as the first argument. This often works well because many systems treat the parameters, even if using registers, as a stack. We are pushing a new parameter onto the callstack.

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

The program uses a newly acquired page of memory via mmap so that we can ensure that it does not much about with other pages. After writing the addresses into it, it is marked as `PROT_EXEC` and no longer writable.

A function pointer to this region is created via reinterpret_cast, and that is what one uses or passes to the callback.