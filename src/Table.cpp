#include "../include/Table.h"
#include "../include/Customer.h"
#include <iostream>

using namespace std;


//--------Constructor---------//


Table::Table(int t_capacity):capacity(t_capacity), id() , open(false), numOfCustomers(0) , customersList() , orderList(){}

Table::Table(const Table &table):capacity(table.capacity), id(table.id) , open(table.open), numOfCustomers(table.numOfCustomers) , customersList() , orderList(){                                       ///COPY CONSTRUCTOR

    for (Customer* customer: table.customersList) {
        customersList.push_back(customer->clone());
    }
    for (OrderPair pair: table.orderList) {
        OrderPair newPair(pair);
        orderList.push_back(newPair);
    }
}

Table::Table(const Table &&table):capacity(table.capacity), id(table.id) , open(table.open), numOfCustomers(table.numOfCustomers) , customersList() , orderList(){                                     ///MOVE CONSTRUCTOR

    for (Customer* customer: customersList) {
        customersList.push_back(customer->clone());
    }
    for (OrderPair pair: table.orderList) {
        OrderPair newPair(pair);
        orderList.push_back(newPair);
    }
}

//Table* Table::clone(){
//    return new Table(*this);
//}


//----------Methods----------//


int Table::getCapacity() const{
    return capacity;
}

void Table::addCustomer(Customer* customer){
    customersList.push_back(customer);
    addtoNumOfCustomers();
}

void Table::removeCustomer(int id){
    changeOrderList(id); //change the order list to be without the orders of the customer.
    int placing;
    for (unsigned int i=0; i<customersList.size(); i++) { //search for the placing of the customer in the vector.
        if (customersList[i]->getId() == id){
            placing = i;
            break;
        }
    }
    customersList.erase(customersList.begin()+placing); //erase the pointer of the customer from the vector
    numOfCustomers--; //lower the number of customers sitting at the table.
}

Customer* Table::getCustomer(int id){
    int placing = -1;
    for (unsigned int i=0; i<customersList.size(); i++) { //search for the placing of the customer in the vector.
        if ((customersList[i] != nullptr)&&(customersList[i]->getId() == id)) {
            placing = i;
            break;
        }
    }
    if(placing == -1){
        return nullptr;
    }
    return customersList[placing];
}

vector<Customer*>& Table::getCustomers(){
    return customersList;
}

vector<OrderPair>& Table::getOrders(){
    return orderList;
}

void Table::order(const vector<Dish> &menu){
    for(Customer* customer: customersList){
        vector<int> customersDish = customer->order(menu); //each customers orders according to his dish type.
        for(int i:customersDish){
            Dish dish = menu[i];
            OrderPair pair(customer->getId(),dish);
            orderList.push_back(pair);
            cout << customer->getName() << " ordered " << dish.getName() << endl;
        }
    }
}

void Table::openTable(){
    setOpen(true);
}

void Table::closeTable(){
    for (Customer* customer : customersList) {
        if(customer != nullptr)
            delete customer;
    }
    customersList.clear();
    orderList.clear();
    setOpen(false);
}

int Table::getBill(){
    int bill=0;
    for (pair<int,Dish> item: orderList) { //iterate through the order list and for every dish add the price to the bill.
        bill+=item.second.getPrice();
    }
    return bill;
}

bool Table::isOpen(){
    return open; //returns open boolean.
}

void Table::setOpen(bool boolean){
    this->open = boolean; //change the boolean open.
}
void Table::addtoNumOfCustomers(){
    numOfCustomers++; //add the number of customers by 1.
}
int Table::getNumOf(){
    return numOfCustomers; //get the number of updated customers.
}

void Table::changeOrderList(int id){
    vector<OrderPair> newOrderList; //create a new list
    for (OrderPair pair: orderList) { //add to the new list all the orders from all the different customers.
        if(pair.first != id){
            newOrderList.push_back(pair);
        }
    }
    orderList.clear(); //erase the previous vector
    for(OrderPair pair:newOrderList){
        orderList.push_back(pair);//add the new vector which consists of all the orders of the customers except the orders from the customer with the id, as the new vector.
    }
}
void Table::setId(int idt){
    id = idt;
}
int Table::getId(){
    return id;
}


//----------------CopyAssignmentOperator-------------------//


Table& Table::operator=(const Table& table) { //ASSIGNMENT OPERATOR
    if (this == &table) {
        return (*this);
    }
    else {
        (*this).open = table.open;
        (*this).numOfCustomers = table.numOfCustomers;
        (*this).id = id;
        (*this).capacity = capacity;

        for (Customer *customer: customersList) { //erasing the previous tables in the vest of this Restaurant.
            if (customer != nullptr)
                delete customer;
        }
        customersList.clear();
        for (Customer *customer: table.customersList) {
            customersList.push_back(customer->clone());
        }

        orderList.clear();
        for (OrderPair pair: table.orderList)
            orderList.push_back(pair);

        return (*this);
    }
}

//----------------MoveAssignmentOperator-------------------//

Table& Table::operator=(const Table&& table) { //ASSIGNMENT OPERATOR
    if (this == &table) {
        return (*this);
    }
    else {
        (*this).open = table.open;
        (*this).numOfCustomers = table.numOfCustomers;
        (*this).id = id;
        (*this).capacity = capacity;

        for (Customer *customer: customersList) { //erasing the previous tables in the vest of this Restaurant.
            if (customer != nullptr)
                delete customer;
        }
        customersList.clear();
        for (Customer *customer: table.customersList) {
            customersList.push_back(customer->clone());
        }

        orderList.clear();
        for (OrderPair pair: table.orderList)
            orderList.push_back(pair);

        return (*this);
    }
}

//----------------Destructor-------------------//

Table::~Table(){
    for(Customer *customer: customersList){
        delete customer;
    }
    customersList.clear();
    orderList.clear();
}