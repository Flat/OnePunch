#!/bin/sh
rm -rf ./output/*
cd ./build
rm -rf ./*
x86_64-w64-mingw32.static-cmake ..
make
x86_64-w64-mingw32-strip ./bin/OnePunch.exe
sha256sum ./bin/OnePunch.exe > ./bin/checksum.txt
gpg --output ./bin/checksum.txt.sig --detach-sig ./bin/checksum.txt
7za a -tzip ./bin/OnePunch-x86_64.zip ./bin/*.exe ./bin/*.txt ./bin/*.sig
cp ./bin/OnePunch-x86_64.zip ../output/
rm -rf ./*
i686-w64-mingw32.static-cmake ..
make
i686-w64-mingw32-strip ./bin/OnePunch.exe
sha256sum ./bin/OnePunch.exe > ./bin/checksum.txt
gpg --output ./bin/checksum.txt.sig --detach-sig ./bin/checksum.txt
7za a -tzip ./bin/OnePunch-i686.zip ./bin/*.exe ./bin/*.txt ./bin/*.sig
cp ./bin/OnePunch-i686.zip ../output/
