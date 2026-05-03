add_subdirectory(Global)
add_subdirectory(Mapper)
add_subdirectory(PowerApplet)
add_subdirectory(Types)

add_library(Config_KeysFile INTERFACE)
add_library(Config::KeysFile ALIAS Config_KeysFile)

target_include_directories(Config_KeysFile INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${SRC_DIR})

target_link_libraries(Config_KeysFile INTERFACE
        Config::KeysFile::Global
        Config::KeysFile::Mapper
        Config::KeysFile::PowerApplet
        Config::KeysFile::Types
        Config::TOML
        CppUtils::Log::Qt)
