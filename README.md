# Online Card Checking System

A C++ console program that checks whether a number is a Pakistani **CNIC** or a **bank card** and shows details about it. Built to demonstrate OOP concepts: abstraction, inheritance, and runtime polymorphism.

## Features

- Accepts input with hyphens or spaces (e.g. `35202-1234567-1` or `4111 1111 1111 1111`)
- Detects the document type by digit count:
  - **13 digits** → CNIC
  - **16 digits** → Bank card
- **CNIC:** shows province (from the first digit) and gender (odd last digit = male, even = female)
- **Bank card:** validates with the **Luhn algorithm** and shows the network (Visa, Mastercard, Discover) from the first digit

## OOP Concepts Used

| Concept | Where |
|---|---|
| Abstract class | `IdentityDocument` (pure virtual `isValid`, `getType`, `printDetails`) |
| Inheritance | `CNIC` and `BankCard` extend `IdentityDocument` |
| Polymorphism | Objects handled through `unique_ptr<IdentityDocument>` |
| Encapsulation | `protected` data and a helper parser inside the base class |

## Build and Run

Requires a C++14 compiler or newer.

```bash
g++ -std=c++14 card_checker.cpp -o card_checker
./card_checker
```

## Example

```
Enter Document/Card Number (Hyphens and spaces allowed): 4111 1111 1111 1111

--- Bank Card Information ---
Network Type:   VISA CARD
Luhn Checksum:  PASSED (Valid Card)
```

## Limitations

- CNIC validation only checks for 13 digits; there is no official checksum.
- Card network detection uses only the first digit, so it is simplified.
- Educational project. Do not enter real personal or card details.