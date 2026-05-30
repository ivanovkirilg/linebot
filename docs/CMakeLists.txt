macro(exclude_from_docs)
  set(CACHE{DOXYGEN_EXCLUDE_PATTERNS} FORCE VALUE ${DOXYGEN_EXCLUDE_PATTERNS} ${ARGV})
endmacro()

if(Doxygen_FOUND)
  # Don't generate documentation for dependencies
  exclude_from_docs(${FETCHCONTENT_BASE_DIR})

  set(CACHE{DOXYGEN_GENERATE_TREEVIEW} VALUE YES)

  file(
    DOWNLOAD
      https://github.com/plantuml/plantuml/releases/download/v1.2026.5/plantuml-mit-1.2026.5.jar
      ${FETCHCONTENT_BASE_DIR}/plantuml.jar
      EXPECTED_HASH SHA256=2e8bf02b5f4dd3fde7bca135dea6b2b319da5c1febd88f9dc6d683d240a47697
  )
  set(CACHE{DOXYGEN_PLANTUML_JAR_PATH} FORCE VALUE ${FETCHCONTENT_BASE_DIR}/plantuml.jar)

  add_custom_target(
    docs_pages_to_namespaces
    ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/page_to_namespace_docs.py
      SYNC COMM DOMN LOGR CTRL DRVR MAIN UI --verbose
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

function(generate_docs)
  if(Doxygen_FOUND)
    doxygen_add_docs(docs)
    add_dependencies(docs interfaces_srcs docs_pages_to_namespaces)
  else()
    message("Doxygen not found, skipping docs.")
  endif()
endfunction()
