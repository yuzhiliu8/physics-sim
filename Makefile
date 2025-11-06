
.PHONY: run build clean

build:
	cmake -S . -B build
	cmake --build build -j$$(nproc)

run: build
	./build/physics-engine

clean:
	rm -rf build

