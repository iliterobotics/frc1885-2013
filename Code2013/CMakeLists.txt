add_library(comm STATIC
	binarychunkreader.cpp
	binarychunkwriter.cpp
	chunkreader.cpp
	chunkrwfactory.cpp
	chunktransmission.cpp
	chunkwriter.cpp
	neterror.cpp
	nettypes.cpp
	textchunkreader.cpp
	textchunkwriter.cpp
)

target_link_libraries(comm)
