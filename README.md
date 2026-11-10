# CampusConnect — C++ OOP Mini Project

CampusConnect is a console-based Campus Event & Club Management System built using C++17.

## Features
- View campus workshops, cultural events and seminars
- Register a student for an event
- Prevent duplicate registration by student UID for the same event
- Check available seats
- View student clubs and registration records
- Display event-type counts
- Save event capacity and registrations in text files

## Requirements
- C++ compiler supporting C++17 (GCC / MinGW / Clang)
- VS Code, Code::Blocks, or any C++ editor

## Compile and Run
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o CampusConnect
./CampusConnect
```
On Windows:
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o CampusConnect.exe
CampusConnect.exe
```

Run the executable from this project folder. The program creates `events.txt` and `registrations.txt` when needed.

## OOP Concepts
- Abstraction: abstract `Event` class
- Encapsulation: private data members with public methods
- Inheritance: `Workshop`, `CulturalEvent`, and `Seminar` inherit from `Event`
- Runtime polymorphism: overridden virtual methods called through base-class pointers
- Function overloading: `countEvents()` and `countEvents(type)`
- Constructors, STL vectors, smart pointers, file handling and exception handling

## GitHub
Upload the project files to your own repository and add your repository URL to the college report.

## Sample Output
See `Sample_Output.txt` for a captured console transcript and `Sample_Output_Screenshot.png` for a visual screenshot of an actual demonstration run.
