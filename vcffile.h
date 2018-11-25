/**
 * @author Ludovic A.
 * Parse a vcf file to text
 * Under GPL 3
 */

#ifndef VCFFILE_H
#define VCFFILE_H

#include <string>

#include "vcard.h"

using namespace std;

class VcfFile
{
        string originFile;
        void parse(bool contact, string outputFile="");

    public:
        VcfFile(string filename);
        void toStdout(bool contact);
        void toTextFile(string outputFile, bool contact = false);

};

#endif // VCFFILE_H
