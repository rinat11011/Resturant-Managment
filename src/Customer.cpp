#include "../include/Customer.h"
#include "../include/Dish.h"
#include "../include/Restaurant.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


//--------Constructor---------//


Customer::Customer(string c_name, int c_id):name(c_name),id(c_id){}

Customer* Customer::clone(){
    return 0;
}

Customer::~Customer(){}

//----------Methods----------//

string Customer::getName() const{return name;}

int Customer::getId() const{return id;}


//--------Vegetarian Customer---------//


VegetarianCustomer::VegetarianCustomer(string name, int id):Customer(name,id){}

Customer* VegetarianCustomer::clone(){
    return new VegetarianCustomer(*this);
}

vector<int> VegetarianCustomer::order(const vector<Dish> &menu){
    int maxValue=0;
    int id_veg_dish=-1;
    int id_bvg=-1;
    vector<int> returnVec;
    bool orderedFood=false;
    bool orderedBVG=false;

    for (Dish dish: menu) {
        if ((id_veg_dish == -1) && (dish.getType() == 0)){ //if its a VEG dish and its the first dish to appear on the list(smallest id).
                id_veg_dish = dish.getId();
                orderedFood=true;
        }
        else if((dish.getType()== 2)){ // if its a BVG dish.
            if(dish.getPrice() > maxValue){ // find the most expensive BVG dish.
                maxValue = dish.getPrice();
                id_bvg = dish.getId();
                orderedBVG=true;
            }
        }
    }
    if(orderedFood && orderedBVG) {
        returnVec.push_back(id_veg_dish);
        returnVec.push_back(id_bvg);
    }
    return returnVec;
}

string VegetarianCustomer::toString() const{
    return "Name: "+this->getName()+", Type of customer: VEG, ID: " + (this->getId()+"");
}


//--------Cheap Customer---------//


CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}

Customer* CheapCustomer::clone(){
    return new CheapCustomer(*this);
}

vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    vector<int> return_vec;
    int min_price = menu[0].getPrice(); // start from the first dish.
    int return_id = menu[0].getId();

    if (!ordered) {
        for (Dish dish:menu) {
            if (dish.getPrice() < min_price) { // find the cheapest dish.
                min_price = dish.getPrice();
                return_id = dish.getId();
            }
        }
        return_vec.push_back(return_id);
        ordered = true; // the customer can only order once.
    }
    return return_vec;
}

string CheapCustomer::toString() const{ return this->getId() + " " + this->getName();}


//--------Spicy Customer---------//


SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id){
}

Customer* SpicyCustomer::clone(){
    return new SpicyCustomer(*this);
}

vector<int> SpicyCustomer::order(const vector<Dish> &menu){
    vector<int> return_vec;
    int max_spc_dish=0;
    int spc_dish_id=-1;
    int min_bvg_price=numeric_limits<int>::max(); // set it to the highest number possible for int.
    int bvg_id;

    for (Dish dish: menu) {
        if(!orderedDish){ // if its the first time the customer ordered a dish.
            if((dish.getType()==1) && (max_spc_dish < dish.getPrice())){  // if its a SPC dish and its the most expensive one in the menu.
                max_spc_dish = dish.getPrice();
                spc_dish_id = dish.getId();

            }
        }
        else{
            if((dish.getType()== 2)&&(min_bvg_price > dish.getPrice())){ // find the cheapest BVG dish.
                min_bvg_price = dish.getPrice();
                bvg_id = dish.getId();
            }
        }
    }
    if (!orderedDish){
        if(spc_dish_id != -1) { // if its the first time, order SPC dish only.
            return_vec.push_back(spc_dish_id);
            orderedDish = true;
        }

    }
    else
        return_vec.push_back(bvg_id);

    return return_vec;
}

string SpicyCustomer::toString() const{ return "Name: "+this->getName()+", Type of customer: SPC, ID: " + (this->getId()+"");}


//--------Alcoholic Customer---------//


AlchoholicCustomer::AlchoholicCustomer(string name, int id):Customer(name,id){}

Customer* AlchoholicCustomer::clone(){
    return new AlchoholicCustomer(*this);
}

vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> return_vec;
    int min_alcId = -1;
    int min_price = numeric_limits<int>::max();// set it to the highest number possible for int.
    bool once = false;

    for (Dish dish:menu) {
        if (dish.getType() == 3) { // if its an ALC dish.
            if((!once) && (dish.getPrice() == lastDishPrice)){ // if its not the first order and the current dish's price is equal to the price of the last order
                if(dish.getId() > lastDish){ //check if the current dish's id is greater then the last ones
                    min_alcId = dish.getId();
                    min_price = dish.getPrice();
                    once = true;
                }

            }
            else if ((dish.getId() != lastDish) && (dish.getPrice() > lastDishPrice) && (dish.getPrice() < min_price)) {//find the next cheapest alc dish.
                min_alcId = dish.getId();
                min_price = dish.getPrice();
            }
        }
    }
    if (min_alcId != -1) {
        lastDishPrice = menu[min_alcId].getPrice();
        lastDish = min_alcId;
        return_vec.push_back(lastDish);
    }
    return return_vec;
}




string AlchoholicCustomer::toString() const { return "Name: "+this->getName()+", Type of customer: ALC, ID: " + (this->getId()+"");}
