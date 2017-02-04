#ifndef COMPAREFILES_H
#define COMPAREFILES_H

#include <string>
#include <fstream>
#include <string.h>

/* return line of file that differs, zero if they are same */
int compareFiles(std::string& file1name, std::string file2name) {
    std::fstream file1(file1name), file2(file2name);
    char string1[256], string2[256];
    int j = 0;

    while (!file1.eof() && !file2.eof()) {
        file1.getline(string1, 256);
        file2.getline(string2, 256);
        while (strncmp("DEBUG", string1, 5) && !file1.eof())
            file1.getline(string1, 256);
        while (strncmp("DEBUG", string2, 5) && !file2.eof())
            file2.getline(string2, 256);
        j++;
        if (strcmp(string1, string2))
            return j;
    }
    while (strncmp("DEBUG", string1, 5) && !file1.eof())
        file1.getline(string1, 256);
    while (strncmp("DEBUG", string2, 5) && !file2.eof())
        file2.getline(string2, 256);
    if (!file1.eof() || !file2.eof())
        return j + 1;
    return 0;
}

#endif /* COMPAREFILES_H */
