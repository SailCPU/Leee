# Leee Third-Party Libraries Configuration
# This file configures all header-only third-party libraries for CMake

cmake_minimum_required(VERSION 3.16)

# Get the directory of this file
get_filename_component(THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

# Function to add header-only library
function(add_header_only_library name include_dir)
    if(NOT TARGET ${name})
        add_library(${name} INTERFACE)
        target_include_directories(${name} INTERFACE ${include_dir})
        set_target_properties(${name} PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${include_dir}
        )
    endif()
endfunction()

# Asio - Asynchronous I/O Library
add_header_only_library(asio "${THIRD_PARTY_DIR}/asio")
if(WIN32)
    target_link_libraries(asio INTERFACE ws2_32 wsock32)
elseif(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(asio INTERFACE Threads::Threads)
endif()

# nlohmann/json - JSON Library
add_header_only_library(nlohmann_json "${THIRD_PARTY_DIR}/json")

# fmt - Formatting Library
add_header_only_library(fmt "${THIRD_PARTY_DIR}/fmt")

# plog - Logging Library
add_header_only_library(plog "${THIRD_PARTY_DIR}/plog")

# Eigen - Linear Algebra Library
add_header_only_library(eigen "${THIRD_PARTY_DIR}/eigen")

# doctest - Testing Framework
add_header_only_library(doctest "${THIRD_PARTY_DIR}/doctest")

# httplib - HTTP Library
add_header_only_library(httplib "${THIRD_PARTY_DIR}/httplib")
if(WIN32)
    target_link_libraries(httplib INTERFACE ws2_32 crypt32)
elseif(APPLE)
    target_link_libraries(httplib INTERFACE "-framework Security")
endif()

# WebSocket++ - WebSocket Library
add_header_only_library(websocketpp "${THIRD_PARTY_DIR}/websocketpp")

# Create an interface library that includes all header-only libraries
add_library(leee_third_party INTERFACE)
target_link_libraries(leee_third_party INTERFACE
    asio
    nlohmann_json
    fmt
    plog
    eigen
    doctest
    httplib
    websocketpp
)

# Set C++ standard requirements
target_compile_features(leee_third_party INTERFACE cxx_std_17)

# Export variables for easy inclusion
set(THIRD_PARTY_INCLUDE_DIRS
    "${THIRD_PARTY_DIR}/asio"
    "${THIRD_PARTY_DIR}/json"
    "${THIRD_PARTY_DIR}/fmt"
    "${THIRD_PARTY_DIR}/plog"
    "${THIRD_PARTY_DIR}/eigen"
    "${THIRD_PARTY_DIR}/doctest"
    "${THIRD_PARTY_DIR}/httplib"
    "${THIRD_PARTY_DIR}/websocketpp"
    PARENT_SCOPE
)
