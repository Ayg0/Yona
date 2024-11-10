Yona OS:
 - OS design:
	- modular kernel.
	- 32 bit.
	- written in C and Assembly.
	- Debugging options: serial ports and (gdb or lldb).
	- uses elf executables.
 - For Codebase Consistency:
	- functions and variables uses snakeCase naming cinvention.
	- macros always uppercase and if it's more than one word they're separated by '_'.
	- structs usually made with typedef.
	- all typedefs should start with '_'.
 - For Source Tree consistency:
	- all includes should be under the include folder.
	- targeted code will reside under 'arch/<target-architecture>/'.
	- the targeted code's include will reside under 'include/arch/<the-component>.h'.
 - Portability:
	- separate the targeted code from the generic code.
	   *Targeted:
			- things that have to do with the hardware, that should change in case you changed the architecture (from x86 to ARM).
			example of that would be writting to the video memory, paging and cpu specific functions.
		*Generic:
			- things that are pure software and shouldn't be effected by hardware change, example of that would be string parsing functions,
			or buffered I/O (gets, putchar...).
	- instead of using uint32_t to refer to a memory address in 32 bit mode, it would be benificial to use something like '_phys_address',
	so if change is needed all you need is to change the type def from uint32_t to uint64_t in one place.
