# Resturant-Managment 

Resturant managment is a C++ program that simulates a restaurant management system. 

The program will open the restaurant, assign customers to tables, make orders, provide bills to the tables, and other requests as described below.

The program will get a config file as an input, which includes all required information about the restaurant opening such as:
* Number of tables
* Number of available seats in each table
* Details about the dishes in the menu. 

Format of the input file:

* Parameter 1: number of tables in the restaurant.
* Parameter 2: a list of tables capacities (maximum seats available) separated by comma: <table 1 number of seats>,<table 2 number of seats> …
* Parameter 3: a list of dishes, each dish in a separate line, including dish name, dish type (3-letters code: Vegetarian – VEG, Beverage – BVG, Alcoholic – ALC, Spicy – SPC) and a price separated by comma:
<dish name>,<dish type>,<dish price>

Each table in the restaurant has a limited amount of seats available (this info is provided in the config file). 
The restaurant can’t connect tables together, nor accommodates more customers than the number of seats available in a table. 
In this restaurant, it’s impossible to add new customers to an open table, but it’s possible to move customers from one table to another.
A bill of a table is the total price of all dishes ordered for that table. 
