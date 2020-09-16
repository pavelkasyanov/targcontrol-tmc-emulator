QMAKE=$1
BUILD_TYPE=$2

rm -rf build/$BUILD_TYPE
mkdir -p build
cd build
mkdir $BUILD_TYPE
cd $BUILD_TYPE

$QMAKE CONFIG+=$BUILD_TYPE ../../targcontrol-tmc-emulator.pro
make -j4
