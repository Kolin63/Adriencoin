.PHONY: build

build:
	cmake --build build/ -- -j$(nproc)
