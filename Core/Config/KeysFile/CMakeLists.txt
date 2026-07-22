add_subdirectory(Keys)
add_subdirectory(Mapper)
add_subdirectory(Types)

add_applet_library(Core_Config_KeysFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::KeysFile::Keys
                       Core::Config::KeysFile::Mapper
                       Core::Config::KeysFile::Types)
