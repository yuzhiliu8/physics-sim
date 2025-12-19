
.PHONY: run build clean

build:
	cmake -S . -B build
	cmake --build build -j$$(nproc)

run: build
	./build/physics-sim

clean:
	rm -rf build

