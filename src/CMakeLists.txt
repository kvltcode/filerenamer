add_executable(FileRenamer main.cpp)
target_compile_features(FileRenamer PRIVATE cxx_std_17)
target_include_directories(FileRenamer PUBLIC ${CMAKE_SOURCE_DIR}/lib)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

set_property(DIRECTORY ${CMAKE_SOURCE_DIR}/src PROPERTY VS_STARTUP_PROJECT FileRenamer)