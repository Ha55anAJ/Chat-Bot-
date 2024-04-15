cmake_minimum_required(VERSION 3.27)
project(OOPSPROJ)

set(CMAKE_CXX_STANDARD 11)

add_executable(OOPSPROJ main.cpp)
include_directories("include/openai")
find_library(CURL_LIBRARY NAMES curl libcurl PATHS /opt/homebrew/Cellar/curl/8.6.0/lib) # Adjust paths if needed

if (NOT CURL_LIBRARY)
    message(FATAL_ERROR "libcurl not found!")
endif()

target_link_libraries(OOPSPROJ PRIVATE ${CURL_LIBRARY})
target_include_directories(OOPSPROJ PRIVATE /opt/homebrew/Cellar/nlohmann-json/3.11.3/include)
