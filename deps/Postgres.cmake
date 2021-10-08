

find_package(PostgreSQL)

if(NOT PostgreSQL_FOUND)
        message("find_package did not find postgres")


        find_library(postgres NAMES pq libpq libpq-dev pq-dev postgresql-devel)
        find_path(libpq-fe NAMES libpq-fe.h PATH_SUFFIXES postgresql pgsql include)
    if(NOT libpq-fe_FOUND OR NOT postgres_FOUND)
        message("No system installed Postgres found. Will build")

        add_library(postgres SHARED IMPORTED GLOBAL)
        ExternalProject_Add(postgres_src
            PREFIX ${nih_cache_path}
            GIT_REPOSITORY https://github.com/postgres/postgres.git
            GIT_TAG master
            CONFIGURE_COMMAND ./configure --without-readline
            BUILD_COMMAND ${CMAKE_COMMAND} -E env --unset=MAKELEVEL make
            UPDATE_COMMAND ""
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND ""
            BUILD_BYPRODUCTS <BINARY_DIR>/src/interfaces/libpq/${CMAKE_SHARED_LIBRARY_PREFIX}pq.so
            )



        ExternalProject_Get_Property (postgres_src SOURCE_DIR)
        ExternalProject_Get_Property (postgres_src BINARY_DIR)
        set (postgres_src_SOURCE_DIR "${SOURCE_DIR}")
        file (MAKE_DIRECTORY ${postgres_src_SOURCE_DIR}/src/include)
        file (MAKE_DIRECTORY ${postgres_src_SOURCE_DIR}/src/interfaces/libpq)

        list(APPEND INCLUDE_DIRS ${SOURCE_DIR}/src/include)
        list(APPEND INCLUDE_DIRS ${SOURCE_DIR}/src/interfaces/libpq)


        set_target_properties (postgres PROPERTIES
            IMPORTED_LOCATION
            ${BINARY_DIR}/src/interfaces/libpq/${CMAKE_SHARED_LIBRARY_PREFIX}pq.so
            INTERFACE_INCLUDE_DIRECTORIES
            "${INCLUDE_DIRS}")
        add_dependencies(postgres postgres_src)

        file(TO_CMAKE_PATH "${postgres_src_SOURCE_DIR}" postgres_src_SOURCE_DIR)

	target_link_libraries(clio PUBLIC postgres)
    else()

        message("Found system installed Postgres via find_libary")

        target_include_directories(clio INTERFACE ${libpq-fe})

        target_link_libraries(clio INTERFACE ${postgres})
    endif()

else()
    message("Found system installed Postgres via find_package")
    message("PostgreSQL_INCLUDE_DIRS is ${PostgreSQL_INCLUDE_DIRS}")
    message("PostgreSQL_LIBRARIES is ${PostgreSQL_LIBRARIES}")

    include_directories(clio INTERFACE "${PostgreSQL_INCLUDE_DIRS}")

    if(MSVC)
        if(NOT PostgreSQL_DLL)
            string(REGEX REPLACE "\.lib$" ".dll" PostgreSQL_DLL "${PostgreSQL_LIBRARY}")
        endif()
        message("PostgreSQL_DLL is ${PostgreSQL_DLL}")
        add_library(libpq SHARED IMPORTED)
        set_target_properties(libpq PROPERTIES
            IMPORTED_IMPLIB ${PostgreSQL_LIBRARY})
        file(COPY ${PostgreSQL_DLL} DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE_INIT} )
        target_link_libraries(clio PUBLIC libpq)
    else()
        target_link_libraries(clio PUBLIC ${PostgreSQL_LIBRARIES})
    endif()
endif()

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

#dump_cmake_variables()
