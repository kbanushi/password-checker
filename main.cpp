#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
using namespace std;

//Function prototypes
int run(const string &leaked_password_file, const string &english_word_file);
bool isFound(const vector<string>& wordVector, const string& password); //Binary Search
bool checkIfLeaked(const string& password, const vector<string> &leakedPasswords); //Check if password was leaked
bool checkIfEnglishWord(const string &password, const vector<string> &englishWords); //Check if password is in dictionary
vector<string> createWordVector(const string& fileName); //Creates vectors for the files

const string rule_msg_1 = "The selected password must be at least eight characters long.";
const string rule_msg_2 = "The selected password must have at least one uppercase letter.";
const string rule_msg_3 = "The selected password must have at least one lowercase letter.";
const string rule_msg_4 = "The selected password must have at least one digit.";
const string rule_msg_5 = "The selected password must have at least one of special characters: [@,!,#,^,&,*,$]";
const string rule_msg_6 = "The selected password has been leaked.";
const string rule_msg_7 = "The selected password cannot contain a dictionary word.";
const string PASSWORD_REJECTED = "Password rejected!"; //CONST variable used for outputs to ensure no typos
const string PASSWORD_ACCEPTED = "Password accepted!"; //CONST variable used for output to ensure no typo

//This function is a binary search implementation to search the given vector for the password.  If it is found the function
// will return true, if not it will return false.
//The inputs of the function are a vector that is passed by reference to save space, and a string.
bool isFound(const vector<string>& wordVector, const string& password){
    int low,mid,high;
    low = 0;
    high = wordVector.size() - 1;
    //While loop that will continue until the value is found, or until the pointers pass each other.
    while (low <= high){
        mid = (high + low) / 2; //Mid is the middlemost index of the vector
        if (password > wordVector.at(mid)){
            low = mid + 1; //This adjusts the low index if the password is in the upper half
        }
        if (password < wordVector.at(mid)) {
            high = mid - 1; //Similar purpose as above
        }
        if (password == wordVector.at(mid)){
            return true; //Return true if there was a match
        }
    }
    return false; //Return false if there is no match
}


//This function is used to take in a string input and then strip the special characters it.  It will return the stripped string.
//It takes a string as an input, which is a const because the input is never changed
string findStrippedWord(const string& password){
    string strippedPassword;
    //We iterate through every character
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
    return strippedPassword; //Finally we return the stripped password
}


//This function will check if the user's input password has been leaked by stripping the user's password with the
// findStrippedWord function, and tests if there is a match against the leaked passwords using the isFound function
//This function takes in a string and vector as inputs and outputs true or false if the password was leaked or not
bool checkIfLeaked(const string& password, const vector<string> &leakedPasswords){
    string strippedPassword = findStrippedWord(password);
    if (isFound(leakedPasswords, strippedPassword)){
        return true; //Return true if found
    }
    return false; //Return false if not found
}


//This function will check if the user's input password contains a dictionary word in it.  We do not use the findStrippedWord
// function because we also need to strip the numbers from the password as well.
//This function takes in a string and a vector as inputs, and output true or false if the password was found in the dictionary
bool checkIfEnglishWord(const string &password, const vector<string> &englishWords){
    string strippedPassword;
    for (char x : password){
        if (isupper(x)){
            strippedPassword += tolower(x);
        }
        else if (islower(x)){
            strippedPassword += x;
        }
    }
    return isFound(englishWords, strippedPassword); //Return boolean result if it was found
}


//This function will create a vector with elements of every line in a text file.
//This function requires a string input as the file name, and will return a vector.
vector<string> createWordVector(const string& fileName){
    ifstream wordFile(fileName);
    string tempString; //Used to read and append line to vector
    vector<string> wordsVector;
    if(wordFile.is_open()){ //Check if the file has been opened first
        while(getline(wordFile,tempString)){ //While there are still lines to read
            wordsVector.push_back(findStrippedWord(tempString)); //Add line to vector
        }
    }
    wordFile.close(); //Close file
    return wordsVector; //Once finished the function will return the filled vector
}


//This function is used to do the inital 5 tests of the program. It will take in the password then check each character to see
// what type char it is.
//This function takes in a string, and 4 boolean variables by reference.  If the function comes across the correct type
// character it will change the appropriate boolean variable to true (signifying it was found)
//This function has no return
void initialTests(const string& password, bool& containsUpper, bool& containsLower, bool& containsNumber, bool&
containsSpecialChar){
    for (char x : password){
        if (isupper(x))
            containsUpper = true;
        else if (islower(x))
            containsLower = true;
        else if (isdigit(x))
            containsNumber = true;
        else {
            containsSpecialChar = true;
        }
    }
}


//This function serves as our main function.  It prompts the user for their desired password, and proceeds to test it using the
// above functions.  It is where most of the variables of the program are declared too.
//This function takes in two strings as inputs, they act as the file locations of the leaked passwords and dictionary
//This function returns an int, which is used to end the function early if necessary
int run(const string &leaked_password_file, const string &english_word_file){
    bool passwordReject = false, containsUpper, containsLower, containsNumber, containsSpecialChar;
    string password;
    cout << "Enter password: ";
    cin >> password;

    //This performs the initial tests
    initialTests(password,containsUpper,containsLower,containsNumber,containsSpecialChar);
    //Check the first 5 rules, if these all pass then we can continue testing the password.
    if (password.size() < 8) {
        cout << rule_msg_1 << endl;
        passwordReject = true;
    }
    if (!containsUpper) {
        cout << rule_msg_2 << endl;
        passwordReject = true;
    }
    if (!containsLower) {
        cout << rule_msg_3 << endl;
        passwordReject = true;
    }
    if (!containsNumber) {
        cout << rule_msg_4 << endl;
        passwordReject = true;
    }
    if (!containsSpecialChar) {
        cout << rule_msg_5 << endl;
        passwordReject = true;
    }
    //If the password failed the initial tests we can inform the user and end the function
    if (passwordReject){
        cout << PASSWORD_REJECTED;
        return 0;
    }else {
        //If the password passed the tests, then we can check the files
        vector<string> leakedPassVector = createWordVector(leaked_password_file);
        vector<string> englishWordsVector = createWordVector(english_word_file);
        //Test if the password was leaked
        if (checkIfLeaked(password, leakedPassVector)) {
            cout << rule_msg_6 << endl;
            cout << PASSWORD_REJECTED;
            return 0;
        }
        //If the password was NOT leaked, we can check if it contained an english word
        if (checkIfEnglishWord(password, englishWordsVector)) {
            cout << rule_msg_7 << endl;
            cout << PASSWORD_REJECTED;
            return 0;
        }
    }
    //If the password passes all the tests we can inform the user it was accepted.
    cout << PASSWORD_ACCEPTED << endl;
    return 1;
} //Return is integer type, it is used to end the function.


//Main is used to store strings directing us to the file locations. It then executes the run function.
int main() {
	/** Change the address of the files appropriatly based on your local machine.
	 * This main function will be replaced during testing. Do not add any line of code to the main function 
	 **/
	string leaked_password_file = "/Users/krenarbanushi/Documents/CS 141/Project 3/alleged-gmail-passwords.txt"; //change this based on the location of the file on your machine
	string english_word_file = "/Users/krenarbanushi/Documents/CS 141/Project 3/words_5.txt"; //change this based on the location of the file on your machine
	run(leaked_password_file, english_word_file);
	return 0;	
}
