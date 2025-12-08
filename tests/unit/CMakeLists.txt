qt_add_executable(FastApplets_UnitTests)
target_link_libraries(FastApplets_UnitTests PUBLIC
        GTest::gtest
        GTest::gtest_main
        Qt${QT_VERSION_MAJOR}::Core)

add_test(NAME FastApplets_UnitTests COMMAND FastApplets_UnitTests)
