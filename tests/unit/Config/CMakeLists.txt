add_subdirectory(TomlParser)

add_library(UnitTests_Config INTERFACE)
add_library(UnitTests::Config ALIAS UnitTests_Config)

target_include_directories(UnitTests_Config INTERFACE
        ${UNIT_DIR}
        ${SRC_DIR})

target_link_libraries(UnitTests_Config INTERFACE
        UnitTests::Config::TomlParser)
