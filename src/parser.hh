
#ifndef PARSER_HH
#define PARSER_HH

#include <string>
#include <dataset.hh>
#include <entry.hh>

namespace knn
{
    void visit_file (const std::string &path,
                     const std::function <void (std::map<entry::index_type,
                                                         entry::value_type> &&,
                                                dataset::class_type)> &visitor);

    dataset parse_file (const std::string &pathname);
}

#endif
