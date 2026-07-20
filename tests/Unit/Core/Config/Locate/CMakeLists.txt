qt_add_executable(UnitTests_Core_Config_Locate
        LocateTest.cpp)

target_include_directories(UnitTests_Core_Config_Locate PRIVATE
        ${UNIT_DIR}
        ${ROOT_DIR})

target_link_libraries(UnitTests_Core_Config_Locate PRIVATE
        Core::Config::Locate
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Core)

add_test(NAME UnitTests_Core_Config_Locate COMMAND UnitTests_Core_Config_Locate)
