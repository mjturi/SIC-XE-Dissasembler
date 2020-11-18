#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <cctype>
#include<iomanip>

using namespace std;

string btoh (string bin) {
    //converts a binary string to a hex string by different cases for each 4 bits
    string s;
    for(int i = 0; i < bin.length(); i+=4){
        string cur_half = bin.substr(i, 4);
        if      (!cur_half.compare("0000")) s += "0";
        else if (!cur_half.compare("0001")) s += "1";
        else if (!cur_half.compare("0010")) s += "2";
        else if (!cur_half.compare("0011")) s += "3";
        else if (!cur_half.compare("0100")) s += "4";
        else if (!cur_half.compare("0101")) s += "5";
        else if (!cur_half.compare("0110")) s += "6";
        else if (!cur_half.compare("0111")) s += "7";
        else if (!cur_half.compare("1000")) s += "8";
        else if (!cur_half.compare("1001")) s += "9";
        else if (!cur_half.compare("1010")) s += "A";
        else if (!cur_half.compare("1011")) s += "B";
        else if (!cur_half.compare("1100")) s += "C";
        else if (!cur_half.compare("1101")) s += "D";
        else if (!cur_half.compare("1110")) s += "E";
        else if (!cur_half.compare("1111")) s += "F";
    }
    transform(s.begin(), s.end(),s.begin(), ::toupper);
    return s;
}

string hexToBin (string val) {
    string binary = ""; //converts hex number to binary by different cases
    for (int i = 0; i < val.length(); i++){
        switch(val[i]){
            case '0': binary.append ("0000"); break;
            case '1': binary.append ("0001"); break;
            case '2': binary.append ("0010"); break;
            case '3': binary.append ("0011"); break;
            case '4': binary.append ("0100"); break;
            case '5': binary.append ("0101"); break;
            case '6': binary.append ("0110"); break;
            case '7': binary.append ("0111"); break;
            case '8': binary.append ("1000"); break;
            case '9': binary.append ("1001"); break;
            case 'A': binary.append ("1010"); break;
            case 'B': binary.append ("1011"); break;
            case 'C': binary.append ("1100"); break;
            case 'D': binary.append ("1101"); break;
            case 'E': binary.append ("1110"); break;
            case 'F': binary.append ("1111"); break;
        }
    }
    return binary;
}

int hexToInt(string hexVal){
    //converts hex to decimal
    int convertedVal;
    stringstream ss;
    ss << hex << hexVal;
    ss >> convertedVal;
    return convertedVal;
}

string opCodeGenerator(string byte){
    //takes first 6 bits of instruction and appends "00" to getopcode
    string opCode;
    opCode = byte.substr(0,6) + "00";
    opCode = btoh(opCode);
    return opCode;
}

string lookUpTable[59][3] = {
//{OPCODE, MNEMONIC, FORMAT},
//Although this project doesn't require format 1 instructions, they were still included
        {"18","ADD","3/4"},
        {"58","ADDF","3/4"},
        {"90","ADDR","2"},
        {"40","AND","3/4"},
        {"B4","CLEAR","2"},
        {"28","COMP","3/4"},
        {"88","COMPF","3/4"},
        {"A0","COMPR","2"},
        {"24","DIV","3/4"},
        {"64","DIVF","3/4"},
        {"9C","DIVR","2"},
        {"C4","FIX","1"},
        {"C0","FLOAT","1"},
        {"F4","HIO","1"},
        {"3C","J","3/4"},
        {"30","JEQ","3/4"},
        {"34","JQT","3/4"},
        {"38","JLT","3/4"},
        {"48","JSUB","3/4"},
        {"00","LDA","3/4"},
        {"68","LDB","3/4"},
        {"50","LDCH","3/4"},
        {"70","LDF","3/4"},
        {"08","LDL","3/4"},
        {"6C","LDS","3/4"},
        {"74","LDT","3/4"},
        {"04","LDX","3/4"},
        {"D0","LPS","3/4"},
        {"20","MUL","3/4"},
        {"60","MULF","3/4"},
        {"98","MULR","2"},
        {"C8","NORM","1"},
        {"44","OR","3/4"},
        {"D8","RD","3/4"},
        {"AC","RMO","2"},
        {"4C","RSUB","3/4"},
        {"A4","SHIFTL","2"},
        {"A8","SHIFTR","2"},
        {"F0","SIO","1"},
        {"EC","SSK","3/4"},
        {"0C","STA","3/4"},
        {"78","STB","3/4"},
        {"54","STCH","3/4"},
        {"80","STF","3/4"},
        {"D4","STI","3/4"},
        {"14","STL","3/4"},
        {"7C","STS","3/4"},
        {"E8","SISW","3/4"},
        {"84","STT","3/4"},
        {"10","STX","3/4"},
        {"1C","SUB","3/4"},
        {"5C","SUBF","3/4"},
        {"94","SUBR","2"},
        {"B0","SVC","2"},
        {"E0","TD","3/4"},
        {"F8","TIO","1"},
        {"2C","TIX","3/4"},
        {"B8","TIXR","2"},
        {"DC","WD","3/4"}
};

string lookupMnemonic(string opCode){ //Requires specified opCode to retrieve Mnemonic
    int counter = 0;
    for (auto& rows: lookUpTable){
        if (*rows == opCode){ // If opcode matches return Mnemonic
            return lookUpTable[counter][1];
        }
        counter++;
    }
    return 0; //returns 0 if opCode doesn't match
}

string lookupFormat(string opCode){ //Requires specified opCode to retrieve Mnemonic
    int counter = 0;
    for (auto& rows: lookUpTable){
        if (*rows == opCode){ // If opcode matches return Mnemonic
            return lookUpTable[counter][2];
        }
        counter++;
    }
    return 0; //returns 0 if opCode doesn't match
}

vector<string> sortedBinaryInstructions(vector<string> textRecord, vector<string> littab) {
    vector<string> binaryInstructions; //will be used to hold all binary instructions
    vector<string> littabAddress;
    string halfBytePairs;
    string halfBytePairsToBeAppended;
    string halfBytePairsBinary;
    string formatTwoCheck;
    string literalBits;
    int addressCounter;

    int start;
    int end;
    int difference;

    for (int w = 0; w < littab.size(); w++){ //gets literals from literal table
        start = littab[w].find_first_of("'") + 1;
        end = littab[w].find_last_of("'");
        difference = end-start;
        littabAddress.push_back(littab[w].substr(start, difference));
    }


    for (int k = 0; k < textRecord.size(); k++){ //each iteration through k is a new text record
        for (int i = 0; i < textRecord[k].length(); i++){ //iterates through single text record
            if (textRecord[k][i] == 'T'){ //skips to bits with object code
                i += 9;
            }
            if (!littabAddress.empty()){ //isolates bits the size of next literal for later comparison
                literalBits = string(1, textRecord[k][i]);
                for (int x = 1; x < littabAddress[0].size(); x++){
                    literalBits += string(1, textRecord[k][i + x]);
                }

                if (literalBits == littabAddress[0]){ //checks to see if literal is in text record
                    i += littabAddress[0].size();
                    binaryInstructions.push_back(littabAddress[0]); //adds literal to instructions vector
                    littabAddress.erase(littabAddress.begin());
                }
            }
            //checks for text record title, if true skips to the start of the object code

            //initializing first 3 bits by converting to hex to binary, will be used to check length
            if (i < textRecord[k].length() - 2){
                halfBytePairs = string(1, textRecord[k][i]) + textRecord[k][i + 1] + textRecord[k][i + 2];
                halfBytePairsBinary = hexToBin(halfBytePairs);
                formatTwoCheck = opCodeGenerator(halfBytePairsBinary);

                //Check lookup table using opcode to identify if format 2
                if (lookupFormat(formatTwoCheck) == "2"){
                    //appending 4 bits since format 2 is 2 bytes long
                    halfBytePairsToBeAppended = string(1,textRecord[k][i + 3]);
                    halfBytePairsToBeAppended = hexToBin(halfBytePairsToBeAppended);
                    halfBytePairsBinary += halfBytePairsToBeAppended;
                    binaryInstructions.push_back(halfBytePairsBinary);
                    i += 3;
                }

                    //checks e bit to see if format 4 instruction
                else if(halfBytePairsBinary[11] == '0'){
                    //appends next 12 bits since format 3 instructions are 3 bytes long
                    halfBytePairsToBeAppended = string(1,textRecord[k][i + 3]) + textRecord[k][i + 4] + textRecord[k][i + 5];
                    halfBytePairsToBeAppended = hexToBin(halfBytePairsToBeAppended);
                    halfBytePairsBinary += halfBytePairsToBeAppended;
                    binaryInstructions.push_back(halfBytePairsBinary);
                    i += 5;
                }

                    //checks if format 4 instruction by checking if e bit is set to 1
                else if(halfBytePairsBinary[11] == '1'){
                    //appends 20 bits since format 4 instructions are 4 bytes long
                    halfBytePairsToBeAppended = string(1,textRecord[k][i + 3]) + textRecord[k][i + 4] + textRecord[k][i + 5] + textRecord[k][i + 6] + textRecord[k][i + 7];
                    halfBytePairsToBeAppended = hexToBin(halfBytePairsToBeAppended);
                    halfBytePairsBinary += halfBytePairsToBeAppended;
                    binaryInstructions.push_back(halfBytePairsBinary);
                    i += 7;
                }
            }
        }
    }
    return binaryInstructions;

};

vector<string> ta (vector<string> inst, vector<string> loc, vector<string> literals, vector<string> symbols, vector<string> mnes, string hrecord) {
    //this function calculates the "operand" (symbol, literal, register, or value) that is associated with each mnemonic
    vector<string> tas; //will be used to hold all "operands"
    tas.push_back("START"); //first one will always be START (according to testing file format)
    string base; //string for holding value of base register
    int lit_count = 0; //counter for lit table
    char hex_chars[14] = {'2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string ad; //string for holding addresses
    string other; //temp strings to aid with conversions from ints to strings
    string hold;
    string flgs;
    string x;
    string r1; //strings for format 2 instructions
    string r2;
    for (int i = 0; i < inst.size(); i++) { //main for loop. iterates through each instruction and determines the operand associated with it
        string cur = inst.at(i);
        string mne = mnes.at(i+1);
        mne.erase(remove(mne.begin(), mne.end(), '+'), mne.end());
        //These are the "special" cases: ones where there is a different format for getting the operand and for format 2 instructions
        if (mne == "RSUB"){
            tas.push_back("");
            continue;
        }
        else if (mne == "ADDR" || mne == "COMPR" || mne == "DIVR" || mne == "MULR" || mne == "RMO" || mne == "SUBR"){
            r1 = cur.substr(8,4);
            r2 = cur.substr(12,4);
            r1 = btoh(r1);
            r2 = btoh(r2);
            for(auto& x: r1)
                x = toupper(x);
            for(auto& x: r2)
                x = toupper(x);
            tas.push_back(r1 + ',' + r2);
            continue;
        }
        else if (mne == "CLEAR" || mne == "SVC"){
            r1 = cur.substr(8,4);
            r1 = btoh(r1);
            for(auto& x: r1)
                x = toupper(x);
            tas.push_back(r1);
            continue;
        }
        else if (mne == "SHIFTR" || mne == "SHIFTL"){
            r1 = cur.substr(8,4);
            r2 = cur.substr(12,4);
            r1 = btoh(r1);
            r2 = btoh(r2);
            stringstream ss;
            ss << hex << r2;
            int a;
            ss >> a;
            stringstream ss2;
            string inc = "1";
            ss2 << hex << inc;
            int y;
            ss2 >> y;
            a += y;
            stringstream ssfin;
            ssfin << dec << a;
            x = ssfin.str();
            for(auto& x: r1)
                x = toupper(x);
            tas.push_back(r1 + ',' + r2);
            continue;
        }
        else if (mne == "TIX"){
            stringstream ss;
            ss << hex << x;
            int a;
            ss >> a;
            stringstream ss2;
            string inc = "1";
            ss2 << hex << inc;
            int y;
            ss2 >> y;
            a += y;
            stringstream ssfin;
            ssfin << hex << a;
            x = ssfin.str();
        }
        else if (mne == "TIXR"){
            stringstream ss;
            ss << hex << x;
            int a;
            ss >> a;
            stringstream ss2;
            string inc = "1";
            ss2 << hex << inc;
            int y;
            ss2 >> y;
            a += y;
            stringstream ssfin;
            ssfin << hex << a;
            x = ssfin.str();
            r1 = cur.substr(8,4);
            r1 = btoh(r1);
            for(auto& x: r1)
                x = toupper(x);
            tas.push_back(r1);
            continue;
        }
        //checking to see if there is a literal at this location
        bool lit_flag = false;
        for (int i = 0; i < sizeof(hex_chars)/sizeof(hex_chars[0]); i++) {
            int found = cur.find(hex_chars[i]);
            if (found > cur.length())
                continue;
            else {
                lit_flag = true;
                break;
            }
        }
        if (lit_flag == true) {
            string hold = literals.at(lit_count);
            string lit = hold.substr(8, 6);
            lit.erase(remove_if(lit.begin(), lit.end(), ::isspace), lit.end());
            tas.push_back(lit);
            lit_count += 1;
        } else { // this is for regular format 3/4 instructions
            //each case parses through the flag bits and matches them with a prewritten case, then performs the TA
            //calculation based on them, checking for symbols as well when required
            string flgs = cur.substr(6, 6);
            string ad;
            string other;
            string hold;
            if (flgs == "110000"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                tas.push_back(ad);
            }
            else if (flgs == "110001"){
                ad = cur.substr(12,20);
                ad = btoh(ad);
                while (ad.size() < 6){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++) {
                    sym = symbols.at(i).substr(8, 6);
                    if (ad == sym) {
                        sym = symbols.at(i).substr(0, 6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym);
                        break;
                    }
                }
                string lit;
                for (int i = 0; i < literals.size(); i++){
                    lit = literals.at(i).substr(24,6);
                    if (ad == lit){
                        lit = literals.at(i).substr(8,6);
                        lit.erase(remove_if(lit.begin(), lit.end(), ::isspace), lit.end());
                        tas.push_back(lit);
                        break;
                    }
                }
            }
            else if (flgs == "110010"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = loc.at(i+2);
                if (ad.at(0) == 'F') {
                    while (ad.size() < 8) {
                        ad.insert(0, "F");
                    }
                }
                stringstream ss;
                ss << hex << ad;
                unsigned int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym);
                        break;
                    }
                }
                string lit;
                for (int i = 0; i < literals.size(); i++){
                    lit = literals.at(i).substr(26,4);
                    if (ad == lit){
                        lit = literals.at(i).substr(8,6);
                        lit.erase(remove_if(lit.begin(), lit.end(), ::isspace), lit.end());
                        tas.push_back(lit);
                        break;
                    }
                }

            }
            else if (flgs == "110100"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = base;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                tas.push_back(ad + ",X");
            }
            else if (flgs == "111000"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = x;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym + ",X");
                        break;
                    }
                }
            }
            else if (flgs == "111001"){
                ad = cur.substr(12,20);
                ad = btoh(ad);
                other = x;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 6){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(8,6);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym + ",X");
                        break;
                    }
                }
            }
            else if (flgs == "111010"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = x;
                hold = loc.at(i+2);
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                stringstream ss3;
                ss3 << hex << hold;
                int b;
                ss3 >> b;
                a = a + b + y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym + ",X");
                        break;
                    }

                }
            }
            else if (flgs == "111100"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = x;
                hold = base;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                stringstream ss3;
                ss3 << hex << hold;
                int b;
                ss3 >> b;
                a = a + b + y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back(sym + ",X");
                        break;
                    }

                }
            }
            else if (flgs == "100000"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                tas.push_back("@" + ad);
            }
            else if (flgs == "100001"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                while (ad.size() < 6){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(8,6);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("@" + sym);
                        break;
                    }
                }
            }
            else if (flgs == "100010"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = loc.at(i+2);
                if (ad.at(0) == 'F') {
                    while (ad.size() < 8) {
                        ad.insert(0, "F");
                    }
                }
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("@" + sym);
                        break;
                    }
                }
            }
            else if (flgs == "100100"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = base;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                for(auto& x: ad)
                    x = toupper(x);
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("@" + sym);
                        break;
                    }
                }
            }
            else if (flgs == "010000"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                tas.push_back("#" + ad);
            }
            else if (flgs == "010001"){
                ad = cur.substr(12,20);
                ad = btoh(ad);
                while (ad.size() < 6){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(8,6);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("#" + sym);
                        break;
                    }
                }
            }
            else if (flgs == "010010"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = loc.at(i+2);
                if (ad.at(0) == 'F') {
                    while (ad.size() < 8) {
                        ad.insert(0, "F");
                    }
                }
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("#" + sym);
                        break;
                    }
                }
            }
            else if (flgs == "010100"){
                ad = cur.substr(12,12);
                ad = btoh(ad);
                other = base;
                stringstream ss;
                ss << hex << ad;
                int a;
                ss >> a;
                stringstream ss2;
                ss2 << hex << other;
                int y;
                ss2 >> y;
                a += y;
                stringstream ssfin;
                ssfin << hex << a;
                ad = ssfin.str();
                while (ad.size() < 4){
                    ad.insert(0, "0");
                }
                if (mne == "LDB"){
                    base = ad;
                }
                if (mne == "LDX"){
                    x = ad;
                }
                for(auto& x: ad)
                    x = toupper(x);
                // check if address matches a symbol
                string sym;
                for(int i = 0; i < symbols.size(); i++){
                    sym = symbols.at(i).substr(10,4);
                    if (ad == sym){
                        sym = symbols.at(i).substr(0,6);
                        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
                        tas.push_back("#" + sym);
                        break;
                    }
                }
            }
        }
    }
    for (int i = inst.size()+ 1; i < loc.size(); i++){ //loop for adding symbol definitions at the end
        string new_tas;
        if (i == loc.size()-1){// if its the last label need to check hrecord and subtract the prog length from last loc
            string prog_length = hrecord.substr(15,4);
            string cur = loc.at(i);
            stringstream ss;
            ss << hex << prog_length;
            int a;
            ss >> a;
            stringstream ss2;
            ss2 << hex << cur;
            int y;
            ss2 >> y;
            a -= y;
            a = a/3;
            stringstream ssfin;
            ssfin << hex << a;
            new_tas = ssfin.str();
            tas.push_back(new_tas);
        }
        else {
            string next = loc.at(i+1);
            string cur = loc.at(i);
            stringstream ss;
            ss << hex << next;
            int a;
            ss >> a;
            stringstream ss2;
            ss2 << hex << cur;
            int y;
            ss2 >> y;
            a -= y;
            a = a/3;
            stringstream ssfin;
            ssfin << dec << a;
            new_tas = ssfin.str();
            tas.push_back(new_tas);
        }
    }
    return tas;
}

vector<string> mnemon (vector<string> inst, vector<string> loc){ //this function returns a vector of mnemonics in order based on the opcode
    vector<string> mnemons; //this will hold the mnemonics
    mnemons.push_back(""); //start of program
    char hex_chars[14] = {'2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < inst.size(); i++) { //main for loop, goes through all binary instructions and parses opcode to determine mnemonics
        string cur = inst.at(i);
        bool lit_flag = false;
        for (int i = 0; i < sizeof(hex_chars)/sizeof(hex_chars[0]); i++) { //loop to check for literal definitions
            int found = cur.find(hex_chars[i]);
            if (found > cur.length())
                continue;
            else {
                lit_flag = true;
                break;
            }
        }
        if (lit_flag == true) {
            //its a literal
            mnemons.push_back("*");
        } else { // this is for actual instructions, conducts lookups from predefined tables
            string op = cur.substr(0, 6) + "00";
            op = btoh(op);
            string form = lookupFormat(op);
            string mnem = lookupMnemonic(op);
            if (form.size() > 1) {
                if (cur.size() > 24)
                    form = form.at(2);
                else
                    form = form.at(0);
            }
            if (form == "4")
                mnem.insert(mnem.begin(), '+');
            mnemons.push_back(mnem);
        }
    }
    for (int i = inst.size(); i < loc.size()-1; i++){ //for symbol definitions at the end of the program
        mnemons.push_back("RESW");
    }
    return mnemons;
}

vector<string> labels (vector<string> loc, vector<string> symbols, string hrecord){ //this function will return a vector of "labels" if there is a symbol defined or used at the location
    vector<string> label;
    string pname = hrecord.substr(1,6); //first value will always be progname
    pname.erase(remove_if(pname.begin(), pname.end(), ::isspace), pname.end());
    label.push_back(pname);
    bool lab_flg = false;
    for (int i = 1; i < loc.size(); i++){ //loop to check if there is a symbol defined/used, if not, pushes empty string for new line
        for (int j = 0; j < symbols.size(); j++){
            if (loc.at(i) == symbols.at(j).substr(10,4)){
                string new_lab = symbols.at(j).substr(0,6);
                new_lab.erase(remove_if(new_lab.begin(), new_lab.end(), ::isspace), new_lab.end());
                label.push_back(new_lab);
                lab_flg = true;
            }
        }
        if (lab_flg == false)
            label.push_back("");
        else
            lab_flg = false;
    }
    return label;
}

vector<string> locctr (vector<string> inst, string hrecord, vector<string> symbols, vector<string> literals){ //this function returns a vector of locations for the locctr column
    vector<string> loc;
    string start = hrecord.substr(9,4); //first loc is always the start of the program
    loc.push_back(start);
    int lit_count = 0;
    char hex_chars[14] = {'2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i=0; i < inst.size(); i++){ //main loop to go through each instruction
        string cur = inst.at(i);
        bool lit_flag = false;
        for (int i=0; i < sizeof(hex_chars)/sizeof(hex_chars[0]); i++){ //check if there is a literal here
            int found = cur.find(hex_chars[i]);
            if (found > cur.length())
                continue;
            else{
                lit_flag = true;
                break;
            }
        }
        if (lit_flag == true){
            //its a literal, get first literal from vector and add length / 2 to loc
            string hold = literals.at(lit_count);
            string lit_size = hold.substr(19,1);
            stringstream ss;
            ss << hex << lit_size;
            int x;
            ss >> x;
            x = x/2;
            string prev = loc.at(i);
            stringstream ss2;
            ss2 << hex << prev;
            int y;
            ss2 >> y;
            x += y;
            stringstream ssfin;
            ssfin << hex << x;
            string new_loc = ssfin.str();
            for(auto& x: new_loc)
                x = toupper(x);
            while (new_loc.size() < 4){
                new_loc.insert(0, "0");
            }
            loc.push_back(new_loc);
            lit_count += 1;
        }
        else { // this is for actual instructions, not literals
            //determines type of instruction and adds the format size to the previous loc then pushes it into the vector
            string op = cur.substr(0,6) + "00";
            op = btoh(op);
            string form = lookupFormat(op);
            if (form.size() > 1){
                if (cur.size() > 24)
                    form = form.at(2);
                else
                    form = form.at(0);
            }
            string prev = loc.at(i);
            stringstream ss;
            ss << hex << prev;
            int x;
            ss >> x;
            stringstream ss2;
            ss2 << hex << form;
            int y;
            ss2 >> y;
            x += y;
            stringstream ssfin;
            ssfin << hex << x;
            string new_loc = ssfin.str();
            for(auto& x: new_loc)
                x = toupper(x);
            while (new_loc.size() < 4){
                new_loc.insert(0, "0");
            }
            loc.push_back(new_loc);
        }
    }
    bool sym_flag = false;
    for(int i=0; i < symbols.size(); i++){ // loc for last symbols
        string cur_sym = symbols.at(i).substr(10,4);
        if (sym_flag == true){
            loc.push_back(cur_sym);
        }
        if (loc.at(loc.size()-1) == cur_sym){
            sym_flag = true;
        }

    }
    loc.insert(loc.begin(), start);
    return loc;
}

vector<string> formatBinInstru(vector<string> binaryInstructions, vector<string> mne){
    vector<string> hexInstructions;
    mne.erase(mne.begin());
    int size = mne.size();
    hexInstructions.push_back("");

    for(int i = 0; i < size; i++){
        if (!binaryInstructions.empty()){
            if(mne.at(i) == "*"){ //skips star because there is no object code in that location
                hexInstructions.push_back(binaryInstructions.at(0));
                binaryInstructions.erase(binaryInstructions.begin());
            }
            else{
                //converts binary instruction to hex
                hexInstructions.push_back(btoh(binaryInstructions.at(0)));
                binaryInstructions.erase(binaryInstructions.begin());
            }
        }
        else{
            hexInstructions.push_back("");
        }
    }
    return hexInstructions;
}

vector<string> getRecord(ifstream& objectFile,string recordType){ //function to obtain header, text, modification, and end records separately
    vector<string> record;
    if (objectFile.is_open()) {
        string test;
        while (getline(objectFile, test)) {
            if (test.at(0) == 'H' && recordType == "H")
                record.push_back(test);
            else if (test.at(0) == 'T' && recordType == "T")
                record.push_back(test);
            else if (test.at(0) == 'M' && recordType == "M")
                record.push_back(test);
            else if (test.at(0) == 'E' && recordType == "E")
                record.push_back(test);
        }
    }
    return record;
}

vector<string> getSym(ifstream& symFile, string symType){ //function to pase the sym table and only store useful pieces based on their location
    vector<string> sym;
    if (symFile.is_open()) {
        string test;
        int count = 1;
        while (getline(symFile, test)) {
            if (count > 2) {
                if (!test.empty()) {
                    if (test.at(0) != '-' && symType == "S")
                        sym.push_back(test);
                } else {
                    getline(symFile, test);
                    getline(symFile, test);
                    count = -100;
                }
            } else if (count < 0 && symType == "L")
                sym.push_back(test);
            count++;
        }
        symFile.close();
    }
    return sym;
}

void writeToFile(vector<string> instruction,vector<string> locctr,vector<string> labels,vector<string> mnemonic,vector<string> targetAddress){
    vector<string> hexInstructions = formatBinInstru(instruction, mnemonic); //converts binary instruction to hex (object code)
    ofstream file;
    string end = labels[1]; //grabs label for end
    file.open("listing_file.txt");
    while (!(locctr.empty() || labels.empty() || mnemonic.empty() || targetAddress.empty() || hexInstructions.empty())) {
        if (mnemonic.at(0) == "*") {//checks a line ahead for literal declaration
            file<< setw(30) << "LTORG" << endl;
        }
        //Main part that writes listing file
        file << setw(4) << locctr.at(0) << setw(13) << labels.at(0) << setw(13) << mnemonic.at(0) << setw(13) << targetAddress.at(0)
             << setw(13) << hexInstructions.at(0) << endl;
        // Used to add BASE in listing file
        if (mnemonic.at(0) == "+LDB" || mnemonic.at(0) == "LDB"){
            file<< setw(30) << "BASE";
            if (targetAddress[0][0] == '#' || targetAddress[0][0] == '@'){//used to remove any prepended characters
                file<< setw(13) << targetAddress.at(0).substr(1,targetAddress.at(0).length()) << endl;
            }
            else
            {
                file<< setw(13) << targetAddress.at(0) << endl;
            }
        }
        locctr.erase(locctr.begin());
        labels.erase(labels.begin());
        mnemonic.erase(mnemonic.begin());
        targetAddress.erase(targetAddress.begin());
        hexInstructions.erase(hexInstructions.begin());
    }
    file << setw(30) << "END" << setw(13) << end;
    file.close();
}


int main(int argc, char **argv) {
    if (argc != 3){
        cout << "Please make sure to include the two input files: the object file and the symbol file";
        return 0;
    }

    ifstream object_file_one(argv[1]);
    ifstream object_file_two(argv[1]);
    ifstream object_file_three(argv[1]);
    ifstream object_file_four(argv[1]);
    ifstream sym_file_one(argv[2]);
    ifstream sym_file_two(argv[2]);

    vector<string> mrecord = getRecord(object_file_one, "M");
    vector<string> trecord  = getRecord(object_file_two, "T");
    vector<string> symbols = getSym(sym_file_one, "S");
    vector<string> literals = getSym(sym_file_two, "L");
    vector<string> hrecordvector = getRecord(object_file_three, "H");
    vector<string> erecordvector = getRecord(object_file_four, "E");
    string erecord = erecordvector[0];
    string hrecord = hrecordvector[0];

    vector<string> inst = sortedBinaryInstructions(trecord, literals); //used to get all instructions
    vector<string> loc = locctr(inst, hrecord, symbols, literals); //used to set the locctr column
    vector<string> lab = labels(loc, symbols, hrecord); //used to calculate all "labels" for symbols
    vector<string> mne = mnemon(inst, loc); //used to set the menemonics for each line
    vector<string> tas = ta(inst, loc, literals, symbols, mne, hrecord); //used to calculate the operands/tas for each instruction

    writeToFile(inst, loc, lab, mne, tas); //writes listing file

    object_file_one.close();
    object_file_two.close();
    object_file_three.close();
    object_file_four.close();
    sym_file_one.close();
    sym_file_two.close();

}
