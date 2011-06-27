ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Sources/libutils    Sources/libutils)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Sources/tinyxml     Sources/tinyxml)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/Sources/libutils
    ${CMAKE_SOURCE_DIR}/Sources/tinyxml
    )

ADD_SUBDIRECTORY(mw)
