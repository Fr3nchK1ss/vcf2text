#ifndef VCARD_H
#define VCARD_H

#include <string>
#include <tuple>
#include <vector>

using namespace std;

static const string BEGIN = "BEGIN";              //1 with value "VCARD"
static const string END = "END";                  //1 with value "VCARD" 1
    static const string VCARD = "VCARD";
static const string SOURCE = "SOURCE";            //* with value type URI or others
static const string KIND = "KIND";                //*1 assume value = individual if absent
    static const string individual= "individual"; //must be recognized
    static const string group= "group";           //must be recognized
    static const string org= "org";               //must be recognized
    static const string location= "location";     //must be recognized
    //an x-name MAY be implemented
    //a iana token MAY be implemented
static const string XML = "XML";                  //*, must be preserved when propagating
//Identification properties
static const string FN = "FN";                    //1* with text value
static const string N = "N";                      //*1 with text value with ; and ,
static const string NICKNAME = "NICKNAME";        //* with text value with ,
static const string PHOTO = "PHOTO";              //* a single URI
static const string BDAY = "BDAY";                //*1 date-and-or-time or text
static const string ANNIVERSARY = "ANNIVERSARY";  //*1 same
static const string GENDER = "GENDER";            //*1 M-ale/F/O-ther/N-one/U-nknown
/*
 * Delivery Addressing properties
 * * text separated with ; and , which structure is:
 * post office box;extended address;street address;locality;region;postal code;country
 * ; separators must exist even with missing component
 * accept PREF GEO TZ LABEL
 */
static const string ADR = "ADR";
/*
 * Communications properties
 * can include the PREF and TYPE param (text, voice[default], fax, cell, video, pager, textphone)
 * TYPE:text; TYPE:voice; or TYPE = "text,voice" or TYPE = "voice, home"
 */
static const string TEL = "TEL";
static const string EMAIL = "EMAIL";              //* text
static const string IMPP = "IMPP";                //* a single URI
static const string LANG = "LANG";                //* a single language-tag value
//Geographical properties
static const string TZ = "TZ";                    //* a single text value
static const string GEO = "GEO";                  //* a single geo URI
//Organizational properties
static const string TITLE = "TITLE";              //* a single text value
static const string ROLE = "ROLE";                //* a single text value
static const string LOGO = "LOGO";                //* a single text value
static const string ORG = "ORG";                  //* a text value with ;
static const string MEMBER = "MEMBER";            //* a single URI, only if KIND = "group"
/*
 *TYPE param = "contact" / "acquaintance" / "friend" / "met" / "co-worker"
 * / "colleague" / "co-resident" / "neighbor" / "child" / "parent" / "sibling"
 * / "spouse" / "kin" / "muse" / "crush" / "date" / "sweetheart" / "me" / "agent"
 * / "emergency"
 */
static const string RELATED = "RELATED";
//Explanatory properties
static const string CATEGORIES = "CATEGORIES";    //* tags comma separated
static const string NOTE = "NOTE";                //* a single text value
static const string PRODID = "PRODID";            //*1 a single text value
static const string REV = "REV";                  //*1 a timestamp
static const string SOUND = "SOUND";              //* a single URI
static const string UID = "UID";                  //*1 a single URI / free form text
static const string CLIENTPIDMAP = "CLIENTPIDMAP";//*
static const string URL = "URL";                  //* a single URI value
static const string VERSION = "VERSION";          //1, may be absent in vCard < v4
//Security properties
static const string KEY = "KEY";                  //* a single URI
//Calendar properties
static const string FBURL = "FBURL";              //* a single URI
static const string CALADRURI = "CALADRURI";      //* a single URI
static const string CALURI = "CALURI";            //* a single URI


/**
 * The RFC 6350 defines the vCard data format for representing and exchanging
 * a variety of information about individuals and other entities. This RFC
 * obsoletes RFCs 2425, 2426, 4770 and updates RFC 2739
 */
class VCard
{
    //Not complete list of properties descriptions (see above for complete list)
    vector<tuple<string, string, string>> descriptions;

    public:
        VCard();
        void parse(string vc_line);
        string print_RFCFormat();
        string print_ContactFormat();
        void reset();

};

#endif // VCARD_H
