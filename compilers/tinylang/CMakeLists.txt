add_executable(tinylang 
	ast.cpp
	lex.cpp
	parser.cpp
	main.cpp)

set_target_properties(tinylang PROPERTIES OUTPUT_NAME "tinylang.out")
