cmake_minimum_required(VERSION 3.16...3.31)
project(pmc_lsp LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(lspcpp
  URL https://github.com/kuafuwang/LspCpp/archive/19150d12c4ae26239d75258ed598ba8ea3587cb7.tar.gz)
set(LSPCPP_BUILD_WEBSOCKETS OFF CACHE BOOL "") # stdio only
set(USE_ZLIB OFF CACHE BOOL "")                # bundled IXWebSocket: no zlib needed
FetchContent_MakeAvailable(lspcpp)

add_executable(pmc-lsp 
    src/main.cpp
    src/methods/onInitialization.cpp
    src/methods/onShutdown.cpp
    src/methods/onHover.cpp
    src/methods/onNotify.cpp
)
target_link_libraries(pmc-lsp PRIVATE lspcpp)
# Always <build dir>/pmc-lsp: "$<0:>" stops multi-config generators from adding Release/.
set_target_properties(pmc-lsp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>")
