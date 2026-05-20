cmake_minimum_required(VERSION 3.20)

# Окремий CMake-проєкт для домашньої роботи 06. Його можна збирати як
# через root CMakeLists.txt, так і напряму з цієї директорії.
project(balistics CXX)

# Код домашньої роботи збирається з тим самим стандартом, що й лекційні приклади.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Один executable для задачі з balistics.
add_executable(telemetry_check src/main.cpp)

# Creates a static library named telemetry
add_library(telemetry STATIC src/telemetry.cpp)

# Add the 'include' folder to the search path for telemetry
target_include_directories(telemetry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Базові compiler warnings допомагають побачити підозрілі місця до runtime.
target_compile_options(telemetry PRIVATE -Wall -Wextra -pedantic)
target_compile_options(telemetry_check PRIVATE -Wall -Wextra -pedantic)

# Link the telemetry library to telemetry_check
target_link_libraries(telemetry_check PRIVATE telemetry)
