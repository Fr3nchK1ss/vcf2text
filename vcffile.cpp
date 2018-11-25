/**
 * @author Ludovic A.
 * Parse a vcf file to text
 * Under GPL 3
 */

#include "vcffile.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>

using namespace std;


/**
 *@brief    Get a line properly even from Windows or Mac files
 *          Code from different users from StackO.
 */
std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

/**
 * A VcfFile may contain one or more vCard(s)
 * The input file is read from the file system
 * and parsed in vCards.
 */
VcfFile::VcfFile(string filename):
    originFile(filename)
{}


/**
 *@brief Parse an entire file where we expect to find
 *       successive vCards.
 */
void VcfFile::parse(bool contact, string outputFile)
{
    ifstream vcf(originFile);
    ofstream out;

    if (!outputFile.empty())
    {
        out.open(outputFile);
        if (!out)
            cout << "Output file error" << endl;
    }//else, print to standard output = outputFile CAN be empty

    if (!vcf)
    {
        cout << "Input file error" << endl;
    }
    else
    {
        string line = "";
        string word ="";
        regex beginOfVCard{"^"+BEGIN+".*"+VCARD};
        regex endOfVCard{"^"+END+".*"+VCARD};
        VCard* vc = new VCard();
        int cardCount = 0;

        while ( safeGetline(vcf, line) )
        {

            //Look for a vCard BEGIN
            if ( regex_match(line, beginOfVCard) )
            {
                cardCount++;

                //Fill in the different fields of the new vCard until END:VCARD is reached.
                while (safeGetline(vcf, line) && !regex_match(line, endOfVCard))
                {
                    if (line.empty())
                        continue;//skip blanks
                    /*
                     * _Line unfolding_, from RFC:
                     * "Individual lines within vCard are delimited by the [RFC5322] line break,
                     * which is a CRLF sequence (U+000D followed by U+000A).
                     * A logical line MAY be continued on the next physical line anywhere between
                     * two characters by inserting a CRLF immediately followed by a single white-
                     * space character (space (U+0020) or tab (U+0009)). This CRLF+space is removed
                     * when processing the content-type.
                     */

                    string nextLine = "";

                    //cout << "peek: '" << char(vcf.peek()) << "'" << endl;
                    for (char nextChar = vcf.peek(); nextChar == ' ' || nextChar == '\t';)
                    {//nextLine is to be unfolded!

                        safeGetline(vcf, nextLine);
                        //Remove leading spaces or tabs
                        while(nextLine[0] == ' ' || nextLine[0] == '\t')
                        {
                            //erase leading space or tabs
                            nextLine.erase(0,1);  //cout << "erase 1 space" << endl;
                        }

                        //Unfold nextLine, merging it with current line
                        line += " " + nextLine;
                        nextChar = vcf.peek();
                    }//Lines are unfold

                    //cout << "line: " << line << endl;
                    //Now let the vcard handles its own parsing
                    vc->parse(line);
                }

                //Time to print that card!
                if ( outputFile.empty() )
                {
                    if (contact)
                        cout << vc->print_ContactFormat() << endl;
                    else
                    {
                        cout << "== " << cardCount << ". ==" << endl;
                        cout << vc->print_RFCFormat();
                        cout << endl;
                    }
                }
                else
                {
                    //Write to a file
                    if (contact)
                        out << vc->print_ContactFormat() << "\n";
                    else
                    {
                        out << vc->print_RFCFormat();
                        out << "\n";
                    }
                }
                vc->reset();
            }//End of current vCard parsing

        }//End of file parsing

        if(out)
            out.close();
        delete vc;
    }
}


/**
 *@brief    Print to standard output for debug
 */
void VcfFile::toStdout(bool contact)
{
    //by default parse() prints to stdout
    parse(contact);
}


/**
 *@brief    Output vcf data to a text file
 */
void VcfFile::toTextFile(string outputFile, bool contact)
{
    //Print the result of the parsing to 'outputFile'
    parse(contact, outputFile);
    cout << "Parsing done to " << outputFile << endl << endl;
}
