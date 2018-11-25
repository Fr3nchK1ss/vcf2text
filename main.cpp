/**
 * @author Ludovic A.
 * Parse a vcf file to human readable text
 * Under GPL 3
 */

#include <iostream>
#include <chrono>
#include <unistd.h>

#include "main.h"

using namespace std;

void printHelp()
{
    cout <<
    "Options:\n"
    " -o output_file [optional: default output is 'parsed.txt']\n"
    " -s [optional: write only to stdout, supersedes -o]\n"
    " -c special output formatting for a Contacts list\n"
    " -h print this help\n\n"
    "Example:\n"
    ">vcf2text backup2017.vcf\n"
    ">vcf2text backup2018.vcf -co backup2018.txt\n"
    ">ls\n"
    "backup2018.txt\tparsed.txt\n\n";

}

int main(int argc, char* argv[])
{
    int option;
    string inputFile = "";
    string outputFile = "parsed.txt";
    bool contact = false;
    bool toStdout = false;

    //Test for at least an input file
    if (argc < 2 || access( argv[1], F_OK ) == -1 )
    {
        cout << "No vcf file to parse\n" << endl;
        printHelp();
        return EXIT_SUCCESS;
    }
    else
        inputFile = argv[1];


    while ( (option = getopt(argc, argv, "o:sch")) != -1 )
    {
        switch ( option ) {
            case 'o':
                //cout << "output file: " << optarg << endl;
                outputFile = optarg;
                break;
            case 's':
                toStdout = true;
                break;
            case 'c':
                contact = true;
                break;
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
        }
    }


    //Warn user that the parsing starts
    cout << "Parsing: " << inputFile;
    if (contact)
        cout << " as a Contact list.";
    cout << endl << endl;

    VcfFile vcf(inputFile);
    if (toStdout)
        vcf.toStdout(contact);
    else
        vcf.toTextFile(outputFile, contact);

    return EXIT_SUCCESS;
}
