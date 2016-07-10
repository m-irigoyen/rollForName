cd ../../build/
rmdir /S /Q ship 

mkdir ship
cd ship
mkdir debug
mkdir release

cd ../UIs/Debug/
copy "rollForNameConsole.exe" "../../ship/debug/rollForName.exe"
cd ../../
cd UIs/Release/
copy "rollForNameConsole.exe" "../../ship/release/rollForName.exe"
cd ../../

cd../
xcopy /e /i /y "sources/res" "build/ship/release/res"
xcopy /e /i /y "sources/res" "build/ship/debug/res"
xcopy /e /i /y "sources/userScripts" "build/ship/debug/scripts"
xcopy /e /i /y "sources/userScripts" "build/ship/release/scripts"

cd sources/scripts