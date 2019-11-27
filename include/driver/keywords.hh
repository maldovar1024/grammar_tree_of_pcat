#ifndef __KEYWOEDS_HH__
#define __KEYWOEDS_HH__
#include <map>
#include <string>
#include "parser.hh"


inline const std::map<std::string, yy::parser::symbol_type(*)(yy::parser::location_type)> keywords{
    { "AND",       yy::parser::make_AND       },
    { "ARRAY",     yy::parser::make_ARRAY     },
    { "BEGIN",     yy::parser::make_BEGIN     },
    { "BY",        yy::parser::make_BY        },
    { "DIV",       yy::parser::make_DIV       },
    { "DO",        yy::parser::make_DO        },
    { "ELSE",      yy::parser::make_ELSE      },
    { "ELSIF",     yy::parser::make_ELSIF     },
    { "END",       yy::parser::make_END       },
    { "EXIT",      yy::parser::make_EXIT      },
    { "FOR",       yy::parser::make_FOR       },
    { "IF",        yy::parser::make_IF        },
    { "IN",        yy::parser::make_IN        },
    { "IS",        yy::parser::make_IS        },
    { "LOOP",      yy::parser::make_LOOP      },
    { "MOD",       yy::parser::make_MOD       },
    { "NOT",       yy::parser::make_NOT       },
    { "OF",        yy::parser::make_OF        },
    { "OR",        yy::parser::make_OR        },
    { "OUT",       yy::parser::make_OUT       },
    { "PROCEDURE", yy::parser::make_PROCEDURE },
    { "PROGRAM",   yy::parser::make_PROGRAM   },
    { "READ",      yy::parser::make_READ      },
    { "RECORD",    yy::parser::make_RECORD    },
    { "RETURN",    yy::parser::make_RETURN    },
    { "THEN",      yy::parser::make_THEN      },
    { "TO",        yy::parser::make_TO        },
    { "TYPE",      yy::parser::make_TYPE      },
    { "VAR",       yy::parser::make_VAR       },
    { "WHILE",     yy::parser::make_WHILE     },
    { "WRITE",     yy::parser::make_WRITE     },
};


#endif // ! __KEYWOEDS_HH__

