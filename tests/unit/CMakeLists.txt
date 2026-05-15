set(UNIT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(Config)

qt_add_executable(UnitTests)

target_link_libraries(UnitTests
        PRIVATE UnitTests::Config)

add_test(NAME UnitTests COMMAND UnitTests)
