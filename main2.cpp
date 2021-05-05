#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <math.h>
#define EXPONENT_LENTH 8
#define MANTISSA_LENTH 20
#define BITS_LENTH EXPONENT_LENTH+MANTISSA_LENTH+1
using namespace std;


class Processor {
private:
    std::bitset<BITS_LENTH> R[8];
public:
    Processor() {};
    void processing(string command, string A);
    std::bitset<BITS_LENTH> to_ieee754(double x);
    double from_ieee754(std::bitset<BITS_LENTH> &x);
    
    void finit() {
        for (int i=0; i<8; i++) {
            R[i] = 0;
        }
    }
    
    void down() {
        for (int i=7; i>0; i--) {
            R[i] = R[i-1];
        }
        R[0] = 0;
    }
    
    void up(int from=1) {
        for (int i=from; i<7; i++) {
            R[i] = R[i+1];
        }
        R[7] = 0;
    }
    
    void fild(double x) {
        down();
        R[0] = to_ieee754(x);
    }
    
    void fadd() {
        double d = from_ieee754(R[0]) + from_ieee754(R[1]);
        R[0] = to_ieee754(d);
        up();
    }
    
    void fdub() {
        down();
        R[0] = R[1];
    }
    
    void fdiv() {
        double d = from_ieee754(R[0]) / from_ieee754(R[1]);
        R[0] = to_ieee754(d);
        up();
    }

    void show() {
        cout << left;
        cout << setw(5);
        for (int i=0; i<8; i++) {
            //cout << R[i] << endl;
            cout << setw(5) << "R" + to_string(i) + ": ";
            for (int j=BITS_LENTH-1; j>=0; j--) {
                cout << R[i][j];
                if (j == BITS_LENTH-1 || j == MANTISSA_LENTH) {
                    cout << " ";
                }
            }
            double d = from_ieee754(R[i]);
            if (abs(d) > 0.000001) {
                cout << " == " << d;
            }
            cout << endl;
        }
        for (int j=0; j<BITS_LENTH+7; j++) {
    	    cout << "_";
        }
    	cout << endl << endl;
    }
};

void Processor::processing(string command, string A) {
    if (command == "finit") {
	    finit();
	} else if (command == "fild") {
	    char* p;
        double converted = strtod(A.c_str(), &p);
        if (*p || A == "") {
            throw invalid_argument("received inappropriate value");
        }
        else {
            fild(converted);
        }
        
	} else if (command == "fadd") {
	    fadd();
	} else if (command == "fdub") {
	     fdub();
	} else if (command == "fdiv") {
	     fdiv();
	} else {
	    throw invalid_argument("received inappropriate command");
	}
}

std::bitset<BITS_LENTH> Processor::to_ieee754(double x) {   //https://www.h-schmidt.net/FloatConverter/IEEE754.html
    if (x == 0) {
        return std::bitset<BITS_LENTH> (0);
    }
    std::bitset<MANTISSA_LENTH> m;
    string sign = x > 0 ? "0" : "1";
    x = abs(x);
    int slide = pow(2, EXPONENT_LENTH-1) - 1;
    int expo = -1;
    while (x >= 1) {x /= 2; expo++;}
    while (x < 0.5) {x *= 2; expo--;}
    std::bitset<EXPONENT_LENTH> exponent(slide + expo);
    
    for (int i=MANTISSA_LENTH; i>0; i--) {
        if (x > 0) {
            x *= 2;
            if (x >= 1) {  
                m[i] = 1;
                x -= 1;
            } else m[i] = 0;
        } else break;
    }
    return std::bitset<BITS_LENTH> (sign + exponent.to_string() + m.to_string());
}

double Processor::from_ieee754(std::bitset<BITS_LENTH> &x) {
    std::bitset<MANTISSA_LENTH> m;
    int sign = x[BITS_LENTH-1] == 0 ? 1 : -1;
    int e = 0;
    int expo_bit_rank = 1;
    for (int i=MANTISSA_LENTH; i<BITS_LENTH-1; i++) {
        if (x[i] == 1) {
            e += expo_bit_rank;
        }
        expo_bit_rank *= 2;
    }
    double mant_bit_rank = 1;
    double result = 1;
    for (int i=MANTISSA_LENTH; i>=0; i--) {
        if (x[i] == 1) {
            result += mant_bit_rank;
        }
        mant_bit_rank /= 2;
    }
    if (result >= 2) result -= 1;
    if (result == 0) return pow(2, e-127)*sign;
    return result*pow(2, e-127)*sign;
}

int main() {
    Processor P;
    string str;
	string words[10];
	int word = 0;
	ifstream file;
	file.open("instructions2.txt");
	while (!file.eof()) {
        while(getline(file, str)) {
            for (int k = 0; k < 10; ++k) {words[k] = "";}
            word = 0;
            for (int i = 0; i < str.size(); ++i) {
                if (isspace(str[i])) {
                    word += 1;
                } else {
                    words[word] += str[i];
                }
            }
            cout << ">>>  " << words[0] << " " << words[1] << endl;
            P.processing(words[0], words[1]);
            P.show();
        }
	}
	return 0;
}




