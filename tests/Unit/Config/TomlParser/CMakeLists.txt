add_library(UnitTests_Core_Config_TomlParser
        TomlParserTest.cpp)
add_library(UnitTests::Core::Config::TomlParser ALIAS UnitTests_Core_Config_TomlParser)

target_include_directories(UnitTests_Core_Config_TomlParser PRIVATE
        ${UNIT_DIR}
        ${ROOT_DIR})

target_link_libraries(UnitTests_Core_Config_TomlParser
        PRIVATE Core::Config::TomlParser
        PUBLIC  Qt${QT_VERSION_MAJOR}::Test
                Qt${QT_VERSION_MAJOR}::Core)
