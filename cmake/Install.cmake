
## copy resource from source directory to binrary directory
#execute_process(
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VA_WEBKIT_SOURCE_DIR}/share/lib/LiTE/data ${VA_WEBKIT_BINARY_DIR}/data
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VA_WEBKIT_SOURCE_DIR}/share/lib/LiTE/fonts ${VA_WEBKIT_BINARY_DIR}/fonts
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VA_WEBKIT_SOURCE_DIR}/fonts ${VA_WEBKIT_BINARY_DIR}/fonts
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VA_WEBKIT_SOURCE_DIR}/etc ${VA_WEBKIT_BINARY_DIR}/etc
#)

add_custom_target(ds_archive
    COMMAND tar -czf ds-${DS_TARGET}-`date +%Y-%m-%d`.tar.gz bin lib data fonts etc --exclude=*.a --exclude-vcs
    WORKING_DIRECTORY ${DS_BINRARY_DIR}
    COMMENT "archiving ds_signage.tar.gz..."
    )
