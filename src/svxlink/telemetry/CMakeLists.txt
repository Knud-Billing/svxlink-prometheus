# The name of the library
set(LIBNAME telemetry)

# Which include files to export to the global include directory
set(EXPINC telemetry.h)

# What sources to compile for the library
set(LIBSRC
  telemetry.cpp
)

set(PROMETHEUS_LIBRARIES libprometheus-cpp-core.so libprometheus-cpp-pull.so)
set(LIBS ${LIBS} ${PROMETHEUS_LIBRARIES})
#set(LIBS ${LIBS} prometheus)

# Copy exported include files to the global include directory
foreach(incfile ${EXPINC})
  expinc(${incfile})
endforeach(incfile)

# Build only a static library
add_library(${LIBNAME} STATIC ${LIBSRC} ${VERSION_DEPENDS})
target_link_libraries(${LIBNAME} ${LIBS})

