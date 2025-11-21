#pragma once

#include <exception>
#include <string>

class NotImplementedException : public std::exception {
public:
    // Construct with given error message:
    NotImplementedException(
        const char *error = "Functionality not yet implemented!") {
        errorMessage = error;
    }

    // Provided for compatibility with std::exception.
   [[nodiscard]] const char *what() const noexcept { return errorMessage.c_str(); }

private:
    std::string errorMessage;
};


class NoteOutOfRangeException : public std::exception {
public:
    // Construct with given error message:
    NoteOutOfRangeException(
        const char *error = "MIDI note must be between 0-127") {
        errorMessage = error;
    }

    // Provided for compatibility with std::exception.
    [[nodiscard]] const char *what() const noexcept { return errorMessage.c_str(); }

private:
    std::string errorMessage;
};
