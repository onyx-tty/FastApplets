set(UNIT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(Config)

qt_add_executable(UnitTests)

target_link_libraries(UnitTests
        PRIVATE UnitTests::Config
        PUBLIC  Qt::Test
                Qt${QT_VERSION_MAJOR}::Core)

add_test(NAME UnitTests COMMAND UnitTests)
