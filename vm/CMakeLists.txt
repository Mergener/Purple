add_library(purplevmlib SHARED
	assembly.cpp
	bytecodereader.cpp
	purplethread.cpp
	runtime.cpp
	"export/purple_natfns.cpp"
	"export/purple_string.cpp")

set_target_properties(purplevmlib PROPERTIES OUTPUT_NAME "purple")

target_link_libraries(purplevmlib global)