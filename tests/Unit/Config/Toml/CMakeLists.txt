qt_add_executable(UnitTests_Core_Config_Toml
        ParseTest.cpp)

target_include_directories(UnitTests_Core_Config_Toml PRIVATE
        ${UNIT_DIR}
        ${ROOT_DIR})

target_link_libraries(UnitTests_Core_Config_Toml PRIVATE
        Core::Config::Toml
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Core)

add_test(NAME UnitTests_Core_Config_Toml COMMAND UnitTests_Core_Config_Toml)
