
#ifndef PARSER_HH
#define PARSER_HH

#include <string>
#include <dataset.hh>
#include <entry.hh>

namespace knn
{
    void visit_file (const std::string &path,
                     const std::function <void (entry &&entry,
                                                dataset::class_type)> &visitor);

    dataset parse_file (const std::string &pathname);
}

#endif
