add_library(UnitTests_Config_TomlParser
        TomlParserTest.cpp)
add_library(UnitTests::Config::TomlParser ALIAS UnitTests_Config_TomlParser)

target_include_directories(UnitTests_Config_TomlParser PRIVATE
        ${UNIT_DIR}
        ${SRC_DIR})

target_link_libraries(UnitTests_Config_TomlParser
        PRIVATE Config::TomlParser
        PUBLIC  Qt${QT_VERSION_MAJOR}::Test
                Qt${QT_VERSION_MAJOR}::Core)
