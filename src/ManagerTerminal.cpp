/*
   Damon Morey, Philip Prater, Samuel Shippey, Son Vu, Yves Wienecke 2018©
   ManagerTerminal.cpp
   */

#include "ManagerTerminal.h"
using namespace std;

//get option for menu
int getOption(string prompt, int lower, int upper) {
	int opt;
	assert(lower <= upper);

	while(true) {
		cout << prompt << "\n: ";
		cout.flush();
		cin >> opt;
		if (cin.fail()) {
			cin.clear();
			opt = lower -1;
		}

		cin.ignore(100, '\n');
		if(opt >= lower && opt <= upper)
			break;
		else
			cout << "Invalid option, try again.\n\n";
	}
	return opt;
}

//store string in variable from user input
string getString(string prompt) {
	cout << endl << prompt << ": ";
	cout.flush();
	string response;
	getline(cin, response);
	return response;
}

bool getYesOrNo(string prompt) {
  char opt;
  while(true) {
    cout << prompt << " (y/n): ";
    cout.flush();
    cin >> opt;
    if(cin.fail()) {
      cin.clear();
      opt = 'n';
    }
    cin.ignore(100, '\n');
    if(opt == 'y')
      return true;
    else if(opt == 'n')
      return false;
  }
}

//clear screen
void clear() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

//ManagerTerminal
//Connect Manager Terminal to DataCenter
ManagerTerminal::ManagerTerminal(DataCenter * dc) {
	this->dc = dc;
}

//run Manager Terminal menu
void ManagerTerminal::run() {
	int opt;
	bool running;
    
    clear();
	cout << "Starting Manager Terminal..." << endl;

  //main loop for manager terminal operations
  running = true;
  while(running) {
      cout << "\n\nManager Terminal Main Menu\n";
    opt = getOption(
           "Select an option from the list:\n" 
           "1) Manage Service Directory\n"
           "2) Manage Memberships\n"
           "3) Manage Provider Network\n"
           "4) Request Report\n"
           "5) Quit Manager Terminal", 1, 5);
      clear();
    switch(opt) {
      case 1:
        dc->printServiceList();
        switch(getOption("1) Add Service\n"
                         "2) Remove Service\n"
                         "3) Modify Service", 1, 3)) {
          case 1:
            addService();
            break;
          case 2:
            removeService();
            break;
          case 3:
            modifyService();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 2:
            dc->printMembers();
        switch(getOption("1) Add Member\n"
                         "2) Remove Member\n"
                         "3) Modify Member\n"
                         "4) Suspend Member\n"
                         "5) Activate Member", 1, 5)) {
          case 1:
            addMember();
            break;
          case 2:
            removeMember();
            break;
          case 3:
            modifyMember();
            break;
          case 4:
            suspendMember();
            break;
          case 5:
            activateMember();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 3:
        dc->printProviders();
        switch(getOption("1) Add Provider\n"
                         "2) Remove Provider\n"
                         "3) Modify Provider", 1, 3)) {
          case 1:
            addProvider();
            break;
          case 2:
            removeProvider();
            break;
          case 3:
            modifyProvider();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 4:
        requestReport();
        break;
      case 5:
        running = false;
        break;
      default:
        break;
    }
  }
}

//add new member to Data Center through Manager terminal
void ManagerTerminal::addMember(void) {
	string name, number, phone;
	address adr;
	regex phone_format("\\d{3}-\\d{3}-\\d{4}");

	do { name = getString("Enter full name of member to be added, up to 25 letters"); } while (name.size() > 25);
    
    do { number = getString("Enter the 9 digit member ID of this new member. IDs must not start with a 0");
    } while (number.size());
    
    do { phone = getString("Enter phone number in the form ###-###-####");
    } while (!regex_match(phone, phone_format));
    
    do { adr.streetAddress = getString("Enter street address, up to 25 letters"); } while (adr.streetAddress.size() > 25);	

    do { adr.city = getString("Enter city, up to 14 letters"); } while (adr.city.size() > 14);

    do { adr.state = getString("Enter state, ex. OR"); } while (adr.state.size() != 2);
    

    do{ adr.zip = getString("Enter zip code");
    } while (stoi(adr.zip) < 10000 || stoi(adr.zip) > 99999);

	Member m(name, number, phone, adr);
	dc->addMember(m);

    cout << "Added " << m.name << " to members!\n\n";
}

//remove member to Data Center through Manager terminal
void ManagerTerminal::removeMember(void) { 
    string id;
	id = getString("Enter ID of member to be removed");
	if(dc->hasMember(id)) {
		dc->removeMember(id);
		cout << "Member successfully removed from the system." << endl;
	} else {
		cout << "Member was not found in the system." << endl;
	}
}

void ManagerTerminal::modifyMember(void) {
    string id;

    id = getString("Enter ID of member to be modified");

    if (dc->modifyMember(id)) {
        cout << "Member updated!" << endl;
    } else {
        cout << "Member was not found in the system." << endl;
    }
}

void ManagerTerminal::suspendMember(void) {
  string id;

  id = getString("Enter ID of member to supend");

  if(dc->suspendMember(id)) {
    cout << "Member suspended." << endl;
  } else {
    cout << "Member was not found in the system." << endl;
  }
}

void ManagerTerminal::activateMember(void) {
  string id;

  id = getString("Enter ID of member to activate");

  if(dc->activateMember(id)) {
    cout << "Member activated." << endl;
  } else {
    cout << "Member was not found in the system." << endl;
  }
}

//add new provider to Data Center through Manager terminal
void ManagerTerminal::addProvider(void) {
	string name, number, phone;
	address adr;
	regex phone_format("\\d{3}-\\d{3}-\\d{4}");

	do { name = getString("Enter full name of provider to be added, up to 25 letters"); } while (name.size() > 25);
    
    do { number = getString("Enter the 9 digit provider ID of this new provider. IDs must not start with 0");
    } while (stoi(number) < 100000000 || stoi(number) > 999999999);
    
    do { phone = getString("Enter phone number in the form ###-###-####");
    } while (!regex_match(phone, phone_format));
    
    do { adr.streetAddress = getString("Enter street address, up to 25 letters"); } while (adr.streetAddress.size() > 25);	

    do { adr.city = getString("Enter city, up to 14 letters"); } while (adr.city.size() > 14);

    do { adr.state = getString("Enter state, ex. OR"); } while (adr.state.size() != 2);
    

    do{ adr.zip = getString("Enter zip code");
  } while (stoi(adr.zip) < 10000 || stoi(adr.zip) > 99999);


	Provider p(name, number, phone, adr, 0);
	dc->addProvider(p);
    
  cout << "Added " << p.name << " to providers!\n\n";
}


//remove provider from Data Center through Manager terminal
void ManagerTerminal::removeProvider(void) {
	string id;
	id = getString("Enter ID of provider to be removed");
	if(dc->hasProvider(id)) {
		dc->removeProvider(id);
		cout << "Provider successfully removed from the system." << endl;
	} else {
		cout << "Provider was not found in the system." << endl;
	}
}

void ManagerTerminal::modifyProvider(void) {
   string id;
   id = getString("Enter ID of provider to be modified");
   if(dc->modifyProvider(id)) {
       cout << "Provider modified!\n";
   } else {
       cout << "Provider was not found in the system." << endl;
   }
}

//add new service to service list in Data Center
void ManagerTerminal::addService(void) {
	string code, name, cost;
    do { code = getString("Enter 6 digit code of service to be added");
    } while (stoi(code) < 100000 || stoi(code) > 999999);

    do { name = getString("Enter name of service to be added, up to 20 letters"); } while (name.size() > 20);

    do { cout << "\nEnter cost of service to be added, up to $999.99: ";
        cin >> get_money(cost);
    } while (stold(cost) > 999.99);

	Service s(code, name, stof(cost));
	dc->addService(s);
    cout << "Service added!\n\n";
}


//remove service from service list in Data Center
void ManagerTerminal::removeService(void) {
	string code;
	code = getString("Enter code of service to be removed");
	if(dc->hasService(code)) {
		dc->removeService(code);
		cout << "Service successfully removed from the system." << endl;
	} else {
		cout << "Service was not found in the system." << endl;
	}
}

void ManagerTerminal::modifyService(void) {
    string name;
    name = getString("Enter code of service to be modified");
    if(dc->modifyService(name)) {
        cout << "Modified service!\n";
    } else {
        cout << "Service was not found in the system." << endl;
    }
}

//Request reports through manager terminal 
void ManagerTerminal::requestReport(void) {
	int opt;
	string id;
	opt = getOption("Select an option from the list:"
			"1) All Reports\n"
			"2) Manager Report\n"
			"3) Member Report\n"
			"4) Provider Report",1,4);
	switch(opt) {
		case 1:
			dc->allReports();
			cout << "All reports written." << endl;
			break;

		case 2:
			dc->managerReport();
			cout << "Manager report written." << endl;
			break;

		case 3:
      dc->printMembers();
			id = getString("Enter the member ID for the requested report");
			if(dc->memberReport(id)) {
				cout << "Member report written." << endl;
			} else {
				cout << "Didn't find member, no report written." << endl;
			}
			break;

    case 4:
      dc->printProviders();
      id = getString("Enter the provider ID for the requested report");
      if(dc->providerReport(id)) {
        cout << "Provider report written." << endl;
      } else {
        cout << "Didn't find provider, no report written." << endl;
      }
      break;
  }
}

