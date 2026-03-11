if(Doxygen_FOUND)
  # Don't generate documentation for dependencies
  set(DOXYGEN_EXCLUDE_PATTERNS ${FETCHCONTENT_BASE_DIR})
  set(DOXYGEN_GENERATE_TREEVIEW YES)

  file(
    DOWNLOAD
      https://github.com/plantuml/plantuml/releases/download/v1.2026.2/plantuml.jar
      ${FETCHCONTENT_BASE_DIR}/plantuml.jar
      EXPECTED_HASH SHA256=3cdce52133c424dea22425b947ae9d47f2167b0866dfcf99e714d4ea1689975c
  )
  set(DOXYGEN_PLANTUML_JAR_PATH ${FETCHCONTENT_BASE_DIR}/plantuml.jar)

  doxygen_add_docs(docs)
endif()
