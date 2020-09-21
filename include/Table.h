#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    Table* clone();
    Table(const Table &table);                  //COPY CONSTRUCTOR
    Table(const Table &&table);                //MOVE CONSTRUCTOR
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    void setOpen(bool boolean);
    void addtoNumOfCustomers();
    int getNumOf();
    void changeOrderList(int id);
    void setId(int idt);
    int getId();
    ~Table();
    Table& operator=(const Table& table);
    Table& operator=(const Table&& table);
private:
    int capacity;
    int id;
    bool open;
    int numOfCustomers;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif