#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <bitset>
#define BITS_LENTH 30
using namespace std;

class Processor {
private:
    std::bitset<BITS_LENTH> R1;
    std::bitset<BITS_LENTH> R2;
    int PC = 0, TC = 0;
public:
    Processor() {
        for (int i = 0; i < BITS_LENTH; ++i) {
		    R1[i] = rand() % 2;
		    R2[i] = rand() % 2;
        }
    };
    //void show_register(std::bitset<BITS_LENTH> R) {for (int i = 0; i < BITS_LENTH; ++i){cout << R[i];} cout << endl;}
    void command_processing(string command, string A, string B);
    
    void move(std::bitset<BITS_LENTH> *where, std::bitset<BITS_LENTH> what) {
        *where = what;
    }
    void inverse(std::bitset<BITS_LENTH> *where, std::bitset<BITS_LENTH> what) {
        *where = what.flip();
    }
    void add(std::bitset<BITS_LENTH> *where, std::bitset<BITS_LENTH> what) {
        *where = (*where).to_ulong() + what.to_ulong();
    }
    void com(std::bitset<BITS_LENTH> *where, std::bitset<BITS_LENTH> what) {
        inverse(where, what); 
        add(where, std::bitset<BITS_LENTH>(1));
    }

    void show_values() {cout << "R1 value: " << R1.to_ulong() << endl << "R2 value: " << R2.to_ulong() << endl;}
    void show_tic(string full_command, std::bitset<BITS_LENTH> *R_ptr) {++TC; cout << ">>>> " << full_command << endl; show(R_ptr);}
    void show_sign(std::bitset<BITS_LENTH> *R) {if ((*R)[BITS_LENTH-1] == 0) {cout << "+";} else {cout << "-";};}
    void show(std::bitset<BITS_LENTH> *R_ptr) {
        cout << left;
    	cout << setw(5) << "R1: " << R1 << endl;;
    	cout << setw(5) << "R2: " << R2 << endl;;
    	cout << setw(5) << "PS: "; show_sign(R_ptr); cout << endl; // PS - sign of the last result
    	cout << setw(5) << "PC: " << PC << endl; // PC - command counter
    	cout << setw(5) << "TC: " << TC << endl; // TC - ticker counter
    	cout << "_______________________________________________________________" << endl;
    }
};

void Processor::command_processing(string command, string A, string B) {
    ++PC;
    string full_command = command + " " + A + " " + B;
    std::bitset<BITS_LENTH> *where;
    std::bitset<BITS_LENTH> what;
    
    /* First processor tic for the command*/
	if (A == "R1" || A == "R1,") {
	    where = &R1;
	} else if ( A == "R2" || A == "R2,") {
	    where = &R2;
	} else {
	    throw invalid_argument("received inappropriate attribute");
	}
	if (B == "R1") {
	    what = R1;
	} else if (B == "R2") {
	    what = R2;
	} else {
	    what = std::stoi(B);
	}
	show_tic(full_command, where);

	/* Second processor tic for the command*/
	if (command == "mov") {
	    move(where, what);
	} else if (command == "inv") {
	    inverse(where, what);
	} else if (command == "add") {
	    add(where, what);
	} else if (command == "com") {
	    com(where, what);
	} else {
	    throw invalid_argument("received inappropriate command");
	}
	show_tic(full_command, where);
}


int main() {
    Processor P;
    string str;
	string words[3];
	int word = 0;
	ifstream file;
	file.open("instructions.txt");
	while (!file.eof()) {
        while(getline(file, str)) {
            for (int k = 0; k < 3; ++k) {words[k] = "";}
            word = 0;
            for (int i = 0; i < str.size(); ++i) {
                if (isspace(str[i])) {
                    word += 1;
                } else {
                    words[word] += str[i];
                }
            }
            P.command_processing(words[0], words[1], words[2]);
        }
	}
	return 0;
}
