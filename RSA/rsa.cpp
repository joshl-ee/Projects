#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

//Function prototypes
void decrypt(string input, string output, long int prime1, long int prime2, long int key);
void encrypt(string output, long int n, string message);
long int modExp(long int base, string exp, long int mod);
long int calcDKey(long int p, long int q);
long int gcd(long int p, long int q);
string to_bin(int exp);

int main(int argc, char* argv[]) {
    //Check to see if two primes are provided
    if (argc != 3) {
        throw invalid_argument( "Please enter 2 primes" );
    }
    //Store given primes as long ints
    long int p = stol(argv[1]);
    long int q = stol(argv[2]);
    //Calculate decryption key based on given primes
    long int dKey = calcDKey(p, q);

    while (1) {

        cout << "What would you like to do?: " << endl;

        //Take in command and corresponding arguments
        string command;
        string arg1, arg2, arg3;
        cin >> command;
        if (!command.compare("exit") || !command.compare("EXIT")) return 0;
        if (!command.compare("decrypt") || !command.compare("DECRYPT")) {
            cin >> arg1 >> arg2;
            decrypt(arg1, arg2, p, q, dKey);
        }
        if (!command.compare("encrypt") || !command.compare("ENCRYPT")) {
            cin >> arg1 >> arg2;
            long int n = stol(arg2);
            //Check if given n value is valid
            if (n < 27) {
                throw invalid_argument( "n value is too small" );
            }
            getline(cin, arg3);
            //Based on the encrypt input format given in the HW4 Bytes page, there will always be an extra space
            //attached to the beginning of the message because of how getline works. This next line removes the space.
            arg3.erase(0, 1);
            encrypt(arg1, n, arg3);
        }
    }
}
//Decrypt function
void decrypt (string input, string output, long int p, long int q, long int key) {
    ifstream in(input);
    ofstream out(output);
    long int C;
    long int M;
    int x = 1 + (log((p*q)/27)/log(100));
    string Mstring;
    string letter;
    //std::string message is the decrypted C.
    string message;
    //Read in each C value one-by-one
    while (in >> C) {
        //Calculate M via modular exponentiation
        M = modExp(C, to_bin(key), p*q);
        //Store M as a string
        Mstring = to_string(M);
        //Add leading zeros if necessary
        while (Mstring.size() < 2*x) {
            Mstring.insert(Mstring.begin(), '0');
        }
        //Loop through each 2 char block of M (one letter of the message)
        for (int i = 0; i < Mstring.size()/2; i++) {
            //Set letter as a 2 char block of M
            letter[0] = Mstring[i*2];
            letter[1] = Mstring[i*2+1];
            //If letter is a space ("00")
            if  (letter[0] == '0' && letter[1] == '0') {
                message.append(" ");
                continue;
            }
            //If letter is a-z
            message.push_back(char(stoi(letter) + 96));
        }
        //Write decrypted C to output file
        out << message;
        //Reset message for next C
        message.clear();
    }

}

//Encrypt function
void encrypt (string output, long int n, string message) {
    int e = 65537;
    int x = 1 + (log(n/27)/log(100));

    ofstream out(output);

    int messageSize;
    //Set messageSize to how many C's should be made
    if (message.size() % x == 0) {
        messageSize = message.size()/x;
    }
    else messageSize = (message.size()/x) + 1;
    //Iterate through each x-sized block of message and write corresponding C to output file
    for (int i = 0; i < messageSize; i++) {
        //std::string m is the message converted to numbers, where a-z -> 01-26
        string m;
        for (int j = 0; j < x; j++) {
            //If letter is a space, null character, or outside of the range of the message, add 00 to m
            if (message[j] == ' ' || message[j] == '\0' || j > message.size() - 1) {
                m.append("00");
                continue;
            }
            //Add leading zeros to letters a-i
            if (int(message[j]) - 96 < 10) m.append("0");
            //Add numerical value of letter to m
            m.append(to_string(int(message[j]) - 96));
        }
        //Delete part of message that is already converted
        message.erase(0,x);
        //Calculate C via modular exponentiation
        long int M = stol(m);
        out << modExp(M, to_bin(e), n) << " ";

    }
}
//Modular exponentiation function. Exponent must converted to binary and given as a string via to_bin function.
long int modExp(long int base, string exp, long int mod) {
    long int y = 1;
    long int power = base % mod;
    //Modular exponentiation math. Iterates through std::string exp from 0-exp.size()-1 since to_bin makes string backwards.
    for (int i = 0; i < exp.size(); i++) {
        if (exp[i] == '1') {
            y = (y * power) % mod;
        }
        power = (power * power) % mod;
    }
    return y;
}
//Convert number to binary. This results in a binary string in backwards order (lowest power is at front of string).
string to_bin(int exp) {
    string binExp;
    while (exp > 0) {
        binExp.append(to_string(exp % 2));
        exp = exp/2;
    }
    return binExp;
}
//Calculate Decryption Key function using Euclidean Algorithm
long int calcDKey(long int p, long int q) {
    //Calculates lowest common multiple of p and q using GCD function
    long int lcm = abs((p-1)*(q-1))/gcd(p,q);
    //Check if lcm is sufficiently large
    if (lcm < 65538) {
        throw invalid_argument( "LCM of P and Q is too small" );
    }

    long int t = 1;
    long int old_t = 0;
    long int r = 65537;
    long int old_r = lcm;
    long int temp, quotient;
    //Euclidean algorithm
    while(r != 0) {
        quotient = old_r / r;
        temp = r;
        r = old_r - quotient * r;
        old_r = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }
    if (old_t < 0) {
        old_t = old_t + (p-1)*(q-1);
    }
    //Return decryption key
    return old_t;

}
//Calculate GCD function using simplified Euclidean Algorithm
long int gcd(long int p, long int q) {
    int temp;
    //Set p to the larger of p and q. Set q to the smaller of p and q.
    if (p < q) {
        temp = p;
        p = q;
        q = temp;
    }

    int quotient;
    //simplified Euclidean Algorithm
    while (q != 0) {
        quotient = p/q;
        temp = q;
        q = p - quotient * q;
        p = temp;
    }
    //Return GCD
    return p;
}