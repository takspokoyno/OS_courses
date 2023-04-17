#include <unistd.h>

#include <iostream>

char data[] = "Aboba excellent";

int main() {
    while(true){
        std::cout << "pid: " << getpid() << "\nData address: " << (void *)data << "\nData size: " << sizeof(data) << "\n";
	std::cout << "Data: " << data << "\n";
    	std::cout << "Enter new data: ";
	std::cin.getline(data, sizeof(data));
	std::cout << "\n";
	}
    return 0;
}
