#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <map>

#include <jb/jb.h>
#include <jb/dir.h>
#include <jb/textfile.h>

#include "convert.h"

using namespace std;

enum ConvertMode {
    C_MEMO,
    C_NUMBER
};

// Convert Shift-JIS to UTF-8
// string SJISConv(string buf)
// {
    // char cbuf[buf.length() + 1];
    // strcpy(cbuf, buf.c_str());
    // wxEncodingConverter conv;
    // char tmp[10000];
    // string outBuf;
    // char* inTmp = cbuf;
    // char* outTmp = tmp;

    // bool canconv = conv.Init(wxFONTENCODING_CP932, wxFONTENCODING_UTF8);

    // if (!canconv) {
        // return "";
    // }

    // //set initial length of in and out string
    // conv.Convert(inTmp, outTmp);

    // outBuf.assign(tmp);

    // return outBuf;
// }

void split(jb::String input, jb::String& notes, jb::String& tempo)
{
    int bar = input.find_first_of("|");

    if (bar != -1) {
        notes += input.substr(0, bar);
        tempo += input.substr(bar);
    }
    else
        notes += input;

    while (notes[notes.length() - 1] == ' ')
        notes = notes.substr(0, notes.length() - 1);

    return;
}

int convert_to_jub(int* tt, jb::String notes,
                   jb::String tempo, ConvertMode mode)
{
    int nl = notes.length();
    int tl = tempo.length();
    //int nn[20];
    map<jb::String, int> nn;
    //memset(nn, 0, sizeof(nn));
    int count = 0;
    int i = 0;

    while (i < nl) {
        if (notes[i] == ' ') {
            i++;
            continue;
        }
        if (notes.substr(i, 1) != "□" 
                && notes.substr(i, 1) != " "
                && notes.substr(i, 1) != "0")
        {
            nn[notes.substr(i, 1)] += 1 << count;
        }
        i++;
        count++;
    }

    int now = 0;
    int times = 0;
    i = 0;

    while (i < tl) {
        count = 0;

        if (tempo[i] != '|') {
            i++;
        }
        else if (tempo[i] == '|') {
            i++;
            //int t[10];
            jb::String t[20];

            while (tempo[i] != '|') {
                t[count] = tempo.substr(i, 1);
                count++;
                i++;
            }

            i++;
            int ti = 1000 / count;

            for (int j = 0; j < count; j++) {
                if (t[j] != "－" && t[j] != "0" && t[j] != "-")
                    tt[now] = nn[t[j]];

                now += ti;
            }

            times++;
            now = 1000 * times;
        }
    }

    return now;
}

// inline string Iconv_SJIS(bool isSJIS, string input)
// {
    // if (isSJIS)
        // input = SJISConv(input);

    // return input;
// }

void convert_sheet()
{
    wxDir* songs = new wxDir("convert");

    if (!songs->is_opened()) {
        // wxMessageBox(_("convert doesn't exist."), _("Failed"));
        return;
    }

    //cout << "Encoding with:\n"
         //<< "(1) Shift-JIS\n"
         //<< "(2) UTF-8\n"
         //<< "(3) number sheet" << endl;
    //int encoding;
    //cin >> encoding;
    //bool isSJIS = encoding == 1 ? true : false;
    bool isSJIS = false;

    wxString filename;
    bool cont = songs->get_first(&filename, "*.txt", wxDIR_FILES);
    ConvertMode mode;
    while (cont) {
        cout << "converting " << filename.to_std_string()
             << " ..." << endl;
        wxTextFile memo, jub;
        memo.open("convert/" + filename);
        jub.create("convert/"
                   + filename.substr(0, filename.length() - 4)
                   + ".jub");
        wxString input;

        input = memo.first();

        while (!memo.is_eof()) {
            if (input == "#memo" || input == "#memo1") {
                mode = C_MEMO;
                jub.add("#start#");
                break;
            }
            else if (input == "#numbersheet") {
                mode = C_NUMBER;
                jub.add("#start#");
                break;
            }
            else {
                jub.add(input);
            }

            input = memo.next();
        }

        int now = 0;
        int count;

        while (!memo.is_eof()) {
            input = memo.next();
            count = 0;
            wxString notes, tempo;

            if (!input.empty() && (input[0] > '9' || input[0] < '0')) {
                if (input[0] == 't') {
                    wxString tt;
                    tt << now << " ";
                    //cout << input << endl;
                    jub.add(tt + input);
                }
                else {
                    split(input, notes, tempo);
                    count++;

                    while (count < 4 && !memo.is_eof()) {
                        input = memo.next();

                        if (!input.empty() &&
                                (input[0] > '9' || input[0] < '0')) {
                            split(input, notes, tempo);
                            count++;
                        }
                    }

                    int notePosition[4010];
                    memset(notePosition, 0, sizeof(notePosition));
                    //
                    int interval
                    = convert_to_jub(notePosition, notes, tempo, mode);

                    //
                    for (int i = 0; i <= interval; i++) {
                        if (notePosition[i] != 0) {
                            wxString tt;
                            tt << i + now << " " << notePosition[i];
                            jub.add(tt);
                            //fout << i + now << " " << tt[i] << endl;
                        }
                    }

                    now += interval;
                    //cout << notes << endl;
                    //cout << tempo << endl;
                }
            }
        }

        memo.close();
        jub.write();
        jub.close();
        cont = songs->get_next(&filename);
    }

    cout << "done." << endl;

    return;
}

