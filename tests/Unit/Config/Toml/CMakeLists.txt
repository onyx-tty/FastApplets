add_library(UnitTests_Core_Config_Toml
        ParseTomlTest.cpp)
add_library(UnitTests::Core::Config::Toml ALIAS UnitTests_Core_Config_Toml)

target_include_directories(UnitTests_Core_Config_Toml PRIVATE
        ${UNIT_DIR}
        ${ROOT_DIR})

target_link_libraries(UnitTests_Core_Config_Toml PRIVATE
        Core::Config::Toml
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Core)
