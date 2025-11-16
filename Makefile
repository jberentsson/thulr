all:
	cmake -B build
	cmake --build build --config Release

tidy:
	clang-tidy source/**/*.{cpp,hpp} -- std=c++17 -Ibuild/_deps/catch2-src/single_include

format:
	clang-format -i source/*/*.{hpp,cpp}
