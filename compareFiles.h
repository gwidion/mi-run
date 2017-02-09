#ifndef COMPAREFILES_H
#define COMPAREFILES_H

#include <string>
#include <fstream>
#include <string.h>

/* return line of file that differs, zero if they are same */
int compareFiles(std::string & outputFile, std::string & referenceFile) {
    std::fstream file1(outputFile), file2(referenceFile);
    std::string buffer1, buffer2;
    std::string buffer;
    size_t position;
    int j = 0;

    while (!file1.eof()) {
        getline(file1, buffer);
        position = buffer.find("DEBUG");
        buffer1 += buffer.substr(0, position);
    }
    while (!file2.eof()) {
        getline(file2, buffer);
        position = buffer.find("DEBUG");
        buffer2 += buffer.substr(0, position);
    }
    if (buffer1.compare(buffer2))
        return j;
    return 0;
}

#endif /* COMPAREFILES_H */
