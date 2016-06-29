::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin

cd ../../

set PATH=%PATH%;C:\Program Files (x86)\MSBuild\14.0\Bin;C:\Program Files (x86)\CMake\bin

mkdir build
cd build
cmake ../sources/ -G "Visual Studio 14 2015 Win64"

cd ../sources/scripts