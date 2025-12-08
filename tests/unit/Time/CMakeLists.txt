add_library(FastApplets_UnitTests_TimeTest
        TimeTest.cpp)
target_include_directories(FastApplets_UnitTests_TimeTest PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ../${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(FastApplets_UnitTests_TimeTest PRIVATE
        Time)
target_link_libraries(FastApplets_UnitTests_TimeTest PUBLIC
        GTest::gtest
        GTest::gtest_main)
