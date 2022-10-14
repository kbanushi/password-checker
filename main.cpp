/****************************
 *
 * HEADER
 *
 * *** **********************/
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
using namespace std;

bool isFound(const vector<string>& wordVector, const string& password);
bool containsUpperCase(const string& password);
bool containsLowerCase(const string& password);
bool containsNumber(const string& password);
bool containsSpecialChar(const string& password);
bool checkIfLeaked(const string& password, const vector<string> &leakedPasswords);
bool checkIfEnglishWord(const string &password, const vector<string> &englishWords);
vector<string> createWordVector(const string& fileName);

const string rule_msg_1 = "The selected password must be at least eight characters long.";
const string rule_msg_2 = "The selected password must have at least one uppercase letter.";
const string rule_msg_3 = "The selected password must have at least one lowercase letter.";
const string rule_msg_4 = "The selected password must have at least one digit.";
const string rule_msg_5 = "The selected password must have at least one of special characters: [@,!,#,^,&,*,$]";
const string rule_msg_6 = "The selected password has been leaked.";
const string rule_msg_7 = "The selected password cannot contain a dictionary word.";
const string PASSWORD_REJECTED = "Password rejected!";
const string PASSWORD_ACCEPTED = "Password accepted!";


bool isFound(const vector<string>& wordVector, const string& password){
    int low,mid,high;
    low = 0;
    high = wordVector.size() - 1;
    mid = (low + high) / 2;

    while (low <= high){
        mid = (high + low) / 2;
        if (password > wordVector.at(mid)){
            low = mid + 1;
        }
        if (password < wordVector.at(mid)) {
            high = mid - 1;
        }
        if (password == wordVector.at(mid)){
            return true;
        }
    }
    return false;
}

bool containsUpperCase(const string& password){
    for (int i = 0; i < password.size(); i++){
        if (isupper(password.at(i)))
            return true;
    }
    return false;
}

bool containsLowerCase(const string& password){
    for (char x : password){
        if (islower(x))
            return true;
    }
    return false;
}

bool containsNumber(const string& password){
    for (char x : password){
        if (isdigit(x))
            return true;
    }
    return false;
}

bool containsSpecialChar(const string& password){
    for (char x : password){
        if (!isupper(x) && !islower(x) && !isdigit(x))
            return true;
    }
    return false;
}

bool isSpecialChar(const char& currLetter){
    if (currLetter == '!' || currLetter == '@' || currLetter == '#'|| currLetter == '$'|| currLetter == '%'
    || currLetter == '^'|| currLetter == '&' || currLetter == '*'){
        return true;
    }
    return false;
}

string findStrippedWord(const string& password){
    string strippedPassword;
    for(int i = 0; i < password.size(); i++){
        if (isupper(password.at(i))){
            strippedPassword += tolower(password.at(i));
        }
        else if (islower(password.at(i))){
            strippedPassword += password.at(i);
        }
        else if (isdigit(password.at(i))){
            strippedPassword += password.at(i);
        }
    }
    return strippedPassword;
}

bool checkIfLeaked(const string& password, const vector<string> &leakedPasswords){ //This will need to strip the password then
    string strippedPassword = findStrippedWord(password);

    if (isFound(leakedPasswords, strippedPassword)){
        return true;
    }

    return false;
}

bool checkIfEnglishWord(const string &password, const vector<string> &englishWords){ //This will need to strip password then// search
    string strippedPassword;
    for (char x : password){
        if (isupper(x)){
            strippedPassword += tolower(x);
        }
        else if (islower(x)){
            strippedPassword += x;
        }
    }
    return isFound(englishWords, strippedPassword);
}

vector<string> createWordVector(const string& fileName){
    ifstream wordFile(fileName);
    string tempString;
    vector<string> wordsVector;
    if(wordFile.is_open()){
        while(getline(wordFile,tempString)){
            wordsVector.push_back(findStrippedWord(tempString));
        }
    }
    wordFile.close();
    return wordsVector;
}
/****
 * Runs the main part of the program
 ****/
int run(const string &leaked_password_file, const string &english_word_file);

int run(const string &leaked_password_file, const string &english_word_file){
    bool passwordReject = false;
    string password;
    cout << "Enter password: ";
    cin >> password;
    vector<string> leakedPassVector = createWordVector(leaked_password_file);
    vector<string> englishWordsVector = createWordVector(english_word_file);

    if (password.size() < 8) {
        cout << rule_msg_1 << endl;
        passwordReject = true;
    }
    if (!containsUpperCase(password)) {
        cout << rule_msg_2 << endl;
        passwordReject = true;
    }
    if (!containsLowerCase(password)) {
        cout << rule_msg_3 << endl;
        passwordReject = true;
    }
    if (!containsNumber(password)) {
        cout << rule_msg_4 << endl;
        passwordReject = true;
    }
    if (!containsSpecialChar(password)) {
        cout << rule_msg_5 << endl;
        passwordReject = true;
    }

    if (passwordReject){
        cout << PASSWORD_REJECTED;
        return 0;
    }else {
        if (checkIfLeaked(password, leakedPassVector)) {
            cout << rule_msg_6 << endl;
            cout << PASSWORD_REJECTED;
            return 0;
        }
        if (checkIfEnglishWord(password, englishWordsVector)) {
            cout << rule_msg_7 << endl;
            cout << PASSWORD_REJECTED;
            return 0;
        }
    }
    cout << PASSWORD_ACCEPTED << endl;
    return 1;
}

int main() {
	/** Change the address of the files appropriatly based on your local machine.
	 * This main function will be replaced during testing. Do not add any line of code to the main function 
	 **/
	string leaked_password_file = "/Users/krenarbanushi/Documents/CS 141/Project 3/alleged-gmail-passwords.txt"; //change this based on the location of the file on your machine
	string english_word_file = "/Users/krenarbanushi/Documents/CS 141/Project 3/words_5.txt"; //change this based on the location of the file on your machine
	run(leaked_password_file, english_word_file);
	return 0;	
}
