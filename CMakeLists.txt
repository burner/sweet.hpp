cmake_minimum_required(VERSION 2.8.7)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(sweet)

set(PROJECT_VERSION "0.0")
set(PROJECT_DESCRIPTION "A bunch of header useful for daily life. The headers contain functionally for
everyday life.")
set(PROJECT_HOMEPAGE_URL "https://github.com/burner/sweet.hpp")

include(CPackComponent)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

string(REPLACE "/${CMAKE_LIBRARY_ARCHITECTURE}" "" CMAKE_INSTALL_LIBDIR_ARCHIND "${CMAKE_INSTALL_LIBDIR}")

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Robert 'BuRnEr' Schadek")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_COMPONENTS_GROUPING IGNORE)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_NSIS_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

set("CPACK_DEBIAN_PACKAGE_NAME" "lib${CPACK_PACKAGE_NAME}-dev")
set("CPACK_RPM_PACKAGE_NAME" "lib${CPACK_PACKAGE_NAME}-devel")


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

set(OUR_INCLUDE_DEST "${CMAKE_INSTALL_INCLUDEDIR}/sweet")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${OUR_INCLUDE_DEST}>)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	INCLUDES DESTINATION "${OUR_INCLUDE_DEST}"
)

set(CONFIG_FILE_NAME_WITHOUT_EXT "${PROJECT_NAME}-config")
set(CMAKE_CONFIG_FILE_BASENAME "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME_WITHOUT_EXT}")
set(CMAKE_CONFIG_VERSION_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}-version.cmake")
set(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}.cmake")

export(EXPORT ${CONFIG_FILE_NAME_WITHOUT_EXT} NAMESPACE ${PROJECT_NAME}::)
install(EXPORT ${CONFIG_FILE_NAME_WITHOUT_EXT}
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR_ARCHIND}/cmake/${PROJECT_NAME}
)
install(FILES ${sweettest_HEADERS}
	DESTINATION ${OUR_INCLUDE_DEST}
)

if(${CMAKE_VERSION} VERSION_GREATER "3.14")
	set(OPTIONAL_ARCH_INDEPENDENT "ARCH_INDEPENDENT")
endif()

write_basic_package_version_file("${CMAKE_CONFIG_VERSION_FILE_NAME}"
	COMPATIBILITY AnyNewerVersion
	${OPTIONAL_ARCH_INDEPENDENT}
)

set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")
configure_file("${CMAKE_SOURCE_DIR}/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
install(FILES "${PKG_CONFIG_FILE_NAME}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/pkgconfig"
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb --std=c++1y -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")



option(WITH_TESTS "Build tests" ON)

if(WITH_TESTS)
    add_executable(sweettest
        ${sweettest_SOURCES}
        ${sweettest_HEADERS}
    )

    target_include_directories(sweettest
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
     )

    target_link_libraries(sweettest sqlite3 pthread)
endif()


include(CPack)
