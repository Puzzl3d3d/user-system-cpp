#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <sstream>
#include <cstdlib>

using namespace std;

typedef std::map<string, string> BasePairMap;

vector<string> customSplit(string str, char separator) {
    vector<string> strings;

    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {
        
        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }

    return strings;
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

string hashify(string str) {
    stringstream ss;

    const std::hash<std::string> hasher;
    const auto hashResult = hasher(str);

    ss << hashResult;

    return ss.str();
}

void login(string username) {
    cout << username << " successfully logged in!\n";
    
    system("pause");
}

int count(string s, char sep) {
  int count = 0;

  for (int i = 0; i < s.size(); i++)
    if (s[i] == sep) count++;

  return count;
}

int main()
{
    fstream usersFile;
    string username;

    cout << "Username: ";
    getline(cin, username); // Instead of cin >> username;, allows whitespaces (allegedly)

    usersFile.open("users.dat", fstream::in | fstream::out | fstream::app);
    if (usersFile.is_open()) {
        string line;
        BasePairMap users;
        while (getline(usersFile, line)) {
            vector<string> strings = customSplit(line, '=');

            users[strings[0]] = strings[1];
        }

        usersFile.close();

        for ( auto it = users.begin(); it != users.end(); ++it  )
        {
            string user = it->first;
            string hashWithSalt = it->second;

            string hash = hashWithSalt.substr(0, hashWithSalt.length() - 12);
            string salt = hashWithSalt.substr(hashWithSalt.length() - 12, 12);

            if (username == user) {
                string hashedInput;

                while (hashedInput != hash) {
                    string input;
                    cout << "\nEnter password: ";
                    getline(cin, input); // Instead of cin >> input;, allows whitespaces (allegedly)

                    string salted = input + salt;
                    string hashedInput = hashify(salted);

                    if (hash == hashedInput) {
                        cout << "Correct password!\n";

                        login(username);

                        return 1;
                    } else {
                        cout << "Incorrect password.\n";
                    }
                }
            }
        }

        // User does not exist

        string password;

        cout << "Enter a new password: ";
        getline(cin, password); // Instead of cin >> password;, allows whitespaces (allegedly)

        cout << password;

        srand((unsigned)time(NULL));
        string salt = random_string(12);

        string salted = password + salt;

        string saltedHash = hashify(salted);

        usersFile.open("users.dat", fstream::in | fstream::out | fstream::app);

        usersFile << username << "=" << saltedHash << salt <<"\n";

        usersFile.close();

        cout << "\nSuccessfully signed up new user\n";

        login(username);
    }

    return 0;
}