qt_add_executable(UnitTests_Core_Args
        ArgsTest.cpp)

target_include_directories(UnitTests_Core_Args PRIVATE
        ${UNIT_DIR}
        ${ROOT_DIR})

target_link_libraries(UnitTests_Core_Args PRIVATE
        Core::Args
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Core)

add_test(NAME UnitTests_Core_Args COMMAND UnitTests_Core_Args)
