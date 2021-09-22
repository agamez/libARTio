cmake_minimum_required(VERSION 3.0)

project("ART I/O generic library" C)
set(VERSION, "1.0.0")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

set(libARTio_PC ${CMAKE_CURRENT_BINARY_DIR}/libARTio.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libARTio.pc.in ${libARTio_PC} @ONLY)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
)

set(libARTio_SRCS
	io.c
)

set(libARTio_PUBLIC_HDRS
	io.h
)

set(libARTio_PRIVATE_HDRS
	io_controllers.h
)

# Build library
add_library(ARTio SHARED ${libARTio_SRCS} ${libARTio_MODULES_SRCS} ${libARTio_PUBLIC_HDRS} ${libARTio_PRIVATE_HDRS})
set_target_properties(ARTio PROPERTIES SOVERSION 1)

if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
    install(TARGETS
	ARTio

        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
endif()
if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL )
    install(FILES ${libARTio_PUBLIC_HDRS} DESTINATION "${INSTALL_INC_DIR}/libARTio/")
endif()
if(NOT SKIP_INSTALL_FILES AND NOT SKIP_INSTALL_ALL )
    install(FILES ${libARTio_PC} DESTINATION "${INSTALL_PKGCONFIG_DIR}")
endif()
