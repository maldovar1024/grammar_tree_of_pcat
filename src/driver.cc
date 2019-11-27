#include "driver/driver.hh"


int driver::parse(const std::string& f) {
    file = f;
    location.initialize(&file);
    
    begin_scan();

    yy::parser parse(*this);
    parse.set_debug_level(debug_mode);
    int res = parse.parse();
    
    end_scan();

    if (res == 0) {
        std::cout << "program ";
        program->print(std::cout, 2);
    }

    return res;
}
