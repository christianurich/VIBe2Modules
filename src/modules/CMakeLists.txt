file(GLOB MODULE_CPPS *.cpp muparser/src/*.cpp)
file(GLOB MODULE_UI_FILES *.ui)
set(MOC_MODULE_FILES guicellularautomata.h guicellularautomata_neighbourhood.h guiequationeditor.h guimarker.h guisinglevalue.h)

QT4_WRAP_UI(MODULE_CPPS ${MODULE_UI_FILES})
QT4_WRAP_CPP(MODULE_CPPS ${MOC_MODULE_FILES})

include_directories(muparser/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)
include_directories(${VIBE2CORE_INCLUDE_DIRS})

add_library(vibe2modules SHARED ${MODULE_CPPS})

target_link_libraries(vibe2modules ${VIBE2CORE_LIBRARIES} ${VIBE2HELPER_LIBRARIES} ${QT_LIBRARIES})

INSTALL(TARGETS vibe2modules
        DESTINATION "lib"
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
