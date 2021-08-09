cmake_minimum_required(VERSION 2.8.7)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(sweettest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(sweettest_SOURCES 
testrunner.cpp
bitsettest/bitsettest.cpp
rangetest/rangetest.cpp
faptest/faptest.cpp
fectortest/fectortest.cpp
algotest/algotest.cpp
dbctest/dbctest.cpp
#logtest/loggertest.cpp
formattest/formattest.cpp
filesystemtest/filesystemtest.cpp
benchmarks/strcmpsse.cpp
benchmarks/setVsUnordered.cpp
convtest/convtest.cpp
loggertest/loggertest.cpp
fixedtest/fixedtest.cpp
cachetest/cachetest.cpp
comparetest/comparetest.cpp
optiontest/optiontest.cpp
deathtest/death.cpp
base64test/base64test.cpp
stringfunctiontest/stringtest.cpp
unittest/unittest.cpp
benchmarktest/benchmarktest.cpp
ssimdtest/ssimdtest.cpp
typelist.cpp
sjsontest/sjsontest.cpp
bitvectortest/bitvectortest.cpp
bllocatortest/allocatortest.cpp
stringtest/stringcmptest.cpp
int128test/int128test.cpp
decimaltest/decimaltest.cpp
getoptstest/getoptstest.cpp
combinationstest/combitest.cpp
)

set(sweettest_HEADERS
filesystem.hpp
cache.hpp
stringfunctions.hpp
format.hpp
bitvector.hpp
semaphore.hpp
compare.hpp
bitset.hpp
death.hpp
nanoxml.hpp
sweetqlimpl/sqliteimpl.hpp
sweetqlimpl/types.hpp
log.hpp
getopts.hpp
fixed.hpp
trie.hpp
streamhelper.hpp
base64.hpp
sweetql.hpp
decimal.hpp
fector.hpp
sjson.hpp
fap.hpp
parallelalgo.hpp
logger.hpp
allocator.hpp
int128.hpp
benchmark.hpp
options.hpp
unit.hpp
bllocator.hpp
ssimd.hpp
dbc.hpp
conv.hpp
sysinfo.hpp
combinations.hpp
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb --std=c++1y -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}/includes
)

option(WITH_TESTS "Build tests" ON)

if(WITH_TESTS)
    add_executable(sweettest
        ${sweettest_SOURCES}
        ${sweettest_HEADERS}
    )

    target_link_libraries(sweettest sqlite3 pthread)
endif()

