// Dilyana Dimitrova
// ID: 202055501
// e-mail: dimitrova55@pusan.ac.kr

#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <vector>

using namespace std;

/*
 Problem definition:
     Determine the order of treatment for the patients in the Emergency Room.
     Five levels of urgency: Immediate > Emergency > Urgent > Semi-Urgent > Non-Urgent
     - If two patients level of urgency is the same, the older person has higher priority
     - If two patients level of urgency and age are the same, lexicographic ordering is applied to their IDs.
     Input:
        text file containing patients information
     Output:
        text file containing patients information in an ordered way according to the level of emergency
 */

// converts the type of emergency into level of emergency, like on a scale
int DetermineEmerencyLevel (string str)
{
    /*
    This function receives a string (the level of emergency as a string) and converts it to a number.
    */
    if(!str.compare("IMMEDIATE")) return 5;
    if(!str.compare("EMERGENCY")) return 4;
    if(!str.compare("URGENT")) return 3;
    if(!str.compare("SEMI-URGENT")) return 2;
    if(!str.compare("NON-URGENT")) return 1;
    return 0;
}

//prints the priority queue
template<typename T> void print_queue (T& q, ofstream& file)
{
    while(!q.empty())
    {
        q.top().PrintPatient(file);
        q.pop();
    }
}

class EmergencyRoom
{
    friend bool operator>(const EmergencyRoom& p1, const EmergencyRoom& p2);        //predefining ">" operator as a friend function
private:
    string name;                    //patient's name
    int age;                        //patient's age
    string emergencyType;           //patient's type of emergency

public:
    EmergencyRoom(string n = "", int age = 0, string emT = "");     //constructor
    void PrintPatient(ofstream& file) const;
};

EmergencyRoom::EmergencyRoom(string n, int a, string emT):name(n), age(a), emergencyType(emT) {}

void EmergencyRoom::PrintPatient(ofstream& file) const
{
    //cout << name << "," << age << "," << emergencyType << endl;
    file << name << "," << age << "," << emergencyType << endl;
}

//predefining the ">" operator
bool operator>(const EmergencyRoom& p1, const EmergencyRoom& p2)
{
    int emL1 = DetermineEmerencyLevel(p1.emergencyType);
    int emL2 = DetermineEmerencyLevel(p2.emergencyType);

    if(emL1 > emL2) return 1;
    else if(emL1 == emL2)
    {
        if(p1.age > p2.age) return 1;
        else if(p1.age == p2.age)
        {
            if((p1.name).compare(p2.name) < 0) return 1;
            else return 0;
        }
    }
    return 0;
}


int main()
{
    /*
    EmergencyRoom patient1("rdsiyuknom", 35, "SEMI-URGENT");
    EmergencyRoom patient2("fvmzqhostn", 81, "IMMEDIATE");
    EmergencyRoom patient3("zavpskhoef", 35, "SEMI-URGENT");

    if(patient1 > patient2)
        cout << "patient1 is with higher priority." << endl;
    else
        cout << "patient2 is with higher priority." << endl;

    if(patient1 > patient3)
        cout << "patient1 is with higher priority." << endl;
    else
        cout << "patient3 is with higher priority." << endl;
    patient1.PrintPatient();

    */

    for(int i=1; i<=100; i++)
    {
        string fileNum = "";
        string fileName = "";
        if(i>=1 && i<=9)
            fileNum = "0000" + to_string(i);
        else if(i>=10 && i<=99)
            fileNum = "000" + to_string(i);
        else
            fileNum = "00" + to_string(i);

        //fileName = ".\\PA3_eval_data\\input_" + fileNum + ".txt";
        //cout << fileName << endl;
        fileName = ".\\DIR_INPUT_DATA\\input_" + fileNum + ".txt";


        auto cmp = [](EmergencyRoom p1, EmergencyRoom p2) { return p2 > p1; };                  //custom comparator
        priority_queue<EmergencyRoom, vector<EmergencyRoom>, decltype(cmp)> patientQueue(cmp);  //creating queue

        ifstream ifile(fileName);                         //opening file
        if(ifile)
        {
            string text{""};
            while (getline(ifile, text))                   //reading from file
            {
                //cout << text << endl;
                string name = "", emT = "", num = "";
                int age = 0;

                stringstream ss;
                ss << text;                            //gets the information from the file

                //split the information
                getline(ss >> ws, name, ',');
                getline(ss >> ws, num, ',');
                getline(ss >> ws, emT);
                age = stoi(num);                    //converting the string containing the age into number
                //cout << name << " " << age << " " << emT << '\n';
                EmergencyRoom patient(name, age, emT);                  //creating the object
                //patient.PrintPatient();
                patientQueue.push(patient);                             //pushing the object into queue

            }
        }
        ifile.close();

        //fileName = ".\\PA3_eval_data\\output_" + fileNum + ".txt";
        fileName = ".\\DIR_INPUT_DATA\\output_" + fileNum + ".txt";

        ofstream ofile(fileName);                   //creating the output file
        print_queue(patientQueue, ofile);           //printing the queue into the file
        ofile.close();

    } //end of "for" loop
    return 0;
}
