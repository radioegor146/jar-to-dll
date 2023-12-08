all: build

build-injector:
	javac -source 1.8 -target 1.8 injector-src/Injector.java
build-header-converter:
	javac -source 1.8 -target 1.8 header-converter-src/HeaderConverter.java
convert-injector: build-injector build-header-converter
	java -cp header-converter-src HeaderConverter injector injector-src/Injector.class native-src/classes/injector.h
convert-jar: build-header-converter
	java -cp header-converter-src HeaderConverter input-jar input.jar native-src/classes/jar.h

native_files := main.cpp injector.cpp utils.cpp

build-library: convert-injector convert-jar
	cd native-src && x86_64-w64-mingw32-g++ -O2 $(native_files) -shared -Wl,--dll -Wl,--exclude-all-symbols -static \
		 -static-libgcc -static-libstdc++ -luser32 -o ../output.dll

build: clean build-library
	echo Ready

clean-injector:
	rm -f injector-src/Injector.class
clean-header-converter:
	rm -f header-converter-src/*.class
clean-headers:
	rm -f native-src/classes/injector.h
	rm -f native-src/classes/jar.h
clean-output-library:
	rm -f output.dll
clean: clean-injector clean-header-converter clean-headers clean-output-library