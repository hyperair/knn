#include <utility>
#include <entry.hh>

using knn::entry;
using knn::invalid_dimension;

entry::entry (const std::set<double> &dimensions,
              std::map<double, double> values) :
    values (std::move (values)),
    dimensions (dimensions)
{}

double entry::operator[] (const double index) const
{
    if (dimensions.find (index) == dimensions.end ())
        throw invalid_dimension (index);

    auto iter = values.find (index);
    if (iter != values.end ())
        return iter->second;

    else
        return 0;
}


invalid_dimension::invalid_dimension (const double dimension) :
    _dimension (dimension)
{}

const char *invalid_dimension::what () const throw ()
{
    return "Attempted to access dimension not present in data set";
}
