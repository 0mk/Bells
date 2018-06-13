all: build html
	make -C build

clean:
	rm -rf html latex
	make -C build clean

build: 
	mkdir -p build
	cd build; cmake ..

html: Doxyfile
	doxygen Doxyfile

install:
	make -C build install
