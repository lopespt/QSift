mkdir build
cd build
cmake -DCMAKE_EXE_LINKER_FLAGS=-lcblas -DWITH_CUDA=OFF ..
make -j8
