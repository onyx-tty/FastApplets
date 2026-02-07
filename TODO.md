# PROJ PowerApplet \[2/4\]

1.  \[X\] Keybindings

2.  \[X\] DBus client class for PowerActionManager

3.  \[ \] Shell command mode as an alternative to DBus mode

4.  \[ \] Configurable DBus target

    <br>

# PROJ WeatherApplet \[4/6\]

1.  \[X\] Curl

2.  \[X\] JSON

3.  \[X\] Data structure

4.  \[X\] UI skeleton

5.  \[ \] UI layout

6.  \[ \] Proper GUI

    <br>

# <span class="todo TODO">TODO</span> LauncherApplet

<br>

# <span class="todo TODO">TODO</span> SystemApplet

<br>

# <span class="todo TODO">TODO</span> SecurityApplet

<br>

# Base \[10/23\]

1.  \[X\] LICENSE

2.  \[ \] README.md

    1.  \[ \] Basics

    2.  \[ \] Requirement instructions

    3.  \[ \] Build instructions

    4.  \[ \] Usage instructions

3.  \[X\] TODO.md

4.  \[X\] Icon embedment in the executable

5.  \[ \] Default icons fetched from current icon theme

6.  \[ \] Custom icons

7.  \[ \] Fallback icons

8.  \[X\] CMake presets

9.  \[X\] TOML configuration

10. \[X\] Centralized CMake structure

11. \[ \] Enforced static compilation of private libraries

12. \[X\] Cleaner build directories

13. \[ \] Standardized window class and title

14. \[X\] Config reliant on environment variables, adhering to the XDG
    Base Directory Specification

15. \[X\] Handle type mismatches in config

16. \[ \] Handle incorrect values in config

17. \[ \] Fallback config

18. \[ \] Generate config on runtime if missing

19. \[ \] Split and break-up large functions

20. \[ \] Testing

    1.  \[ \] GTest

    2.  \[ \] GMock

    3.  \[ \] GBenchmark

21. \[ \] Extract utility functions to the new CppUtils repo

22. \[X\] Utilize the logging wrappers

23. \[ \] Encapsulate non-trivial global variables in source files

    Avoid placing non-trivial objects at namespace scope because it
    triggers dynamic initialization at program startup,  
    which can lead to Undefined Behavior if other globals or TUs depend
    on them.  
    Lazy initialize or construct on first use.
