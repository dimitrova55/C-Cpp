// Dilyana Dimitrova

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<bits/stdc++.h>

using namespace std;

/*
Problem definition:     S enc = n{S unit}
    "S enc" is the encoded result, "n" is the number of repetitions of "S unit".
    - n must be one of the following symbols: 0123456789abcdef
    - S unit can contain: 0123456789abcdefghijklmnopqrstuvwxyz.,!?@#$%^&*
    - Only the characters "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b, c, d, e, f" are allowed in front of the "{"

    Input:
        text file containing a number of encoded strings
    Output:
        decoded text file

    Example:
            input                           output
            3{hww}#                         hwwhwwhww#
            6{01b}5{x}                      01b01b01b01b01b01bxxxxx
            cws4{3h,}                       cws3h,3h,3h,3h,
            3{poc}l?p0{5{y@}}               pocpocpocl?p
            y{s9}3{*,}3{5*r}                ERROR: Invalid input

*/

int HexToDecimal (char c)
{
    /*
    This function receives a hexadecimal number and converts it to decimal.
    */
    switch(c)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
        default: return 16;     //if the char is not a hexadecimal number
    }
}

class Decode
{
    private:
        string str;     //the string to be multiplied
        int num;        //the number of times

    public:
        Decode(string s, int n);
        string MultiplyString ();
        void PrintString () const;
};

Decode::Decode(string s, int n): str(s), num(n){}

string Decode::MultiplyString ()
{
    if(num == 0)
    {
        str = "";
    }

    else
    {
        string s = str;
        for (int i=1; i<num; i++)
            str += s;
    }
    return str;
}

void Decode::PrintString () const
{
    cout << "str = " << str << endl;
}

/*
main idea: Decode the reverse string of each line. In the end reverse
the decoded version to get the desired output

ex: the reverse version of "a{0{d@}}4{p%}0{2f}" is "}f2{0}%p{4}}@d{0{a"
it begins with closing bracket (count the brackets) so take the substring from
the last closing bracket to the first opening one (in case there are multiple like here: "5{,#4}5{2{c{1{1}}}}")
then, check the char after the opening bracket:
    if it is one of [0,f], multiply the substring
    if it is Not one of [0,f], "ERROR. Invalid input" is assigned to the main string

the above operations continue until the closing brackets == opening brackets
if they equal, append the multiplied substring to the main one
if Not, continue to multiply the substring until they equal

if there are any left chars before the first closing bracket or after the last opening one,
    they are appended to the main string
if the string has no brackets, the chars are just appended to the main string

*/

int main()
{
    vector <string> encodedStr;         //array of strings - will hold all strings from "encoded.txt"
    vector <string> decodedStringLine;  //array of strings - will hold the decoded versions of the above

    // open the file, reverse every line of string
    // and put it into the array of strings
    ifstream ifile("encoded.txt");
    if(ifile)
    {
        string text{""};
		while (getline(ifile, text))
        {
		    reverse(text.begin(), text.end());
			encodedStr.push_back(text);
			//cout << text << endl;
		}
    }
    ifile.close();

    // main "for" loop iterates over every line of text
    for (int i = 0; i < encodedStr.size(); i++)
    {
        string subStr = "";         //substring for multiplying and so on
        string decodedStr = "";     //main string for construction
        int countBrackets = -1;     //variable for counting brackets
        //increment, if it is closing bracket
        //decrement, if it is opening bracket
        //(because i work with the reverse version of the string :) )

        // second "for" loop iterates over every char of the string (the string is reversed!!)
        for (int j=0; j < encodedStr[i].size(); j++)
        {
            if(encodedStr[i][j] == '}')    // "if" for closing bracket
            {
                if(countBrackets < 0) countBrackets = 0;
                ++countBrackets;
                Decode d(subStr, 1);    //if there are any chars before, they are multiplied by 1
                decodedStr = decodedStr + d.MultiplyString();   //and added to the main string
                subStr = "";        //the substring is cleaned so that it can take the next substring
                continue;
            }

            if(encodedStr[i][j] == '{')     //"if" for opening bracket
            {
                --countBrackets;
                ++j;
                int nTimes = HexToDecimal(encodedStr[i][j]);

                if (nTimes > 15)
                {
                    decodedStr = "ERROR: Invalid input";
                    break;

                }
                Decode d(subStr, nTimes);
                subStr = d.MultiplyString();

                //if closing brackets == opening brackets, countBrackets will be zero.
                //append the multiplied substring to the main one
                //if Not, I continue to multiply the substring until they equal
                if(countBrackets == 0)
                {
                    decodedStr = decodedStr + subStr;
                    subStr = "";
                }

                continue;
            } //end of "if" for opening bracket

            subStr = subStr + encodedStr[i][j];     //constructing the substring

        } //end of the second loop

        //if any chars are left they are appended
        //the main decoded string is reversed back to normal
        if(decodedStr != "ERROR: Invalid input")
            {
                decodedStr = decodedStr + subStr;
                reverse(decodedStr.begin(), decodedStr.end());

            }
        //if the input does not contain any brackets,
        //the string is reversed and assigned
        if(countBrackets < 0)
        {
            decodedStr = encodedStr[i];
            reverse(decodedStr.begin(), decodedStr.end());
        }

        //add the decoded version of the string to the array of decoded strings
        decodedStringLine.push_back(decodedStr);
    } // end of main "for" loop

    //create, open, insert the information into file, close the file
    ofstream ofile("decoded.txt");
    for (auto line = decodedStringLine.begin(); line < decodedStringLine.end(); line++)
    {
        //cout << *line << "\n";
        ofile << *line << endl;
    }
    ofile.close();

    return 0;
}
