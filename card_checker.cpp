// ONLINE CARD CHECKING SYSTEM
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>

using namespace std;

// 1. ABSTRACT BASE CLASS (Abstraction)
class IdentityDocument {
protected:
    string rawNumber;
    vector<int> digits;

    void parseString() {
        digits.clear();
        for (char ch : rawNumber) {
            if (isdigit(ch)) {
                digits.push_back(ch - '0');
            }
        }
    }

public:
    // Pass by const reference for efficiency
    IdentityDocument(const string& num) : rawNumber(num) {
        parseString();
    }

    // Virtual destructor for safe dynamic cleanups
    virtual ~IdentityDocument() = default;

    // Pure Virtual Functions 
    virtual bool isValid() const = 0;
    virtual string getType() const = 0;
    virtual void printDetails() const = 0;
};

// 2. ENHANCED CNIC CLASS (Inheritance)
class CNIC : public IdentityDocument {
public:
    CNIC(const string& num) : IdentityDocument(num) {}

    bool isValid() const override {
        // CNICs must have exactly 13 digits
        return digits.size() == 13;
    }

    string getType() const override {
        return "CNIC (National Identity Card)";
    }

    // Business logic parsing embedded metadata
    string getProvince() const {
        if (!isValid()) return "Unknown";
        switch (digits[0]) {
            case 1: return "Khyber Pakhtunkhwa (KPK)";
            case 2: return "FATA";
            case 3: return "Punjab";
            case 4: return "Sindh";
            case 5: return "Balochistan";
            case 6: return "Islamabad (ICT)";
            case 7: return "Gilgit-Baltistan";
            default: return "Unknown / Overseas";
        }
    }

    string getGender() const {
        if (!isValid()) return "Unknown";
        // The last digit determines gender: Odd = Male, Even = Female
        return (digits.back() % 2 != 0) ? "Male" : "Female";
    }

    void printDetails() const override {
        cout << "\n--- CNIC Information ---" << endl;
        cout << "Document Type: " << getType() << endl;
        cout << "Province:      " << getProvince() << endl;
        cout << "Gender:        " << getGender() << endl;
    }
};

// 3. ENHANCED BANK CARD CLASS (Inheritance)
class BankCard : public IdentityDocument {
public:
    BankCard(const string& num) : IdentityDocument(num) {}

    // Luhn Algorithm (Mod 10 Checksum validation)
    bool isValid() const override {
        if (digits.size() != 16) return false;

        int sum = 0;
        bool doubleCurrentDigit = false;

        // Traverse digits from right to left
        for (int i = digits.size() - 1; i >= 0; i--) {
            int currentDigit = digits[i];

            if (doubleCurrentDigit) {
                currentDigit *= 2;
                if (currentDigit > 9) {
                    currentDigit -= 9; // Equivalent to adding the two digits of the product
                }
            }

            sum += currentDigit;
            doubleCurrentDigit = !doubleCurrentDigit; // Alternating flag
        }

        return (sum % 10 == 0);
    }

    string getType() const override {
        if (digits.empty()) return "INVALID CARD";
        
        switch (digits[0]) {
            case 4: return "VISA CARD";
            case 5: return "MASTERCARD / DEBIT CARD";
            case 6: return "DISCOVER / ATM CARD";
            default: return "UNKNOWN NETWORKING CARD";
        }
    }

    void printDetails() const override {
        cout << "\n--- Bank Card Information ---" << endl;
        cout << "Network Type:   " << getType() << endl;
        cout << "Luhn Checksum:  " << (isValid() ? "PASSED (Valid Card)" : "FAILED (Fake/Typo Card)") << endl;
    }
};

// MAIN FUNCTION
int main() {
    string inputNumber;
    cout << "Enter Document/Card Number (Hyphens and spaces allowed): ";
    
    // Fixed line: cin >> ws flushes out hidden buffer junk so getline works correctly
    if (!(cin >> ws)) return 0;
    getline(cin, inputNumber); 
    
    int digitCount = 0;
    for (char ch : inputNumber) {
        if (isdigit(ch)) digitCount++;
    }

    unique_ptr<IdentityDocument> doc = nullptr;

    if (digitCount == 13) {
        doc = make_unique<CNIC>(inputNumber);
    } else if (digitCount == 16) {
        doc = make_unique<BankCard>(inputNumber);
    }

    // Runtime Polymorphism Execution with Null-Pointer Protection
    if (doc != nullptr && doc->isValid()) {
        doc->printDetails();
    } else {
        cout << "\nError: INVALID NUMBER OR FORMAT DETECTED." << endl;
        if (doc != nullptr) {
             cout << "Parsed Type attempted: " << doc->getType() << endl;
        } else {
             cout << "Reason: The input length does not match a CNIC (13 digits) or a Bank Card (16 digits)." << endl;
        }
    }

    return 0;
}
