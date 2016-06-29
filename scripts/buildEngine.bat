::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin

cd ../

set PATH=%PATH%;C:\Program Files (x86)\MSBuild\14.0\Bin;C:\Program Files (x86)\CMake\bin

cd ../build
msbuild "RollForName.sln" /p:Configuration=Debug /p:Platform="x64"
msbuild "RollForName.sln" /p:Configuration=Release /p:Platform="x64"

cd ../sources/scripts