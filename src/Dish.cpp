#include "../include/Dish.h"
#include <iostream>
#include <string>



//--------Constructor---------//


Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type){}


//----------Methods-----------//


int Dish::getId() const{
    return id;
}

std::string Dish::getName() const{
    return name;
}

int Dish::getPrice() const{
    return price;
}

DishType Dish::getType() const{
    return type;
}

std::string Dish::enumType(){
    if(type==0) {
        return "VEG";
    }
    else if(type==1){
        return "SPC";
    }
    else if(type==2){
        return "BVG";
    }
    else{
       return "ALC";
    }
}