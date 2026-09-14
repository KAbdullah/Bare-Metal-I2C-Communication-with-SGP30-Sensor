cmake_minimum_required(VERSION 3.28)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nosys.specs")

# # Don't include below in production, because it allows bypassing the linker file and just checks if the main code can compile by itself
# # or not without any errors.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

project(Bare-Metal-I2C-Communication-with-SGP30-Sensor VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

add_executable("output.elf")

target_sources("output.elf"
  PRIVATE startup.c
  PRIVATE src/main.c
)

target_compile_options("output.elf"
  PRIVATE 
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16 
    -mfloat-abi=hard   
    -O0
    -g3
    -Wall
    -Wextra
)

target_link_options("output.elf"
  PRIVATE
    -nostdlib
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
)

# Don't need this since our CPU can handle .elf files
add_custom_command(
  OUTPUT output.bin
  DEPENDS output.elf
  COMMAND arm-none-eabi-objcopy -O binary output.elf output.bin
)

add_custom_target(flash
  DEPENDS output.elf
  COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program output.elf verify reset exit"
)