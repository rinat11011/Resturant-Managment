#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <string>
using namespace std;
extern Restaurant* backup;

class Restaurant{		
public:
	Restaurant();
	Restaurant(const Restaurant &restaurant);                               //COPY CONSTRUCTOR
    Restaurant(const Restaurant &&restaurant);                              //MOVE CONSTRUCTOR
	Restaurant(const std::string &configFilePath);
	vector<int>& makeVectorSeats(string s,int numOfTables);
	void createTables(vector<int>& seats);
	int getNumOfTables() const;
	DishType getType(string s);
	void createMenu(vector<string>& txt_v,int i);
	std::vector<Dish>& getMenu();
	std::vector<Dish> menu;
	void start();
    Table* getTable(int ind);
    void createCustomer(string line,int tableId);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the actions log
    string getLine();
    string getInput();
    std::vector<Table*>& getTables();
    void setclose();
    Restaurant& operator=(const Restaurant &rest);                          //ASSIGNMENT OPERATOR
    Restaurant& operator=(const Restaurant &&rest);                         //MOVE ASSIGNMENT OPERATOR
	~Restaurant();                                                          //DESTRUCTOR

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<BaseAction*> actionsLog;
    int numOfTables;
    string line;
	int CustomerID;
	string input;
};

#endif