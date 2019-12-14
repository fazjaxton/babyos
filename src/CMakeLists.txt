project(BabyOS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(babyos
    animation.cpp
    babyos.cpp
    main.cpp
    picture.cpp
    texture_repository.cpp
    image_scanner.cpp
    )

target_link_libraries(babyos
    sfml-graphics
    sfml-window
    sfml-system
    fmt
    )
