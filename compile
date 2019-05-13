#!/bin/bash
> lock
echo Packing...
java -jar JavaDllPacker.jar $1 classes.h
if [ $? -eq 1 ]
then
   echo "Error - breaking"
   rm classes.h
   rm main.o
   rm lock
   exit 1
fi
echo Compiling...
echo x86_64-w64-mingw32-g++ -Wall -DBUILD_DLL -O2 -O2 -std=c++11 -m64  -c main.cpp -o main.o
x86_64-w64-mingw32-g++ -Wall -DBUILD_DLL -O2 -O2 -std=c++11 -m64  -c main.cpp -o main.o
if [ $? -eq 1 ]
then
   echo "Error - breaking"
   rm classes.h
   rm main.o
   rm lock
   exit 1
fi
echo x86_64-w64-mingw32-g++ -shared -Wl,--dll main.o -o $2 -s -m64 -static -static-libgcc -static-libstdc++ -luser32
x86_64-w64-mingw32-g++ -shared -Wl,--dll main.o -o $2 -s -m64 -static -static-libgcc -static-libstdc++ -luser32
if [ $? -eq 1 ]
then
   echo "Error - breaking"
   rm classes.h
   rm main.o
   rm lock
   exit 1
fi
echo Ready!
rm classes.h
rm main.o
rm lock
exit 0
