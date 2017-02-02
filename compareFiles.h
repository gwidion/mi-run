#include <string>
#include <fstream>
#include <string.h>

/* return line of file that differs, zero if they are same */
int compareFiles( std::string& file1name, std::string file2name ) {
    std::fstream file1( file1name ), file2( file2name );
    char string1[256], string2[256];
    int j = 0;

    while(!file1.eof())
    {
        file1.getline(string1,256);
        file2.getline(string2,256);
        j++;
        if(strcmp(string1,string2) != 0)
        {
            return j;
        }
    }
    return 0;
}