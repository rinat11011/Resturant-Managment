#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include <vector>
using namespace std;

class Customer{
public:
    Customer(std::string c_name, int c_id);
	virtual Customer* clone()=0;
	virtual ~Customer()=0;
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;

private:
    const std::string name;
    const int id;

};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    virtual Customer* clone();
	std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    virtual Customer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
	bool ordered=false;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    virtual Customer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
	bool orderedDish=false;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
    virtual Customer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    void setAlcVector(vector<Dish> &menu);
	std::string toString() const;
private:
	int lastDish = -1;
	int lastDishPrice = -1;

};


#endif