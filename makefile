# All Targets
all: rest

# Tool invocations
# Executable "rest" depends on the files Dish.o, Customer.o, Main.o, Restaurant.o, Table.o 
rest: bin/Dish.o bin/Customer.o bin/Main.o bin/Restaurant.o bin/Table.o bin/Customer.o bin/Action.o bin/Restaurant.o 
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/rest bin/*
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files

# Depends on the source and header files
bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files
bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files
bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files
bin/Table.o: src/Table.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp
	
# Depends on the source and header files
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

#Clean the build directory
clean:
	rm -f bin/*

