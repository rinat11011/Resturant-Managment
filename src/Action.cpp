#include "../include/Action.h"
#include <iostream>
#include <string>
#include <vector>
#include "../include/Restaurant.h"
#include "../include/Table.h"
extern Restaurant* backup;
using namespace std;


//---------------Constructor--------------//


BaseAction::BaseAction(): argument(),errorMsg(),status(PENDING){
}

BaseAction* BaseAction::clone(){return 0;}

BaseAction::~BaseAction(){}

//-----------------Methods-------------------//


ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::act(Restaurant& restaurant) {}

string BaseAction::toString() const{
    return 0;
}

void BaseAction::setArgument(string input){
    argument = input;
}

string BaseAction::getArgument(){
    return argument;
}

void BaseAction::complete(){
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    setErrorMsg(errorMsg);
    status = ERROR;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

void BaseAction::setErrorMsg(string msg){
    errorMsg = msg;
}


//------------------Open Table------------------//


OpenTable::OpenTable(int id, std::vector<Customer*> &customersList):BaseAction(), tableId(id),customers(){}



OpenTable::OpenTable(const OpenTable &otherTable): BaseAction(),tableId(otherTable.tableId),customers(){                                         ///COPY CONSTRUCTOR
    argument = otherTable.argument;
    if(otherTable.getStatus() == COMPLETED){
        complete();
    }
    else{
        error(otherTable.getErrorMsg());
    }
    for (Customer* customer: otherTable.customers) {
        customers.push_back(customer->clone());
    }
}

OpenTable::OpenTable(const OpenTable &&otherTable): BaseAction(),tableId(otherTable.tableId),customers(){                                        ///MOVE CONSTRUCTOR
    argument = otherTable.argument;
    if(otherTable.getStatus() == COMPLETED){
        complete();
    }
    else{
        error(otherTable.getErrorMsg());
    }
    for (Customer* customer: otherTable.customers) {
        customers.push_back(customer->clone());
    }
}

BaseAction* OpenTable::clone(){
    return new OpenTable(*this);
}

OpenTable::~OpenTable(){                                                                                                ///DESTRUCTOR
    for(Customer* customer:customers){
        delete customer;
    }
    customers.clear();
}

void OpenTable::act(Restaurant &restaurant) {
    if ((restaurant.getNumOfTables() - 1 < tableId) || (0 > tableId)|| (restaurant.getTable(tableId)->isOpen())) {// if the number of table is greater than the actual tables in the restaurant ot the table is already open
        cout << "Error: Table does not exist or is already open" << endl;
        error("Table does not exist or is already open");
    }
    else {
        complete();
        restaurant.getTable(tableId)->openTable(); //set the table to be open.
        string txt = restaurant.getLine();
        string newCustomer;
        while ((txt.size() > 0) && (!(txt == "\r"))) { //while the txt is not empty
            int space = txt.find(" "); //find the first space.
            if (space != -1) { /*as long as there is a space.*/
                newCustomer = txt.substr(0, space);
                txt = txt.substr(space + 1);
            }
            else {
                newCustomer = txt;
                txt ="";
            }
            restaurant.createCustomer(newCustomer, tableId); //create the customer
        }
    }
}


string OpenTable::toString() const{
    return getErrorMsg();
}


//-------------------Order-------------------//


Order::Order(int id):BaseAction(), tableId(id){}

Order::~Order(){}

BaseAction* Order::clone(){
    return new Order(*this);
}

void Order::act(Restaurant &restaurant){
    if ((restaurant.getNumOfTables() - 1 < tableId) || (0 > tableId)|| (!restaurant.getTable(tableId)->isOpen())) {
        cout << "Error: Table does not exist or is not open" << endl;
        error("Table does not exist or is not open");
    }
    else {
        complete();
        restaurant.getTable(tableId)->order(restaurant.getMenu());
    }
}

string Order::toString() const{
    return getErrorMsg();
}


//---------------------Move Customer-------------------//


MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTable(src),dstTable(dst),id(customerId){}

BaseAction* MoveCustomer::clone(){
    return new MoveCustomer(*this);
}

MoveCustomer::~MoveCustomer() {}

void MoveCustomer::act(Restaurant &restaurant) {

    if((restaurant.getNumOfTables() - 1 >= srcTable) && (srcTable >= 0) && (restaurant.getNumOfTables() - 1 >= dstTable) && (dstTable >= 0)) {
        Table *src_table = restaurant.getTable(srcTable);
        Table *dst_table = restaurant.getTable(dstTable);   //if: there is no such table(destination or source), the table is not opened, there is no such customer, the table is in full capacity - then return an error.
        if ((src_table->getCustomer(id) == nullptr) || (!src_table->isOpen()) || (!dst_table->isOpen()) || (dst_table->getCapacity() == dst_table->getNumOf()))
        {
            cout << "Error: Cannot move customer" << endl;
            error("Cannot move customer");
        }
        else{
            complete();
            for(OrderPair pair:src_table->getOrders()){// search for the dishes of the customer and push them into the new table orderlist.
                if(pair.first==id)
                    dst_table->getOrders().push_back(pair);
            }
            Customer *customer = src_table->getCustomer((id));//hold the pointer of the customer in the memory.
            dst_table->addCustomer(customer); //add the pointer to the customer list of the destination table
            src_table->removeCustomer(id); // remove the pointer from the source table.
            if(src_table->getCustomers().empty()){
                src_table->closeTable();
            }
        }
    }
    else{
        cout << "Error: Cannot move customer" << endl;
        error("Cannot move customer");
    }

}

string MoveCustomer::toString() const{
    return getErrorMsg();
}


//---------------------Close---------------------//


Close::Close(int id):BaseAction(), tableId(id) {}

BaseAction* Close::clone(){
    return new Close(*this);
}

Close::~Close(){}

void Close::act(Restaurant &restaurant) {
    if ((restaurant.getNumOfTables() - 1 < tableId) || (tableId < 0) || (!restaurant.getTable(tableId)->isOpen())) {
        cout << "Error: Table does not exist or is not open" << endl;
        error("Table does not exist or is not open" );
    }
    else {
        int bill = restaurant.getTable(tableId)->getBill();
        cout << "Table " << tableId << " was closed. Bill " << bill << "NIS" << endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

string Close::toString() const{
    return getErrorMsg();
}


//-------------------CloseAll------------------//


CloseAll::CloseAll():BaseAction(){}

BaseAction* CloseAll::clone(){
    return new CloseAll(*this);
}

CloseAll::~CloseAll(){}

void CloseAll::act(Restaurant &restaurant){
    for(Table* table:restaurant.getTables()){
        if(table->isOpen()) {
            cout << "Table " << table->getId() << " was closed. Bill " << table->getBill() << "NIS" << endl;
            table->closeTable();
        }
    }
    complete();
    restaurant.setclose();
}

string CloseAll::toString() const{
    return getErrorMsg();
}


//------------------PrintMenu------------------//


PrintMenu::PrintMenu():BaseAction(){}

BaseAction* PrintMenu::clone(){
    return new PrintMenu(*this);
}
PrintMenu::~PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant){
    complete();
    for (Dish dish: restaurant.getMenu()) {
        cout << dish.getName() << " " << dish.enumType() << " " << dish.getPrice() << "NIS" << endl;
    }
}

string PrintMenu::toString() const{
    return getErrorMsg();
}


//----------------PrintTableStatus-------------------//


PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id){}

BaseAction* PrintTableStatus::clone(){
    return new PrintTableStatus(*this);
}

PrintTableStatus::~PrintTableStatus() {}

void PrintTableStatus::act(Restaurant &restaurant){
    complete();
    cout << "Table " << tableId << " status: ";

    if ((restaurant.getTable(tableId)!= nullptr) &&(restaurant.getTable(tableId)->isOpen())){
        cout << "open" << endl;
        cout << "Customers:" << endl;

        for( Customer* customer : restaurant.getTable(tableId)->getCustomers()){
            cout << customer->getId() << " " << customer->getName() << endl;
        }


        cout << "Orders:" << endl;

        for (OrderPair pair : restaurant.getTable(tableId)->getOrders()) {
            cout << pair.second.getName() << " " << pair.second.getPrice() << "NIS " << pair.first << endl;
        }

        cout << "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << endl;
    }
    else{
        cout << "closed"<< endl;
    }
}

string PrintTableStatus::toString() const{
    return getErrorMsg();
}


//--------------------PrintActionsLog------------------//


PrintActionsLog::PrintActionsLog():BaseAction(){}

BaseAction* PrintActionsLog::clone(){
    return new PrintActionsLog(*this);
}

PrintActionsLog::~PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant){
    complete();
    for (BaseAction* action : restaurant.getActionsLog()) {
        cout << action->getArgument() << " ";
        if(action->getStatus() == 0) {
            cout << "Pending" << endl;
        }
        else if(action->getStatus() == 1){
            cout << "Completed" << endl;
        }
        else{
            cout << "Error: " << action->toString() << endl;
        }
    }
}

string PrintActionsLog::toString() const{
    return getErrorMsg();
}


//--------------------BackupRestaurant---------------------//


BackupRestaurant::BackupRestaurant():BaseAction(){}

BaseAction* BackupRestaurant::clone(){
    return new BackupRestaurant(*this);
}

BackupRestaurant::~BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    complete();
    if (backup != nullptr){
        delete backup;
    }
    backup = new Restaurant(restaurant);

}

string BackupRestaurant::toString() const{
    return getErrorMsg();
}


//------------------RestoreRestaurant------------------//


RestoreResturant::RestoreResturant():BaseAction(){}

BaseAction* RestoreResturant::clone(){
    return new RestoreResturant(*this);
}

RestoreResturant::~RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant){

    if(backup != nullptr){
        complete();
        restaurant = *backup;
    }
    else{
        cout << "Error: No backup available" << endl;
        error("No backup available");
    }
}

string RestoreResturant::toString() const{
    return getErrorMsg();
}
