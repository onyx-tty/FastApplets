add_library(Config_Resolvers
        Extractors.cpp
        Resolvers.h
        Resolvers.tpp)
add_library(Config::Resolvers ALIAS Config_Resolvers)

target_include_directories(Config_Resolvers PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Resolvers PRIVATE
        Config::TOML)

target_link_libraries(Config_Resolvers PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
