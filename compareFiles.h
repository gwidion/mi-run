#ifndef COMPAREFILES_H
#define COMPAREFILES_H

#include <string>
#include <fstream>
#include <string.h>

/* returns text indicating where is difference or empty string if there is not */
std::string compareFiles(std::string & file1name, std::string & file2name) {
    std::fstream file1(file1name), file2(file2name);
    std::string buffer, buffer1, buffer2;
    size_t position;

    while (!file1.eof()) {
        getline(file1, buffer);
        position = buffer.find("DEBUG");
        buffer1 += buffer.substr(0, position);
        if (position == std::string::npos)
            buffer1 += " ";
    }
    while (!file2.eof()) {
        getline(file2, buffer);
        position = buffer.find("DEBUG");
        buffer2 += buffer.substr(0, position);
        if (position == std::string::npos)
            buffer2 += " ";
    }

    int comparison = -1;
    if (buffer1.length() < buffer2.length())
        comparison = buffer1.length() - 1;
    else if (buffer2.length() < buffer1.length())
        comparison = buffer2.length() - 1;
    for (size_t i = 0; i < buffer1.length(); i++) {
        if (buffer1[i] != buffer2[i]) {
            comparison = i;
            break;
        }
    }
    if (comparison < 0)
        return "";
    return std::string("difference from position ") + std::to_string(comparison + 1) + ": '" + buffer1.substr(comparison, 30) + "' vs '" + buffer2.substr(comparison, 30) + "'";
}

#endif /* COMPAREFILES_H */
