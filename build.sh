cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true

cd build && make clean && make -j8


