cmake_minimum_required(VERSION 3.0.0)
project(raggy-bird VERSION 0.1.0)

include(CTest)
enable_testing()
cmake_policy(SET CMP0074 NEW)

file(GLOB SRCFILES *.cpp *.h)
add_executable(raggy-bird ${SRCFILES})

include_directories(headers "E:\\Program Files\\sfml_mingw\\include")

set(SFML_ROOT "E:\\Program Files\\sfml_mingw")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(raggy-bird ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/msys64/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()