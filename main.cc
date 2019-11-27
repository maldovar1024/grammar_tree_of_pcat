#include <iostream>
#include <string>
#include "driver/driver.hh"


int main(int argc, char const *argv[]) {
    using namespace std;
    using namespace std::string_literals;

    string file = argv[1];
    driver drv;
    
    if (argc == 3 && argv[2] == "-d"s) {
        drv.debug_mode = true;
    }

    int res = drv.parse(file);
    
    return res;
}


