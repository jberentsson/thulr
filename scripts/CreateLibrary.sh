#!/usr/bin/env bash
#
# ========================= DESCRIPTION =========================
# This scripts creates all of the files needed 
# to start programming a new shared library.
#
# ========================= FILES ===============================
# The files and directories that are created are.
# - NewLibraryName/
# - NewLibraryName/CMakeLists.txt
# - NewLibraryName/NewLibraryName.cpp
# - NewLibraryName/NewLibraryName.hpp
# - NewLibraryName/NewLibraryName_test.hpp
#
# ========================= USAGE ===============================
# ./scripts/Createibrary.sh
#

if [ $# -eq 0 ]; then
    echo "No library name specified!"
else
    # Set the variables.
    NEW_LIBRARY_NAME=$1
    NEW_LIBRARY_DIRECTORY="$NEW_LIBRARY_NAME"
    NEW_LIBRARY_PATH="$NEW_LIBRARY_DIRECTORY/$NEW_LIBRARY_NAME"

    if [ ! -d $NEW_LIBRARY_DIRECTORY ]; then
        # Begin.
        echo "-- Creating new library: $NEW_LIBRARY_NAME"
        echo "-- Creating directory."

        # Create the directory.
        mkdir -p $NEW_LIBRARY_DIRECTORY

        # Create the CMake file.
        echo "-- Creating CMake file."
        echo "set(PROJECT_NAME \"$NEW_LIBRARY_NAME\")" >  "$NEW_LIBRARY_DIRECTORY/CMakeLists.txt"
        echo "library_template($NEW_LIBRARY_NAME)"     >> "$NEW_LIBRARY_DIRECTORY/CMakeLists.txt"

        # Create the hpp file.
        echo "-- Creating header file."
        echo "#pragma once"                  >  "$NEW_LIBRARY_PATH.hpp"
        echo ""                              >> "$NEW_LIBRARY_PATH.hpp"
        echo "class $NEW_LIBRARY_NAME {"     >> "$NEW_LIBRARY_PATH.hpp"
        echo "    public:"                   >> "$NEW_LIBRARY_PATH.hpp"
        echo "        $NEW_LIBRARY_NAME();"  >> "$NEW_LIBRARY_PATH.hpp"
        echo "        ~$NEW_LIBRARY_NAME();" >> "$NEW_LIBRARY_PATH.hpp"
        echo ""                              >> "$NEW_LIBRARY_PATH.hpp"
        echo "    private:"                  >> "$NEW_LIBRARY_PATH.hpp"
        echo ""                              >> "$NEW_LIBRARY_PATH.hpp"
        echo "};"                            >> "$NEW_LIBRARY_PATH.hpp"

        # Create the cpp file.
        echo "-- Creating main file."
        echo "#include \"$NEW_LIBRARY_NAME.hpp\""        > "$NEW_LIBRARY_PATH.cpp"
        echo ""                                         >> "$NEW_LIBRARY_PATH.cpp"
        echo "$NEW_LIBRARY_NAME::$NEW_LIBRARY_NAME(){"  >> "$NEW_LIBRARY_PATH.cpp"
        echo ""                                         >> "$NEW_LIBRARY_PATH.cpp"
        echo "}"                                        >> "$NEW_LIBRARY_PATH.cpp"
        echo ""                                         >> "$NEW_LIBRARY_PATH.cpp"
        echo "$NEW_LIBRARY_NAME::~$NEW_LIBRARY_NAME(){" >> "$NEW_LIBRARY_PATH.cpp"
        echo ""                                         >> "$NEW_LIBRARY_PATH.cpp"
        echo "}"                                        >> "$NEW_LIBRARY_PATH.cpp"

        # Create the test file:
        echo "-- Creating test file."                                   
        echo "#include <catch2/catch.hpp>"                             > "$NEW_LIBRARY_PATH""_test.cpp"
        echo "#include \"$NEW_LIBRARY_NAME.cpp\""                     >> "$NEW_LIBRARY_PATH""_test.cpp"
        echo ""                                                       >> "$NEW_LIBRARY_PATH""_test.cpp"
        echo "SCENARIO(\"create a new instance\") {"                  >> "$NEW_LIBRARY_PATH""_test.cpp"
        echo "    $NEW_LIBRARY_NAME my_object = $NEW_LIBRARY_NAME();" >> "$NEW_LIBRARY_PATH""_test.cpp"
        echo "    REQUIRE(true);"                                     >> "$NEW_LIBRARY_PATH""_test.cpp"
        echo "}"                                                      >> "$NEW_LIBRARY_PATH""_test.cpp"
        
        # Finished.
        echo "-- Finished!"
    else
        echo "Library folder already exists!"
    fi
fi
