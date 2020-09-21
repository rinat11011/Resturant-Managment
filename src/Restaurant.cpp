#include <iostream>
#include "../include/Restaurant.h"
#include <fstream>
#include <string>
#include <vector>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"
#include "../include/Customer.h"
using namespace std;


//--------Constructor---------//


Restaurant::Restaurant():menu(), open(true), tables(), actionsLog(), numOfTables(0), line(""), CustomerID(0), input("") {}

Restaurant::Restaurant(const Restaurant &rest): menu(), open(rest.open),  tables(), actionsLog(), numOfTables(rest.numOfTables), line(rest.line), CustomerID(rest.CustomerID), input(rest.input){                           ///COPY CONSTRUCTOR

    for (Table* table: rest.tables) { //creates a new table and adds to the tables vector.
        Table* newTable = new Table(*table);
        tables.push_back(newTable);
    }

    for (BaseAction* action: rest.actionsLog) { //creates a new action a adds to the actionsLog vector.
        actionsLog.push_back(action->clone());
    }
}

Restaurant::Restaurant(const Restaurant &&rest): menu(), open(rest.open),  tables(), actionsLog(), numOfTables(rest.numOfTables), line(rest.line), CustomerID(rest.CustomerID), input(rest.input){                         ///MOVE CONSTRUCTOR

    for (Table* table: rest.tables) {  //creates a new table and adds to the tables vector.
        Table* newTable = new Table(*table);
        tables.push_back(newTable);
    }

    for (BaseAction* action: rest.actionsLog) { //creates a new action a adds to the actionsLog vector.
        actionsLog.push_back(action->clone());
    }
}

Restaurant::Restaurant(const std::string &configFilePath):menu(), open(true), tables(), actionsLog(), numOfTables(0), line(""), CustomerID(0), input(""){
    std::ifstream infile;
    infile.open(configFilePath, ios::in); //reads the file.
    vector<string> txt_v;
    vector<int> seats;
    if (infile.is_open()) {
        string line;
        while(getline(infile,line)){
            if((line.find("#") == string::npos)&&(line.size() > 0) & (!(line == "\r"))){ //if the line does not consist the symbol '#' or the line is empty.
                txt_v.push_back(line);
            }
        }
        numOfTables = stoi(txt_v[0]);
        string line1=txt_v[1];
        for(int i=0;i<numOfTables;i++) {
            int comma = line1.find(','); // find the first comma in the line.
            if (comma != -1) { // if the comma exists.
                string temp = line1.substr(0, comma);
                line1 = line1.substr(comma + 1);
                seats.push_back(stoi(temp));
            } else {
                seats.push_back(stoi(line1));
            }
        }

        createTables(seats);
        createMenu(txt_v, 2);
    }

}


//----------Methods-----------//


void Restaurant::createTables(vector<int>& seats){
    for ( unsigned int j = 0; j < seats.size(); ++j) {
        Table* table = new Table(seats[j]); // creates a new table.
        tables.push_back(table);
        table->setId(j);
    }
}

int Restaurant::getNumOfTables() const{
    return numOfTables;
}

enum DishType Restaurant::getType(string s){
    if(s == "VEG")
        return VEG;
    else if(s=="SPC")
        return SPC;
    else if(s =="BVG")
        return BVG;
    else
        return ALC;
}

void Restaurant::createMenu(vector<string>& txt_v,int i){
    string item;
    DishType type;
    int cost;
    for(unsigned int j=i;j<txt_v.size();j++){
        string line =txt_v[j];

        for (int counter = 0;counter <=2; ++counter) { // divides the line by : (0) item (1) type (2) price.
            int comma = line.find(',');

            if (comma != -1) {
                string temp = line.substr(0, comma);

                if(counter == 0)
                    item = temp;

                else if (counter == 1){
                    type = getType(temp);
                }
                line = line.substr(comma + 1);
            }
            else
                cost = stoi(line);
        }
        Dish dish(j-i,item,cost,type);
        menu.push_back(dish);
    }
}

vector<Dish>& Restaurant::getMenu(){
    return menu;
}


void Restaurant::start() {
    string txt;
    cout << "Restaurant is now open!" << endl;
    getline(cin,txt);
    input = txt;
    while (!(txt == "closeall")) {
        int space = txt.find(" ");
        string action = txt.substr(0, space);
        txt = txt.substr(space + 1);

        if (action == "open") {
            int space = txt.find(" ");
            int tableId = stoi(txt.substr(0, space));
            line = txt.substr(space + 1);
            BaseAction* open = new OpenTable(tableId, tables.at(tableId)->getCustomers());
            open->setArgument(input);
            open->act(*this);
            actionsLog.push_back(open);
        }

        else if(action == "order"){
            int table = stoi(txt);
            BaseAction *order = new Order(table);
            order->setArgument(input);
            if(menu.size()>0) {
                order->act(*this);
            }
            actionsLog.push_back(order);
        }

        else if(action == "move"){
            int src;
            int dst;
            int id;
            int space = txt.find(" ");
            src = stoi(txt.substr(0,space));
            txt = txt.substr(space+1);
            space = txt.find(" ");
            dst = stoi(txt.substr(0,space));
            id = stoi(txt.substr(space+1));
            BaseAction *move = new MoveCustomer(src,dst,id);
            move->setArgument(input);
            move->act(*this);
            actionsLog.push_back(move);
        }

        else if(action =="menu"){
            BaseAction *menu = new PrintMenu();
            menu->act(*this);
            menu->setArgument(input);
            actionsLog.push_back(menu);
        }

        else if(action =="status"){
            int space = txt.find(" ");
            int id = stoi(txt.substr(space+1));
            BaseAction *status = new PrintTableStatus(id);
            status->act(*this);
            status->setArgument(input);
            actionsLog.push_back(status);

        }

        else if (action =="close"){
            int space = txt.find(" ");
            int id = stoi(txt.substr(space+1));
            BaseAction *close = new Close(id);
            close->act(*this);
            close->setArgument(input);
            actionsLog.push_back(close);
        }

        else if(action == "log"){
            BaseAction *Log = new PrintActionsLog();
            Log->act(*this);
            Log->setArgument(input);
            actionsLog.push_back(Log);
        }

        else if(action == "backup"){
            BaseAction *backing_up = new BackupRestaurant();
            backing_up->act(*this);
            actionsLog.push_back(backing_up);
            backing_up->setArgument(input);
        }

        else if(action == "restore"){
            BaseAction *restoring = new RestoreResturant();
            restoring->act(*this);
            restoring->setArgument(input);
            actionsLog.push_back(restoring);
        }

        getline(cin,txt);
        input = txt;
    }

    BaseAction *closeAll = new CloseAll();
    closeAll->act(*this);
    delete closeAll;
    //closeAll= nullptr;
}


Table* Restaurant::getTable(int ind){
    return tables.at(ind);
}
string Restaurant::getLine(){
    return line;
}

void Restaurant::createCustomer(string c_line,int tableId) {
    string name;
    Customer *newCustomer;
    int comma = c_line.find(","); //find the division between the name and the dish type.

    if (comma != -1) {
        name = c_line.substr(0, comma);
        string type = c_line.substr(comma + 1);

        if (type == "veg") {
            newCustomer = new VegetarianCustomer(name, CustomerID); //if the customer is vegetarian.

        } else if (type == "chp") {
            newCustomer = new CheapCustomer(name, CustomerID);//if the customer is cheap.

        } else if (type == "spc") {
            newCustomer = new SpicyCustomer(name, CustomerID);//if the customer eats spicy food.

        } else {
            newCustomer = new AlchoholicCustomer(name, CustomerID);//if the customer is an alcoholic.

        }
        tables[tableId]->addCustomer(newCustomer);
        CustomerID = CustomerID + 1;
    }
}


const vector<BaseAction*>& Restaurant::getActionsLog() const{
    return actionsLog;
}

vector<Table*>& Restaurant::getTables(){
    return tables;
}


//----------------CopyAssignmentOperator---------------//


Restaurant& Restaurant::operator=(const Restaurant &rest) {
    if (this == &rest) {
        return (*this);
    }
    else {
        (*this).open = rest.open;
        (*this).numOfTables = rest.numOfTables;
        (*this).CustomerID = rest.CustomerID;
        for (Table *table: tables) { //erasing the previous tables in the vest of this Restaurant.
            if (table != nullptr)
                delete table;
        }
        tables.clear();

        for (Table* table: rest.tables) {
            Table* newTable = new Table(*table);
            tables.push_back(newTable);
        }

        for (BaseAction* action: actionsLog) { //erasing the previous tables in the vest of this Restaurant.
            if (action != nullptr)
                delete action;
        }
        actionsLog.clear();
        for (BaseAction* action: rest.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        return (*this);
    }
}

void Restaurant::setclose(){
    open= false;
}


//----------------MoveAssignmentOperator---------------//


Restaurant& Restaurant::operator=(const Restaurant &&rest) {
    if (this == &rest) {
        return (*this);
    }
    else {
        (*this).open = rest.open;
        (*this).numOfTables = rest.numOfTables;
        (*this).CustomerID = rest.CustomerID;
        for (Table *table: tables) { //erasing the previous tables in the vest of this Restaurant.
            if (table != nullptr)
                delete table;
        }
        tables.clear();

        for (Table* table: rest.tables) {
            Table* newTable = new Table(*table);
            tables.push_back(newTable);
        }

        for (BaseAction* action: actionsLog) { //erasing the previous tables in the vest of this Restaurant.
            if (action != nullptr)
                delete action;
        }
        actionsLog.clear();
        for (BaseAction* action: rest.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        return (*this);
    }
}


//--------------Destructor-------------//

Restaurant::~Restaurant(){
    for(Table* table:tables){
        delete table;
    }
    tables.clear();
    for(BaseAction* action:actionsLog){
        delete action;
    }
    actionsLog.clear();
}