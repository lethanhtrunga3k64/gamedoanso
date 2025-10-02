#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to generate random 5-digit number
string generateSecretNumber() {
    srand(time(0));
    string secret = "";
    for (int i = 0; i < 5; i++) {
        secret += to_string(rand() % 10);
    }
    return secret;
}

// Function to check the guess
void checkGuess(const string& secret, const string& guess, int& correctPosition, int& correctNumberWrongPosition) {
    correctPosition = 0;
    correctNumberWrongPosition = 0;
    
    vector<bool> secretUsed(5, false);
    vector<bool> guessUsed(5, false);
    
    // First pass: check correct positions
    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret[i]) {
            correctPosition++;
            secretUsed[i] = true;
            guessUsed[i] = true;
        }
    }
    
    // Second pass: check correct numbers in wrong positions
    for (int i = 0; i < 5; i++) {
        if (!guessUsed[i]) {
            for (int j = 0; j < 5; j++) {
                if (!secretUsed[j] && guess[i] == secret[j]) {
                    correctNumberWrongPosition++;
                    secretUsed[j] = true;
                    break;
                }
            }
        }
    }
}

// Function to validate input
bool isValidGuess(const string& guess) {
    if (guess.length() != 5) return false;
    for (char c : guess) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

int main() {
    cout << "Content-type: text/html\n\n";
    cout << "<!DOCTYPE html>\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Number Guessing Game</title>\n";
    cout << "<style>\n";
    cout << "body { font-family: Arial, sans-serif; margin: 40px; background-color: #f0f0f0; }\n";
    cout << ".container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n";
    cout << ".input-group { margin: 20px 0; }\n";
    cout << "input[type='text'] { padding: 10px; font-size: 18px; width: 200px; text-align: center; }\n";
    cout << "button { padding: 10px 20px; font-size: 16px; background: #007bff; color: white; border: none; border-radius: 5px; cursor: pointer; }\n";
    cout << "button:hover { background: #0056b3; }\n";
    cout << ".result { margin: 10px 0; padding: 10px; background: #e9ecef; border-radius: 5px; }\n";
    cout << ".correct { color: green; font-weight: bold; }\n";
    cout << ".wrong-pos { color: orange; font-weight: bold; }\n";
    cout << ".history { margin-top: 20px; }\n";
    cout << ".attempt { padding: 5px; margin: 5px 0; background: #f8f9fa; border-left: 3px solid #007bff; }\n";
    cout << "</style>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div class='container'>\n";
    cout << "<h1>🎯 Number Guessing Game</h1>\n";
    cout << "<p>Đoán số có 5 chữ số (0-9). Mỗi chữ số có thể trùng lặp.</p>\n";
    
    // Get form data
    string queryString = getenv("QUERY_STRING");
    string guess = "";
    string secret = "";
    string message = "";
    
    // Parse query string
    if (!queryString.empty()) {
        size_t pos = queryString.find("guess=");
        if (pos != string::npos) {
            guess = queryString.substr(pos + 6, 5);
        }
        
        pos = queryString.find("secret=");
        if (pos != string::npos) {
            secret = queryString.substr(pos + 7, 5);
        }
    }
    
    // Initialize or get secret number
    if (secret.empty()) {
        secret = generateSecretNumber();
    }
    
    cout << "<form method='GET'>\n";
    cout << "<input type='hidden' name='secret' value='" << secret << "'>\n";
    cout << "<div class='input-group'>\n";
    cout << "<label for='guess'>Nhập số 5 chữ số của bạn:</label><br>\n";
    cout << "<input type='text' id='guess' name='guess' maxlength='5' pattern='[0-9]{5}' required>\n";
    cout << "</div>\n";
    cout << "<button type='submit'>Đoán số</button>\n";
    cout << "</form>\n";
    
    // Process the guess
    if (!guess.empty() && isValidGuess(guess)) {
        int correctPos, correctNumWrongPos;
        checkGuess(secret, guess, correctPos, correctNumWrongPos);
        
        cout << "<div class='result'>\n";
        cout << "<h3>Kết quả:</h3>\n";
        cout << "<p>Số bạn đoán: <strong>" << guess << "</strong></p>\n";
        cout << "<p class='correct'>Số đúng vị trí: " << correctPos << "</p>\n";
        cout << "<p class='wrong-pos'>Số đúng nhưng sai vị trí: " << correctNumWrongPos << "</p>\n";
        
        if (correctPos == 5) {
            cout << "<h2 style='color: green;'>🎉 Chúc mừng! Bạn đã đoán đúng số: " << secret << "</h2>\n";
            cout << "<p><a href='?'><button>Chơi lại</button></a></p>\n";
        }
        cout << "</div>\n";
        
        // Display guess history (simple version)
        cout << "<div class='history'>\n";
        cout << "<h3>Lịch sử đoán:</h3>\n";
        cout << "<div class='attempt'>\n";
        cout << "Lần này: " << guess << " - Đúng vị trí: " << correctPos << " - Sai vị trí: " << correctNumWrongPos << "\n";
        cout << "</div>\n";
        cout << "</div>\n";
        
    } else if (!guess.empty()) {
        cout << "<div style='color: red;'>❌ Vui lòng nhập đúng 5 chữ số (0-9)!</div>\n";
    }
    
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "</html>\n";
    
    return 0;
}