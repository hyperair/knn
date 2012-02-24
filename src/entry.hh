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
        entry (const std::set<int> &dimensions,
               std::map<int, double> values);
        double operator[] (int index) const;

    private:
        const std::map<int, double>  values;
        const std::set<int>         &dimensions;
    };

    class invalid_dimension : public std::exception
    {
    public:
        invalid_dimension (int dimension);

        // override from std::exception
        virtual const char *what () const throw ();
        int dimension () const {return _dimension;}

    private:
        const int _dimension;
    };
}

#endif  // ENTRY_HH
