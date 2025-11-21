all:
	cmake -B build
	cmake --build build --config Release

clean:
	rm -rf build

tidy:
	find source/thulr/source -name "*.cpp" -o -name "*.hpp" | \
	    xargs clang-tidy \
	    -- -std=c++17 \
	    -I source/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/max-sdk-base/c74support/max-includes \
	    -isystem build/_deps/catch2-src/single_include \
	    -isystem source/min-api/test \
	    -Wno-everything

tidy-ci:
	find source/thulr/source -name "*.cpp" -o -name "*.hpp" | \
	    xargs clang-tidy \
	    -checks='readability-*,modernize-*,performance-*,bugprone-*,-modernize-avoid-c-arrays,-readability-identifier-naming,-bugprone-chained-comparison,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-use-enum-class' \
	    -- -std=c++17 \
	    -I source/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/max-sdk-base/c74support/max-includes \
	    -isystem build/_deps/catch2-src/single_include \
	    -isystem source/min-api/test \
	    -Wno-everything

format:
	find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" | \
	    xargs clang-format -i

format-check:
	find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" | \
	    xargs clang-format --dry-run --Werror

test:
	cd build && ctest -C Release --output-on-failure
