add_library(FastApplets_UnitTests_Time
        TimeTest.cpp)
add_library(FastApplets::UnitTests::Time ALIAS FastApplets_UnitTests_Time)

target_include_directories(FastApplets_UnitTests_Time PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ../${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})

target_link_libraries(FastApplets_UnitTests_Time PRIVATE
        Time)

target_link_libraries(FastApplets_UnitTests_Time PUBLIC
        GTest::gtest
        GTest::gtest_main)
