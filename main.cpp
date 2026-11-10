#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <iomanip>
#include <limits>
#include <stdexcept>
using namespace std;

// Abstract base class: common details for every campus event.
class Event {
private:
    int id;
    string title;
    string date;
    int capacity;
    int registered;

public:
    Event(int i, const string& t, const string& d, int c, int r = 0)
        : id(i), title(t), date(d), capacity(c), registered(r) {
        if (capacity <= 0 || registered < 0 || registered > capacity)
            throw invalid_argument("Invalid event capacity or registration count.");
    }

    virtual ~Event() = default;
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getDate() const { return date; }
    int getCapacity() const { return capacity; }
    int getRegistered() const { return registered; }
    int seatsLeft() const { return capacity - registered; }

    bool registerStudent() {
        if (registered >= capacity) return false;
        ++registered;
        return true;
    }
    void undoRegistration() { if (registered > 0) --registered; }

    virtual string getType() const = 0;
    virtual string extraInfo() const = 0;
    virtual string serialize() const = 0;

    virtual void display() const {
        cout << left << setw(6) << id << setw(27) << title
             << setw(14) << getType() << setw(14) << date
             << setw(10) << (to_string(seatsLeft()) + "/" + to_string(capacity))
             << extraInfo() << '\n';
    }
};

class Workshop : public Event {
private:
    string instructor;
public:
    Workshop(int id, const string& title, const string& date, int cap,
             const string& teacher, int reg = 0)
        : Event(id, title, date, cap, reg), instructor(teacher) {}
    string getType() const override { return "Workshop"; }
    string extraInfo() const override { return "Instructor: " + instructor; }
    string serialize() const override {
        return "W|" + to_string(getId()) + "|" + getTitle() + "|" +
               getDate() + "|" + to_string(getCapacity()) + "|" +
               to_string(getRegistered()) + "|" + instructor;
    }
};

class CulturalEvent : public Event {
private:
    string venue;
public:
    CulturalEvent(int id, const string& title, const string& date, int cap,
                  const string& place, int reg = 0)
        : Event(id, title, date, cap, reg), venue(place) {}
    string getType() const override { return "Cultural"; }
    string extraInfo() const override { return "Venue: " + venue; }
    string serialize() const override {
        return "C|" + to_string(getId()) + "|" + getTitle() + "|" +
               getDate() + "|" + to_string(getCapacity()) + "|" +
               to_string(getRegistered()) + "|" + venue;
    }
};

class Seminar : public Event {
private:
    string department;
public:
    Seminar(int id, const string& title, const string& date, int cap,
            const string& dept, int reg = 0)
        : Event(id, title, date, cap, reg), department(dept) {}
    string getType() const override { return "Seminar"; }
    string extraInfo() const override { return "Department: " + department; }
    string serialize() const override {
        return "S|" + to_string(getId()) + "|" + getTitle() + "|" +
               getDate() + "|" + to_string(getCapacity()) + "|" +
               to_string(getRegistered()) + "|" + department;
    }
};

class Club {
private:
    int id;
    string name;
    string focus;
public:
    Club(int i, const string& n, const string& f) : id(i), name(n), focus(f) {}
    int getId() const { return id; }
    string getName() const { return name; }
    void display() const {
        cout << left << setw(8) << id << setw(25) << name << focus << '\n';
    }
};

struct Registration {
    string studentName;
    string studentId;
    int eventId;
};

class CampusConnect {
private:
    vector<unique_ptr<Event>> events;
    vector<Club> clubs;
    vector<Registration> registrations;

    Event* findEvent(int id) {
        for (auto& event : events)
            if (event->getId() == id) return event.get();
        return nullptr;
    }

    void loadEvents() {
        ifstream file("events.txt");
        if (!file) {
            addDefaultEvents();
            saveEvents();
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string type, idText, title, date, capText, regText, extra;
            getline(ss, type, '|'); getline(ss, idText, '|');
            getline(ss, title, '|'); getline(ss, date, '|');
            getline(ss, capText, '|'); getline(ss, regText, '|');
            getline(ss, extra);

            try {
                int id = stoi(idText), cap = stoi(capText), reg = stoi(regText);
                if (type == "W")
                    events.push_back(make_unique<Workshop>(id,title,date,cap,extra,reg));
                else if (type == "C")
                    events.push_back(make_unique<CulturalEvent>(id,title,date,cap,extra,reg));
                else if (type == "S")
                    events.push_back(make_unique<Seminar>(id,title,date,cap,extra,reg));
            } catch (...) {
                cerr << "Skipping an invalid event record.\n";
            }
        }
        if (events.empty()) {
            addDefaultEvents();
            saveEvents();
        }
    }

    void addDefaultEvents() {
        events.push_back(make_unique<Workshop>(101, "Python Bootcamp", "20-11-2026", 40, "Data Science Faculty"));
        events.push_back(make_unique<CulturalEvent>(102, "CU Cultural Night", "25-11-2026", 100, "Open Air Theatre"));
        events.push_back(make_unique<Seminar>(103, "Future of AI", "28-11-2026", 60, "Computer Science"));
    }

    void saveEvents() const {
        ofstream file("events.txt");
        if (!file) throw runtime_error("Unable to save events.txt");
        for (const auto& event : events) file << event->serialize() << '\n';
    }

    void loadRegistrations() {
        ifstream file("registrations.txt");
        string name, sid, eventIdText;
        while (getline(file, name, '|') && getline(file, sid, '|') &&
               getline(file, eventIdText)) {
            try { registrations.push_back({name, sid, stoi(eventIdText)}); }
            catch (...) { cerr << "Skipping an invalid registration record.\n"; }
        }
    }

    void showEvents() const {
        cout << "\n---------------- CAMPUS EVENTS ----------------\n";
        cout << left << setw(6) << "ID" << setw(27) << "Event"
             << setw(14) << "Type" << setw(14) << "Date"
             << setw(10) << "Seats" << "Details\n";
        cout << string(95, '-') << '\n';
        for (const auto& event : events) event->display();
    }

    void showClubs() const {
        cout << "\n--------------- STUDENT CLUBS -----------------\n";
        cout << left << setw(8) << "ID" << setw(25) << "Club" << "Focus\n";
        for (const auto& club : clubs) club.display();
    }

    void registerForEvent() {
        int id;
        string name, studentId;
        cout << "Enter event ID: ";
        if (!(cin >> id)) { resetInput(); return; }
        Event* event = findEvent(id);
        if (!event) { cout << "Event not found.\n"; return; }
        if (event->seatsLeft() == 0) { cout << "Sorry, this event is full.\n"; return; }

        resetInput();
        cout << "Enter student name (avoid | character): ";
        getline(cin, name);
        cout << "Enter student UID: ";
        getline(cin, studentId);
        if (name.empty() || studentId.empty() || name.find('|') != string::npos ||
            studentId.find('|') != string::npos) {
            cout << "Invalid student details.\n";
            return;
        }

        for (const auto& r : registrations) {
            if (r.studentId == studentId && r.eventId == id) {
                cout << "This student is already registered for this event.\n";
                return;
            }
        }

        if (event->registerStudent()) {
            registrations.push_back({name, studentId, id});
            ofstream file("registrations.txt", ios::app);
            if (!file) {
                registrations.pop_back(); // roll back the in-memory record
                event->undoRegistration();
                cout << "Could not save registration. Please try again.\n";
                return;
            }
            file << name << "|" << studentId << "|" << id << '\n';
            saveEvents();
            cout << "Registration successful!\n";
        }
    }

    void showRegistrations() const {
        cout << "\n------------- REGISTRATION RECORDS ------------\n";
        if (registrations.empty()) { cout << "No registrations found.\n"; return; }
        for (const auto& r : registrations)
            cout << "Student: " << r.studentName << " | UID: " << r.studentId
                 << " | Event ID: " << r.eventId << '\n';
    }

    static void resetInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    CampusConnect() {
        clubs.emplace_back(1, "CodeCrafters", "Programming and development");
        clubs.emplace_back(2, "Cultural Crew", "Music, dance and theatre");
        clubs.emplace_back(3, "EcoSphere", "Environment and sustainability");
        loadEvents();
        loadRegistrations();
    }

    // Function overloading: same function name, different parameter lists.
    int countEvents() const { return static_cast<int>(events.size()); }
    int countEvents(const string& type) const {
        int count = 0;
        for (const auto& e : events) if (e->getType() == type) ++count;
        return count;
    }

    void run() {
        int choice;
        do {
            cout << "\n============================================\n"
                 << "              CAMPUSCONNECT\n"
                 << "       Campus Event & Club Management\n"
                 << "============================================\n"
                 << "1. View all events\n"
                 << "2. Register for an event\n"
                 << "3. View student clubs\n"
                 << "4. View registrations\n"
                 << "5. Event summary\n"
                 << "6. Exit\n"
                 << "Enter your choice: ";

            if (!(cin >> choice)) { resetInput(); cout << "Please enter a number.\n"; continue; }

            try {
                switch (choice) {
                    case 1: showEvents(); break;
                    case 2: registerForEvent(); break;
                    case 3: showClubs(); break;
                    case 4: showRegistrations(); break;
                    case 5:
                        cout << "\nTotal events: " << countEvents()
                             << "\nWorkshops: " << countEvents("Workshop")
                             << "\nCultural events: " << countEvents("Cultural")
                             << "\nSeminars: " << countEvents("Seminar") << '\n';
                        break;
                    case 6: saveEvents(); cout << "Thank you for using CampusConnect!\n"; break;
                    default: cout << "Invalid choice. Select 1 to 6.\n";
                }
            } catch (const exception& ex) {
                cout << "Error: " << ex.what() << '\n';
            }
        } while (choice != 6);
    }
};

int main() {
    try {
        CampusConnect app;
        app.run();
    } catch (const exception& ex) {
        cerr << "Application error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
