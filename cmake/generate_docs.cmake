if(Doxygen_FOUND)
  # Don't generate documentation for dependencies
  set(DOXYGEN_EXCLUDE_PATTERNS ${FETCHCONTENT_BASE_DIR})
  set(DOXYGEN_GENERATE_TREEVIEW YES)

  doxygen_add_docs(docs)
endif()
