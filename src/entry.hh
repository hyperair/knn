#ifndef ENTRY_HH
#define ENTRY_HH

#include <set>
#include <map>
#include <exception>

namespace knn
{
    class entry
    {
    public:
        typedef int index_type;
        typedef double value_type;

        entry (const std::set<index_type> &dimensions,
               std::map<index_type, value_type> values);

        value_type operator[] (index_type index) const;
        value_type &operator[] (index_type index);

    private:
        std::map<index_type, value_type>  values;
        const std::set<index_type>         &dimensions;
    };

    class invalid_dimension : public std::exception
    {
    public:
        invalid_dimension (entry::index_type dimension);

        // override from std::exception
        virtual const char *what () const throw ();
        entry::index_type dimension () const {return _dimension;}

    private:
        const entry::index_type _dimension;
    };
}

#endif  // ENTRY_HH
