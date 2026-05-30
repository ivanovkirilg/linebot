if(Doxygen_FOUND)
  # Don't generate documentation for dependencies
  set(DOXYGEN_EXCLUDE_PATTERNS ${FETCHCONTENT_BASE_DIR})
  set(DOXYGEN_GENERATE_TREEVIEW YES)

  file(
    DOWNLOAD
      https://github.com/plantuml/plantuml/releases/download/v1.2026.5/plantuml-mit-1.2026.5.jar
      ${FETCHCONTENT_BASE_DIR}/plantuml.jar
      EXPECTED_HASH SHA256=2e8bf02b5f4dd3fde7bca135dea6b2b319da5c1febd88f9dc6d683d240a47697
  )
  set(DOXYGEN_PLANTUML_JAR_PATH ${FETCHCONTENT_BASE_DIR}/plantuml.jar)

  doxygen_add_docs(docs)

  add_custom_target(
    docs_pages_to_namespaces
    ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/page_to_namespace_docs.py
      SYNC COMM DOMN LOGR CTRL DRVR MAIN UI --verbose
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

  add_dependencies(docs interfaces_srcs docs_pages_to_namespaces)
endif()
