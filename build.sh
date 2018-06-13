rm -rf build
mkdir build
cd build
mkdir bin
mkdir bin/temp
mkdir bin/libs
echo "" >> bin/temp/data
cp -r ../lib/* bin/libs
cp -r ../haarcascades bin/
cp -r ../data/initdata/* bin/
cmake ..
make -j8

