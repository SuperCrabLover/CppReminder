#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

struct Year {
  int value;
  explicit Year(int _v) {
    value = _v;
  }
};

struct Month {
  int value;
  explicit Month(int _v) {
    value = _v;
  }
};

struct Day {
  int value;
  explicit Day(int _v) {
    value = _v;
  }
};


class Date {
public:
  Date (Year _year, Month _month, Day _day) {
    if (_month.value < 1 || _month.value > 12) {
        throw invalid_argument("Month value is invalid: " + to_string(_month.value));
    }
    if (_day.value < 1 || _day.value > 31) {
        throw invalid_argument("Day value is invalid: " + to_string(_day.value));
    }
    year = _year.value;
    month = _month.value;
    day = _day.value;
  }

  Date() {
    year = 1;
    month = 1;
    day = 1;
  }
  int GetYear() const {
    return year;
  }
  int GetMonth() const {
    return month;
  }
  int GetDay() const {
    return day;
  }
  
private:
  int year;
  int month;
  int day;
};

istream& operator>>(istream& stream, Date& date); 
ostream& operator<<(ostream& stream, const Date& date);
bool operator<(const Date& lhs, const Date& rhs);

istream& operator>>(istream& stream, Date& date) {
  int year, day, month;
  char separator;
  string rest = "";
  string input;
  stream >> input;
  stringstream ss(input);
  if (ss >> year      && 
      ss >> separator && 
      separator == '-'&& 
      ss >> month     &&
      ss >> separator &&
      separator == '-'&&
      ss >> day       &&
      ss.eof()) {
      date = Date(Year(year), Month(month), Day(day));   
  } else {
    throw runtime_error("Wrong date format: " + input);
  }
  return stream;
} 

ostream& operator<<(ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() << "-";
  stream << setw(2) << setfill('0') << date.GetMonth() << "-";
  stream << setw(2) << setfill('0') << date.GetDay();
  return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
  if (lhs.GetYear() == rhs.GetYear()) {
    if (lhs.GetMonth() == rhs.GetMonth()) {
      return lhs.GetDay() < rhs.GetDay(); 
    } else {
      return lhs.GetMonth() < rhs.GetMonth();
    }
  } else {
    return lhs.GetYear() < rhs.GetYear();
  }
}

class Database {
public:
  void AddEvent(const Date& date, const string& event) {
    if (event != "") {
      date_to_events[date].insert(event);
    }
  }
  bool DeleteEvent(const Date& date, const string& event) {
    if (date_to_events[date].count(event) == 1) {
      date_to_events[date].erase(event);
      return true;
    } else {
      return false;
    }
  }
  int DeleteDate(const Date& date) {
    if (date_to_events.count(date) == 0) {
      return 0;
    } else {
      int amount = date_to_events[date].size();
      date_to_events.erase(date);
      return amount;
    }
  }

  set<string> Find(const Date& date) const {
    if (date_to_events.count(date) == 0) {
      return {};
    } else {
      return date_to_events.at(date); 
    }
  }
  
  void Print() const {
    for (const auto& curr_date_events : date_to_events) {
      for (const string& event : curr_date_events.second) {
        cout << curr_date_events.first << " " << event << endl;
      }
    }
  }
private:
  map<Date, set<string>> date_to_events;
};

int main(void) {
  Database db;
  string command;
  while (getline(cin, command)) {
    stringstream ss(command);
    string op;
    ss >> op;
    if (op == "") {
      continue;
    } else if ("Add" == op) {
      Date inp_date;
      try {
        ss >> inp_date;
      } catch (const exception& e) {
        cout << e.what() << endl;
        return 0;
      }
      string event;
      ss >> event;
      db.AddEvent(inp_date, event);
    } else if ("Del" == op) {
      Date inp_date;
      try {
        ss >> inp_date;
      } catch (const exception& e) {
        cout << e.what() << endl;
        return 0;
      }
      string event = "";
      if (ss >> event) {
        cout << (db.DeleteEvent(inp_date, event) ? "Deleted successfully" : "Event not found") << endl;
      } else {
        cout << "Deleted " << db.DeleteDate(inp_date) << " events" << endl;
      }
    } else if ("Find" == op) {
      Date inp_date;
      try {
        ss >> inp_date;
      } catch (const exception& e) {
        cout << e.what() << endl;
        return 0;
      }
      const set<string> events = db.Find(inp_date);
      for (const string event : events) {
        cout << event << endl;
      } 
    } else if ("Print" == op) {
      db.Print();
    } else {
      cout << "Unknown command: " << op << endl;
      return 0;
    }
  }

  return 0;
}
