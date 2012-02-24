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
        entry (const std::set<double> &dimensions,
               std::map<double, double> values);
        double operator[] (double index) const;

    private:
        const std::map<double, double>  values;
        const std::set<double>         &dimensions;
    };

    class invalid_dimension : public std::exception
    {
    public:
        invalid_dimension (double dimension);

        // override from std::exception
        virtual const char *what () const throw ();
        double dimension () const {return _dimension;}

    private:
        const double _dimension;
    };
}

#endif  // ENTRY_HH
