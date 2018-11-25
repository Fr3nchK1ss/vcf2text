/**
 * @author Ludovic A.
 * Parse a vcf file to text
 * Under GPL 3
 */

#include "vcard.h"

#include <iostream>
#include <algorithm>
#include <regex>
#include <vector>

using namespace std;

VCard::VCard()
{
    descriptions =
    {{"VERSION",       "vCard version"        ,""},
    {"FN",            "Name represented"      ,""},
    {"N",             "Name"                  ,""},
    {"NICKNAME",      "Nickname"              ,""},
    {"PHOTO",         "Photo uri"             ,""},
    {"BDAY",          "Birthday"              ,""},
    {"GENDER",        "Gender"                ,""},
    {"ADR",           "Post address is"       ,""},
    {"TEL",           "Telephone numbers are" ,""},
    {"EMAIL",         "Email address is"      ,""},
    {"IMPP",          "Instant messaging URI is",""},
    {"TITLE",         "Job is"                ,""},
    {"ROLE",          "Role played"           ,""},
    {"MEMBER",        "Member"                ,""},
    {"CATEGORIES",    "#tags"                 ,""},
    {"NOTE",          "Note"                  ,""},
    {"REV",           "VCard revision"        ,""},
    {"UID",           "Unique identifier of the vCard",""},
    {"URL",           "URL link"              ,""},
    {"KIND",         "This vCard is of kind" ,""}};

}


/**
 *@brief Print the vCard in a special format for phone contacts
 *
 */
string VCard::print_ContactFormat()
{
    string fn{""};
    string tel{""};
    string adr{""};
    string note{""};
    string email{""};

    //Build the string
    for (auto &tup : descriptions)
    {
        if ( get<0>(tup) == FN && !get<2>(tup).empty())
            fn = get<2>(tup);
        else if ( get<0>(tup) == TEL && !get<2>(tup).empty())
            tel = get<2>(tup);
        else if ( get<0>(tup) == ADR && !get<2>(tup).empty())
            adr = get<2>(tup);
        else if ( get<0>(tup) == NOTE && !get<2>(tup).empty())
            note = get<2>(tup);
        else if ( get<0>(tup) == EMAIL && !get<2>(tup).empty())
            email = get<2>(tup);

    }

    return fn+" "+tel+" "+adr+" "+note+" "+email;
}


/**
 *@brief Print the vCard in a format resembling the RFC one.
 *
 */
string VCard::print_RFCFormat()
{
    string result;
    //Build the string
    for (auto &tup : descriptions)
    {
        if(!get<2>(tup).empty())
        {
            result += get<0>(tup) + " (" + get<1>(tup) + ") "+ get<2>(tup) + "\n";
        }
    }

    return result;
}


/**
 *@brief Parse the line looking for RFC properties
 */
void VCard::parse(string vc_line)
{
    regex rxKEY;
    regex rxKEY_PARAMS{";(.*):"};//example "TEL;TYPE=home:"
    regex rxVALUES{":(.*)$"};//example ":5 moutain street;Phoenix;US"
    string delimiter = ";";//Value delimiter as defined by RFC
    smatch match;
    string matchStr; // to store a match as a string

    for (auto &tup : descriptions)
    {
        /*
         * A line has the following format:
         * KEY;KEY_PARAMS=value:VALUE1;VALUE2
         */

        rxKEY = "^" + get<0>(tup) +".*";

        if ( regex_match(vc_line, rxKEY) )
        {
            //cout << vc_line << "\t match with rxKEY" << endl; //DEBUG

            //Single out the key parameters
            //regex_search(vc_line, match, rxKEY_PARAMS);
            //cout << "key params: " << match[1] << endl;

            //Single out the text value of the key, space separated
            regex_search(vc_line, match, rxVALUES);

            string matchStr = string(match[1]).append(delimiter);
            size_t pos = 0;

            while ((pos = matchStr.find(delimiter)) != string::npos)
            {
                //store key values as space separated items
                get<2>(tup) += (matchStr.substr(0, pos))+" ";
                matchStr.erase(0, pos + delimiter.length());
            }

        }
    }
}


void VCard::reset()
{
    for (auto &tup : descriptions)
    {
        get<2>(tup) = "";
    }
}
