#include <cstdlib>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>


using namespace std;


int getProcessId(string process_name)
{

    string procDir = "/proc/";

    DIR* dir = opendir(procDir.c_str());

    if(dir == NULL)
    {
        perror("opendir");
    }

    struct dirent* dirEntry;

    regex r(process_name);
    smatch regex_matches;

    while((dirEntry = readdir(dir)) != NULL)
    {
        cout << dirEntry->d_name << endl;
        int procId;
        if(!(procId = atoi(dirEntry->d_name))) // If the name of the file is not a number continue
            continue; 
        
        stringstream cmdLineFileName;
        cmdLineFileName << procDir << procId << "/cmdline"; // "/proc/id/cmdline"
        ifstream cmdLineFile(cmdLineFileName.str());
        if(cmdLineFile)
        {
            string firstLine;
            getline(cmdLineFile, firstLine);

            if(regex_search(firstLine, regex_matches, r))
            {
                cout << "found pid of " << process_name << " to be: " << procId << "\n";
                return procId;
            }
        }
    }

    return 0;
}


int main(int argc, char* argv[])
{
    string process_name;
    int argCounter = 1;
    while(argCounter < argc)
    {
        if(strcmp(argv[argCounter], "--pname") == 0)
        {
            if(argCounter + 1 < argc ) // Check if there is another arg after pname
                process_name = argv[argCounter + 1];
            else
             cout << "missing process_name\n";
        }
        argCounter++;
    }

    int pid = getProcessId(process_name);

    return 0;
}