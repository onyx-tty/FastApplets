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

# Base \[9/19\]

1.  \[X\] LICENSE

2.  \[ \] README.md

    1.  \[ \] Basics

    2.  \[ \] Requirement instructions

    3.  \[ \] Build instructions

    4.  \[ \] Usage instructions

3.  \[X\] TODO.md

4.  \[X\] Icon embedment in the executable

5.  \[ \] Fallback icons

6.  \[X\] CMake presets

7.  \[X\] TOML configuration

8.  \[X\] Centralized CMake structure

9.  \[ \] Enforced static compilation of private libraries

10. \[X\] Cleaner build directories

11. \[ \] Standardized window class and title

12. \[X\] Config reliant on environmental variables, adhering to the XDG
    Base Directory Specification

13. \[ \] Handle type mismatches in config

14. \[ \] Handle incorrect values in config

15. \[ \] Generate config on runtime if missing

16. \[ \] Fallback config

17. \[ \] Testing

    1.  \[ \] GTest

    2.  \[ \] GMock

    3.  \[ \] GBenchmark

18. \[ \] Extract utility functions to the new CppUtils repo

19. \[X\] Utilize the logging wrappers

20. \[ \] Encapsulate non-trivial global variables in source files

    Avoid placing non-trivial objects at namespace scope because it
    triggers dynamic initialization at program startup,  
    which can lead to Undefined Behavior if other globals or TUs depend
    on them.  
    Lazy initialize or construct on first use.
