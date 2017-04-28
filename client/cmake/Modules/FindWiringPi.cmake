# - Try to find WIREPI
# Once done this will define
#
#  WIREPI_FOUND - system has WIREPI
#  WIREPI_INCLUDE_DIRS - the WIREPI include directory
#  WIREPI_LIBRARIES - Link these to use WIREPI
#  WIREPI_DEFINITIONS - Compiler switches required for using WIREPI

if (WIREPI_LIBRARIES AND WIREPI_INCLUDE_DIRS)
  # in cache already
  set(WIREPI_FOUND TRUE)
else (WIREPI_LIBRARIES AND WIREPI_INCLUDE_DIRS)

  find_path(WIREPI_INCLUDE_DIR
    NAMES
      wiringPi.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /opt/wiringPi/wiringPi/
  )

  find_library(WIREPI_LIBRARY
    NAMES
      wiringPi
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  set(WIREPI_INCLUDE_DIRS
    ${WIREPI_INCLUDE_DIR}
  )

  if (WIREPI_LIBRARY)
    set(WIREPI_LIBRARIES
        ${WIREPI_LIBRARIES}
        ${WIREPI_LIBRARY}
    )
  endif (WIREPI_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(WIREPI DEFAULT_MSG WIREPI_LIBRARIES WIREPI_INCLUDE_DIRS)

  # show the WIREPI_INCLUDE_DIRS and WIREPI_LIBRARIES variables only in the advanced view
  mark_as_advanced(WIREPI_INCLUDE_DIRS WIREPI_LIBRARIES)

endif (WIREPI_LIBRARIES AND WIREPI_INCLUDE_DIRS)

