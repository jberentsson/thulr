all:
	cmake -B build
	cmake --build build --config Release

clean:
	rm -rf build

tidy:
	@find ./source/ -name "*.cpp" -exec clang-tidy --system-headers=0  -p build {} \
               -- -std=c++17 \
	           -I source \
	           -isystem build/_deps/catch2-src/single_include \
	           -Wno-everything \;

tidy-ci:
	@find ./source/ -name "*.cpp" -exec clang-tidy --system-headers=0  -p build {} \
	           -checks='readability-*,modernize-*,performance-*,bugprone-*,-modernize-avoid-c-arrays,-readability-identifier-naming,-bugprone-chained-comparison,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-use-enum-class' \
	           -- -std=c++17 \
	           -I source \
	           -isystem build/_deps/catch2-src/single_include \
	           -Wno-everything \;

format:
	find ./source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -exec clang-format -i {} \;

format-check:
	find ./source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -exec clang-format --dry-run --Werror {} \;

test:
	cd build && ctest -C Release --output-on-failure
