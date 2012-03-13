
#ifndef PARSER_HH
#define PARSER_HH

#include <string>
#include <exception>

#include <dataset.hh>
#include <entry.hh>

namespace knn
{
    void visit_file (const std::string &path,
                     const std::function <void (entry &&entry,
                                                dataset::class_type)> &visitor);

    dataset parse_file (const std::string &pathname);

    class file_open_exception : public std::exception
    {
    public:
        file_open_exception (const std::string &filename) :
            msg ("Could not open file: ")
        {msg += filename;}

        virtual ~file_open_exception () throw () {}

        virtual const char *what () const throw () {return msg.c_str ();}

    private:
        std::string msg;
    };
}

#endif
