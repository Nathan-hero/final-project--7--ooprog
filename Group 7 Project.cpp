#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>		//Hopefully this is allowed
using namespace std;

class LogIn { // To get admin access | username: Admin | password: admin123
private:
    string userName;
    string password;
    bool isAdmin;

public:
    // Constructor
    LogIn() : userName(""), password(""), isAdmin(false) {}

    // Getters
    string getUserName() {
        return userName;
    }
    
    string getPassword() {
        return password;
    }
    
    bool getIsAdmin() {
        return isAdmin;
    }

    void logIn() {
        cout << "----------[Log in]----------\n";
        cout << "Username: ";
        cin >> userName;

        if (userName == "Admin") {
            string inputPassword;
            isAdmin = true;
            bool temp = false;

            while (!temp) {
                cout << "Password: ";
                cin >> inputPassword;

                if (inputPassword == "admin123") {
                    isAdmin = true;
                    temp = true;
                    cout << "Login successful! You are now logged in as Admin.\n";
                } else {
                    cout << "\nIncorrect password. Try again.\n";
                }
            }
        } else {
            cout << "Welcome " << userName << endl;
        }
    }
    
    void logOut(){	//Reset every credentials in log in BUT data should stay the same
    	userName = "";
        password = "";
        isAdmin = false;
        cout << "Successfully logged out.\n";
	}
};

class ScreenMenu {		//Screen to show difference per user
public:
	
	virtual void menuScreen() const = 0;
};

class AdminScreen : public ScreenMenu{
	public:
		 void menuScreen() const override {
        cout << "==========[Admin Menu]==========\n";
        cout << "[1 - Add event]\n[2 - Display events]\n[3 - Edit event]\n[4 - Delete event]\n[5 - Sort]\n[6 - Events to confirm]\n[7 - Log out]\n[8 - Close program]\n";
    }
};

class UserScreen : public ScreenMenu{
	public:
		void menuScreen() const override {
        cout << "==========[User Menu]==========\n";
        cout << "[1 - Add Event]\n[2 - Show all events]\n[3 - Sort events]\n[4 - Log out]\n[5 - Close program]\n";
    }
};
//=================================================================// Data

struct Event {
    string name;
    int month;
    int day;
    int year;
    string eventName;
    string wordMonth;
	string convertMonthToName(int month) { //Convert Number to Name of Month
	    switch (month) {
	        case 1: return "Jan";
	        case 2: return "Feb";
	        case 3: return "Mar";
	        case 4: return "Apr";
	        case 5: return "May";
	        case 6: return "Jun";
	        case 7: return "Jul";
	        case 8: return "Aug";
	        case 9: return "Sep";
	        case 10: return "Oct";
	        case 11: return "Nov";
	        case 12: return "Dec";
	        default: return "Invalid date";
	    }
	}
    Event(string n, int m, int d, int y, string evName)
    : name(n), month(m), day(d), year(y), eventName(evName), wordMonth(convertMonthToName(m)) {}
};


// Singleton class to manage events
class EventManager {
private:
    static EventManager* instance;
    vector<Event> events;

    EventManager() {//Format 		Name of user / month / day / year / name of event
        events.emplace_back("Nathan", 2, 12, 2005, "Birthday Celebration");
        events.emplace_back("Sample", 5, 25, 2025, "House party"); 
        
    }

public:
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    static EventManager& getInstance() {
        if (!instance) {
            instance = new EventManager();
        }
        return *instance;
    }

    void addEvent(const Event& event) {
        events.push_back(event);
    }

    vector<Event>& getEvents() {
        return events;
    }

    void displayEvents() const {		//Display event
        cout << setw(20) << left << "Name" << setw(10) << left << "Month" << setw(10) << left << "Day" << setw(10) << left << "Year" << setw(10) << left << "Event" << endl;
        for (const auto& event : events) {
            cout << setw(20) << left << event.name << setw(10) << left << event.wordMonth << setw(10) << left << event.day << setw(10) << left << event.year << setw(10) << left << event.eventName << endl;
        }
        cout << endl << endl;
    }

    bool isDateUnique(int month,int day, int year) const { //Check if the date is unique
    for(const auto& event : events) {
        if(event.month == month && event.day == day && event.year == year) {
            return false; //Date is Used
    	}
    }
    return true; //Date is unique
    }
    bool isNameUnique(string eventName) const {
    	for(const auto& event : events) {
    		if(event.eventName == eventName){
    			return false;
			}
		}
		return true;
	}
};


// Initialize static member
EventManager* EventManager::instance = nullptr;



//=================================================================// Strategy pattern (Used for mulltiple sorting mechanics sorting used : bubble sort ASC and DSC )

class StrategyPattern {
public:
    virtual void Strat(vector<Event>& events) = 0;
    virtual ~StrategyPattern() = default;
};

// BubbleSort ASC for sorting events in ascending order by year
class BubbleSortASC : public StrategyPattern {
public:
    void Strat(vector<Event>& events) override {
        for (size_t i = 0; i < events.size(); ++i) {
            for (size_t j = 0; j < events.size() - i - 1; ++j) {
                if (events[j].year > events[j + 1].year) {
                    swap(events[j], events[j + 1]);
                }
            }
        }
    }
};
// BubbleSort DSC for sorting events in descending order by year
class BubbleSortDSC : public StrategyPattern {
public:
    void Strat(vector<Event>& events) override {
        for (size_t i = 0; i < events.size(); ++i) {
            for (size_t j = 0; j < events.size() - i - 1; ++j) {
                if (events[j].year < events[j + 1].year) {
                    swap(events[j], events[j + 1]);
                }
            }
        }
    }
};

//=================================================================// [Edit event | admin]
class Edit {
private:
    string searchKey;

public:
    void editEvent() {
   vector<Event>& events = EventManager::getInstance().getEvents(); // Access the events
    string searchKey;

    cout << "Enter the name of the event to edit: ";
    cin.ignore();
    getline(cin, searchKey);

    // Search for the specific event
    bool found = false;
    for (size_t i = 0; i < events.size(); ++i) {
        if (events[i].name == searchKey) {
            found = true;
            cout << "----------[Edit Menu]----------\n";
            cout << "[1 - Name]\n[2 - Day]\n[3 - Month]\n[4 - Year]\n[5 - Event Name]\n";
            cout << "Enter the field you want to edit: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter new name: ";
                    cin.ignore();
                    getline(cin, events[i].name);
                    break;
                case 2: {
                    // Edit day with uniqueness check
                    while (true) {
                        cout << "Enter new day: ";
                        cin >> events[i].day;
                        int maxDay = 31;
                        if (events[i].month == 2) {
                            maxDay = 28;
                            if ((events[i].year % 4 == 0 && events[i].year % 100 != 0) || (events[i].year % 400 == 0)) {
                                maxDay = 29;
                            }
                        } else if (events[i].month == 4 || events[i].month == 6 || events[i].month == 9 || events[i].month == 11) {
                            maxDay = 30;
                        }

                        if (cin.fail() || events[i].day < 1 || events[i].day > maxDay) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid day. Please enter a number between 1 and " << maxDay << ".\n";
                        } else {
                            // Check for uniqueness
                            if (EventManager::getInstance().isDateUnique(events[i].month, events[i].day, events[i].year)) {
                                break; // Exit the loop if the date is unique
                            } else {
                                cout << "Error: Another event exists on this date. Please choose another day.\n";
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    // Edit month with uniqueness check
                    while (true) {
                        cout << "Enter new month (1-12): ";
                        cin >> events[i].month;
                        if (cin.fail() || events[i].month < 1 || events[i].month > 12) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid month. Please enter a number between 1 and 12.\n";
                        } else {
                            // Check for uniqueness
                            if (EventManager::getInstance().isDateUnique(events[i].month, events[i].day, events[i].year)) {
                                break; // Exit the loop if the date is unique
                            } else {
                                cout << "Error: Another event exists on this date. Please choose another month.\n";
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    // Edit year with uniqueness check
                    while (true) {
                        cout << "Enter new year: ";
                        cin >> events[i].year;
                        if (cin.fail() || events[i].year < 1900 || events[i].year > 2100) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid year. Please enter a year between 1900 and 2100.\n";
                        } else {
                            // Check for uniqueness
                            if (EventManager::getInstance().isDateUnique(events[i].month, events[i].day, events[i].year)) {
                                break; // Exit the loop if the date is unique
                            } else {
                                cout << "Error: Another event exists on this date. Please choose another year.\n";
                            }
                        }
                    }
                    break;
                }
                case 5:
                    cout << "Enter new event name: ";
                    cin.ignore();
                    getline(cin, events[i].eventName);
                    break;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }

    cout << "Event updated successfully.\n";
        }
    }
}
};

//====================================================================// Delete
class DeleteEvent {
private:
    string searchKey;

public:
    void deleteEvent() {
        vector<Event>& events = EventManager::getInstance().getEvents();
        cout << "Take note: please input the EVENT NAME to search for the event to delete\n";
		cout << "Enter the name of the event to delete: ";
        cin.ignore();
        getline(cin, searchKey);

        auto it = remove_if(events.begin(), events.end(), [&](const Event& event) {
            return event.eventName == searchKey;
        });

        if (it != events.end()) {
            events.erase(it, events.end());
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }
};

//==================================================================//Sort Menu

class SortMenu {
public:
    void sortEvent() {
        int choiceDate;
        cout << "----------[Sort Menu]----------\n";
        cout << "Sort by: \n";
        cout << "[1 - Date (ASC)]\n[2 - Date (DSC)]\n";
        cout << "Enter your choice: ";
        cin >> choiceDate;

        vector<Event>& events = EventManager::getInstance().getEvents();

        StrategyPattern* strategy = nullptr;
        if (choiceDate == 1) {
            strategy = new BubbleSortASC();		//Sort date in ascending
            cout << "\nSorted date in ascending order\n";
            system("pause");
            system("cls");
        } else if (choiceDate == 2) {
            strategy = new BubbleSortDSC();		//Sort date in descending
            cout << "\nSorted date in descending order\n";
            system("pause");
            system("cls");
        } else {
            cout << "Invalid choice.\n";
            cin.clear();
    		cin.ignore(10000, '\n');
    		system("pause");
            system("cls");
            return;
        }

        if (strategy) {
            strategy->Strat(events);  // Apply sorting
            delete strategy;  // Clean up
        }
    }  
};
//==================================================================// User Menu start //==================================================================//
struct ConfirmEvent {		//Exact copy of the first menu.... to avoid data format from being different from the admin format
    string cName;
    int cMonth;
    int cDay;
    int cYear;
    string cEventName;
    string cWordMonth;
	string cConvertMonthToName(int cMonth) { //Convert Number to Name of Month
	    switch (cMonth) {
	        case 1: return "Jan";
	        case 2: return "Feb";
	        case 3: return "Mar";
	        case 4: return "Apr";
	        case 5: return "May";
	        case 6: return "Jun";
	        case 7: return "Jul";
	        case 8: return "Aug";
	        case 9: return "Sep";
	        case 10: return "Oct";
	        case 11: return "Nov";
	        case 12: return "Dec";
	        default: return "Invalid date";
	    }
	}
    ConfirmEvent(string cn, int cm, int cd, int cy, string cevName)
    : cName(cn), cMonth(cm), cDay(cd), cYear(cy), cEventName(cevName), cWordMonth(cConvertMonthToName(cm)) {}
};

class TemporaryData : public LogIn {		//Temporary data to be merged with the data in the main vector || must be confirmed by the admin
private:
    vector<ConfirmEvent> confirmEvent;

public:
    TemporaryData() {
        confirmEvent.emplace_back("User12", 6, 15, 2025, "Party again");
    }

    void addTempEvent(const ConfirmEvent& event) {
        confirmEvent.push_back(event);
    }

    vector<ConfirmEvent>& getConfirmEvents() {  // Provide access to temporary events
        return confirmEvent;
    }

    void displayTempEvents() const {
        cout << setw(20) << left << "Name" << setw(10) << left << "Month" << setw(10) << left << "Day" << setw(10) << left << "Year" << setw(20) << left << "Event" << endl;
        for (const auto& event : confirmEvent) {
            cout << setw(20) << left << event.cName << setw(10) << left << event.cWordMonth << setw(10) << left << event.cDay << setw(10) << left << event.cYear << setw(20) << left << event.cEventName << endl;
        }
        cout << endl;
    }
};

void adminConfirmEvents(TemporaryData& tempData, EventManager& eventManager) {
    vector<ConfirmEvent>& tempEvents = tempData.getConfirmEvents();
    
    if (tempEvents.empty()) {	//If empty
        cout << "No events to confirm.\n";
        return;
    }

    for (size_t i = 0; i < tempEvents.size(); ++i) {						//For confirming
        const auto& event = tempEvents[i];
        cout << "Event " << i + 1 << ":\n";															//]
        cout << "User: " << event.cName << "\n";													//] For displaying each item to be confirmed by the Admin 
        cout << "Date: " << event.cWordMonth << " " << event.cDay << ", " << event.cYear << "\n";	//]
        cout << "Event Name: " << event.cEventName << "\n";

        cout << "\n[1 - Approve]\n[Any other text to reject]\nChoose:";		//Decided to just one by one accept and reject events from users || just so its simple to code
        int choice;
        cin >> choice;

        if (choice == 1) {
            // Approve and merge to main event manager :)
            Event approvedEvent(event.cName, event.cMonth, event.cDay, event.cYear, event.cEventName);
            eventManager.addEvent(approvedEvent);
            cout << "Event approved and added to the main event list.\n";
        } else {
            cout << "Event rejected.\n";
            system("pause");
	        system("cls");
	        cin.clear();
	    	cin.ignore(10000, '\n');		//To clear cin since the admin is allowed to use any text... if admin were to use letters... loop wont stop unless these codes are used
        }
    }
    // Clear temporary data
    tempEvents.clear();
    cout << "All pending events processed.\n";
}

//==================================================================// User Menu end //==================================================================//


//=================================================================// Main
int main() {
    LogIn login; // Create an instance of LogIn
    Edit edit;
    DeleteEvent deleteEv;
    SortMenu sortMe;
    
    ScreenMenu* adminScreen = new AdminScreen();
    ScreenMenu* userScreen = new UserScreen();
    //====// User //====//
	TemporaryData TemDat;

while(true){

    login.logIn(); // Call the logIn method
    
    int option; // Menu option
    
    if (login.getIsAdmin()) {
        bool tempo = true;
        while (tempo) {
            adminScreen -> menuScreen();			
            cout << "\nChoose an option: ";
            cin >> option;

            switch (option) {
             case 1: {
            	// Add event
                string name, eventName;
                int month, day, year;
                cout << "Enter name: ";
                cin.ignore(10000, '\n');
                getline(cin, name);
            
			while(true){												
                while (true) {
                    cout << "Enter month (1-12): ";
                    cin >> month;
                    if (cin.fail() || month < 1 || month > 12) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid month. Please enter a number between 1 and 12.\n";
                    } else {
                        break;
                    }
                }

                while (true) {
                    cout << "Enter day: ";
                    cin >> day;
                    int maxDay = 31;
                    if (month == 2) {
                        maxDay = 28;
                        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                            maxDay = 29;
                        }
                    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                        maxDay = 30;
                    }

                    if (cin.fail() || day < 1 || day > maxDay) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid day. Please enter a number between 1 and " << maxDay << ".\n";
                    } else {
                        break;
                    }
                }

                while (true) {
                    cout << "Enter year: ";
                    cin >> year;
                    if (cin.fail() || year < 1900 || year > 2100) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid year. Please enter a year between 1900 and 2100.\n";
                    } else {
                        break;
                    }
                }
                if (EventManager::getInstance().isDateUnique(month, day, year)) {
			        break; // Exit the loop if the date is unique							
			    } else {																	
			        cout << "Error: An event already exists on this date. Please choose another date.\n";		
			    }																			
			}																				
				cin.ignore(10000, '\n');
					while(true){
	                    cout << "Enter event name: ";
	                    getline(cin, eventName);
					
					if (EventManager::getInstance().isNameUnique(eventName)){
						break;
					}
					else{
						cout << "Error: An event with the same name already exists. Please enter another name.\n";
					}
					}

                    Event newEvent(name, month, day, year, eventName);
                    EventManager::getInstance().addEvent(newEvent);
                    cout << "Event added successfully.\n";
                }
            case 2: {//Display Events		[Done]
                EventManager::getInstance().displayEvents();
                cout << endl;
				system("pause");
                system("cls");
                break;
            }
            case 3: {// Edit Event			[Done]
                edit.editEvent();
                break;
            }
            case 4: {// Delete Event		[Done]
                deleteEv.deleteEvent();
                break;
            }
            case 5: {// Sort				[Done]
                sortMe.sortEvent();
                break;
            }
            case 6: {// Events to Confirm	[Done]
                adminConfirmEvents(TemDat, EventManager::getInstance());
   				system("pause");
    			system("cls");
				break;
            }
            case 7: {
                // Log out					[Done]
                login.logOut();
                cout << endl << endl;
                system("pause");
                system("cls");
                tempo = false;
                break;
            }
            case 8: {
                // Close Program			[Done]
                return 0;
                break;
            }

            default:
                cout << "Invalid input\n";
                system("pause");
                system("cls");
                //Use this to avoid letters causing multiple loops :) 
                cin.clear();
    			cin.ignore(10000, '\n');
                break;
            }
        }
    } 
	else {
    	//=========================================================================================================// User options //========================================//
    	bool tempoUser = true;
    	while(tempoUser){
			userScreen -> menuScreen();
			
			cin >> option;
			
			switch(option){//						Note: Most of these sections are just taken from admin with a few adjustments to send the codes to TEMPORARY data that we can combine to the main data		
				case 1:{
				//Add event
            string name, eventName;
            int month, day, year;

            cout << "Enter name: ";
            cin.ignore(10000, '\n');
            getline(cin, name);

            // Use a loop to keep asking for the date until a unique one is provided
            while (true) {
            // Get the month
            while (true) {
                cout << "Enter month (1-12): ";
                cin >> month;
            if (cin.fail() || month < 1 || month > 12) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid month. Please enter a number between 1 and 12.\n";
            } else {
                break;
            }
            }

            // Get the day
                while (true) {
                    cout << "Enter day: ";
                    cin >> day;
                int maxDay = 31;
                if (month == 2) {
                    maxDay = 28;
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    maxDay = 29;
                }
                } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                    maxDay = 30;
                }

                if (cin.fail() || day < 1 || day > maxDay) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid day. Please enter a number between 1 and " << maxDay << ".\n";
                } else {
                    break;
                }
                }

                // Get the year
                while (true) {
                    cout << "Enter year: ";
                    cin >> year;
                if (cin.fail() || year < 1900 || year > 2100) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid year. Please enter a year between 1900 and 2100.\n";
                } else {
                    break;
                }
                }

                // Check if the date is unique
                if (EventManager::getInstance().isDateUnique(month, day, year)) {
                    break; // Exit the loop if the date is unique
                } else {
                    cout << "Error: An event already exists on this date. Please choose another date.\n";
                }
                }
                cin.ignore(10000, '\n');
					while(true){
	                    cout << "Enter event name: ";
	                    getline(cin, eventName);
					
					if (EventManager::getInstance().isNameUnique(eventName)){
						break;
					}
					else{
						cout << "Error: An event with the same name already exists. Please enter another name.\n";
					}
					}

                    Event newEvent(name, month, day, year, eventName);
                    EventManager::getInstance().addEvent(newEvent);
                    cout << "Event added successfully.\n";
                }
				case 2:{											//Decided to just display all events to make sure the user won't make an event with the same date as whats already added	
				//Read all event
					EventManager::getInstance().displayEvents();
					cout << endl;
					system("pause");
	                system("cls");
					break;
				}
				case 3:{			//Sort
					sortMe.sortEvent();
					break;
				}
				case 4:{											//Literally the same as admin
				//Log out
					login.logOut();
	                cout << endl << endl;
	                system("pause");
	                system("cls");
	               	tempoUser = false;								//Tried to fix this a couple times.... decided to just end loop and go into Log In at the beginning
					break;
				}
				case 5:{											//Just closing the program
				//Close program
					return 0;
					break;
				}
				default:{
					cout << "Invalid input\n";
	                system("pause");
	                system("cls");
	                cin.clear();
	    			cin.ignore(10000, '\n');
					break;
				}
			}
		}
    }
}
delete userScreen;
delete adminScreen;

}