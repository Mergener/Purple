## Welcome to the Purple repository!

#### About

Purple is currently one of my personal projects, and it consists of a cross platform virtual machine with it's own instruction set.

Within this repository, one can find source code for the virtual machine dynamic library and its standalone executable, as well as a compiler for 'tinylang' - a very tiny programming language, as the name implies - that compiles tinylang source code to the Purple VM bytecode.

Since this is a very informal personal project, I might not be maintaning it in a regular basis.

#### Current task list:

- General:
	- Implement unit tests
	- Improve documentation

- VM:
	- Add and implement instructions for...
		- Simple jump;
		- Conditional jumps;
		- Heap access/allocation;
	- Implement a basic garbage collector
	- Improve the standalone executable
	- Add support for external bytecode dependencies
	- Implement cross-platform solution for dynamically loading native code and
	binding native procedures to Purple assemblies.

- Tinylang:
	- Finish implementing parser
	- Start implementing code generator
	- Improve error messages
	- Implement a minimal argsparser for main that allows the user to customize the following settings:
		- Source files to be compiled
		- Build output filepath and filename
		- Build output type (Purple library or executable)