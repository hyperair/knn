#ifndef PARSER_HH
#define PARSER_HH

#include <string>
#include <dataset.hh>

namespace knn
{
    dataset parse_file (const std::string &pathname);
}

#endif
