#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    virtual BaseAction* clone()=0;
    virtual ~BaseAction()=0;
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    void setArgument(string input);
    string getArgument();
    void setErrorMsg(string msg);

protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    string argument;


private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    OpenTable(const OpenTable &otherTable);                                      ///COPY CONSTRUCTOR
	OpenTable(const OpenTable &&otherTable);                                     ///MOVE CONSTRUCTOR
	virtual BaseAction* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~OpenTable();
private:
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
	virtual BaseAction* clone();
    ~Order();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
	virtual BaseAction* clone();
	~MoveCustomer();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
	virtual BaseAction* clone();
	~Close();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
	virtual BaseAction* clone();
	~CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
	virtual BaseAction* clone();
	~PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
	virtual BaseAction* clone();
	~PrintTableStatus();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
	virtual BaseAction* clone();
	~PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
	virtual BaseAction* clone();
	~BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
	virtual BaseAction* clone();
	~RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;

};


#endif