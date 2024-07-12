all: build

build-injector:
	@echo "Building injector class"
	@javac -source 1.8 -target 1.8 injector-src/*.java
build-header-converter:
	@echo "Building class to header converter"
	@javac -source 1.8 -target 1.8 header-converter-src/HeaderConverter.java
convert-injector: build-injector build-header-converter
	@echo "Converting injector class to .h"
	@java -cp header-converter-src HeaderConverter injector injector-src/*.class native-src/classes/injector.h
convert-jar: build-header-converter
	@echo "Converting input jar to .h"
	@java -cp header-converter-src HeaderConverter input-jar input.jar native-src/classes/jar.h

native_files := main.cpp injector.cpp utils.cpp

build-library: convert-injector convert-jar
	@echo "Building output dll"
	@cd native-src && x86_64-w64-mingw32-g++ -O2 $(native_files) -shared -Wl,--dll -Wl,--exclude-all-symbols -static \
		 -static-libgcc -static-libstdc++ -luser32 -o ../output.dll

build: clean build-library
	@echo "Ready!"

clean-injector:
	@echo "Cleaning injector classes"
	@rm -f injector-src/*.class
clean-header-converter:
	@echo "Cleaning class to header converter classes"
	@rm -f header-converter-src/*.class
clean-headers:
	@echo "Cleaning converted injector and input jar"
	@rm -f native-src/classes/injector.h
	@rm -f native-src/classes/jar.h
clean-output-library:
	@echo "Cleaning output dll"
	@rm -f output.dll
clean: clean-injector clean-header-converter clean-headers clean-output-library
	@echo "Cleaning everything"
