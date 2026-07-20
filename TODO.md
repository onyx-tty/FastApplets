# PROJ PowerApplet \[0/2\]

1.  \[ \] Display logs in a separate window on power action

2.  \[ \] Exclude hibernate from a list of buttons if hibernation is not
    supported

    <br>

# PROJ ActionApplet \[0/1\]

1.  \[ \] Notify about missing buttons instead of quitting with qFatal

    <br>

# PROJ WeatherApplet \[4/7\]

1.  \[X\] Curl

2.  \[X\] JSON

3.  \[X\] Data structure

4.  \[X\] UI skeleton

5.  \[ \] Add WeatherApplet under current design

6.  \[ \] UI layout

7.  \[ \] Clean UI

    <br>

# PROJ PlayerApplet \[0/0\]

<br>

# Docs \[3/6\]

1.  \[X\] LICENSE

2.  \[X\] README.md

3.  \[X\] TODO.md

4.  \[ \] CONTRIBUTE.md

5.  \[ \] Doxygen

6.  \[ \] Implementation docs

    <br>

# Infrastructure \[11/17\]

1.  \[X\] CMake presets

2.  \[X\] TOML configuration

3.  \[X\] Centralized CMake structure

4.  \[X\] Static linkage

5.  \[X\] Cleaner build directories

6.  \[X\] Docker

    1.  \[X\] Compilation

    2.  \[X\] Running tests

    3.  \[X\] Deployment

7.  \[X\] GitHub Workflow

    1.  \[X\] Compilation

    2.  \[X\] Running tests

8.  \[X\] Testing

    1.  \[X\] Unit tests

9.  \[X\] Dependencies

    1.  \[X\] Extract CppUtils

    2.  \[X\] Extract TomlQt

10. \[X\] Extract src/Applets/PowerApplet into PowerApplet/, replace
    src/ with Core/

11. \[X\] Add 'develop' branch

12. \[ \] Support CPack

13. \[ \] Add unit tests for all Config libraries

14. \[ \] Git tags and releases

15. \[ \] Improve template safety with constraints

16. \[ \] Transition TODOs entirely to markdown format

17. \[ \] Remove clang and ninja as recommended dependencies

    <br>

# UI \[0/2\]

1.  \[ \] Generic MainWindow, CentralWidget, and PrimaryButton

2.  \[ \] Support different layouts

    <br>

# Controls \[2/3\]

1.  \[X\] Keybindings

2.  \[X\] Option to disable button 'staging', single button presses
    activate buttons

3.  \[ \] Make mouse controls work well alongside 'staging', prevent
    sunken state on mouse click

    <br>

# Config \[8/10\]

1.  \[X\] Standardized window class and title

2.  \[X\] Config error handling

3.  \[X\] Config value validation

4.  \[X\] Fallback config

5.  \[X\] \$XDG<sub>CONFIGHOME</sub> for config

6.  \[X\] Template overloads for Config/Keys mappers to avoid duplicate
    logic between applets

7.  \[X\] Separate Config/Keys schema from instances

8.  \[X\] Make config schemas generic

9.  \[ \] Support args

10. \[ \] Add fallback \$HOME/.config/FastApplets if
    \$XDG<sub>CONFIGHOME</sub> envvar is missing/empty/wrong

    <br>

# Shell \[2/2\]

1.  \[X\] Shell command mode

2.  \[X\] Support shell expansion

    <br>

# Resources \[1/3\]

1.  \[X\] PowerButton icons

2.  \[ \] Icons from current icon theme

3.  \[ \] Custom icons

    <br>

# Style \[1/2\]

1.  \[ \] nodiscard where applicable

2.  \[X\] Standardize initialization style - always use '='

    <br>

# Other \[0/1\]

1.  \[ \] Consider 'notify-send' for warnings and similar, disable in
    config
