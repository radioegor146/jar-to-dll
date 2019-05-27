# Java Dll injector

Java Dll injector sources + converter to .h file

dll ehacks-pro в корне (только для x64)

Инструкция по применению (для тех кто хочет, но не хватает ума чекнуть скрипт в корне):
1. Скачиваем mingw-64
2. Юзаем JavaDllPacker.jar -> `java -jar JavaDllPacker.jar <jar file> classes.h`
3. Копируем classes.h в папку с сорсами dll
4. Делаем 

`g++ -Wall -DBUILD_DLL -O2 -std=c++11 -m64 -c main.cpp -o main.o`

`g++ -shared -Wl,--dll main.o -o ti-pidor.dll -s -m64 -static -static-libgcc -static-libstdc++ -luser32`

5. Юзаем полученную dll (да, нет)
