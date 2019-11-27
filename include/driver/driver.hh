#ifndef __DRIVER_HH__
#define __DRIVER_HH__

#include <string>
#include "parser.hh"

#define YY_DECL yy::parser::symbol_type yylex(driver& drv)

YY_DECL;

class driver {
public:
    int parse(const std::string& f);

    yy::location location;
    sptr<Body> program;

    void begin_scan();
    void end_scan();
    
    std::string file;
    bool debug_mode{false};
};

#endif // ! __DRIVER_HH__
