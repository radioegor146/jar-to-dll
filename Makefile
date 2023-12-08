build-injector:
	javac injector-src/Injector.java
build-header-converter:
	javac header-converter-src/HeaderConverter.java
convert-injector: build-injector build-header-converter
	java -cp header-converter-src HeaderConverter injector-src/Injector.class native-src/classes/injector.h
convert-jar: build-header-converter
	java -cp header-converter-src HeaderConverter input.jar native-src/classes/jar.h
build-library: convert-injector convert-jar
	cd native-src && x86_64-w64-mingw32-g++ -O2 main.cpp -shared -Wl,--dll -static \
		-static-libgcc -static-libstdc++ -luser32 -o ../output.dll
build: build-library
	echo Ready