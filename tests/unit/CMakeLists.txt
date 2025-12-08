add_subdirectory(Time)

qt_add_executable(FastApplets_UnitTests)
target_link_libraries(FastApplets_UnitTests PUBLIC
        GTest::gtest
        GTest::gtest_main
        Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(FastApplets_UnitTests PRIVATE
        FastApplets_UnitTests_TimeTest)

add_test(NAME FastApplets_UnitTests COMMAND FastApplets_UnitTests)
