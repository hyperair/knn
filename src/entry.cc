#include <utility>
#include <entry.hh>

using knn::entry;
using knn::invalid_dimension;

entry::entry (const std::set<int> &dimensions,
              std::map<int, double> values) :
    values (std::move (values)),
    dimensions (dimensions)
{}

entry::value_type entry::operator[] (const index_type index) const
{
    if (dimensions.find (index) == dimensions.end ())
        throw invalid_dimension (index);

    auto iter = values.find (index);
    if (iter != values.end ())
        return iter->second;

    else
        return 0;
}

entry::value_type &entry::operator[] (const index_type index)
{
    if (dimensions.find (index) == dimensions.end ())
        throw invalid_dimension (index);

    return values[index];
}


invalid_dimension::invalid_dimension (const entry::index_type dimension) :
    _dimension (dimension)
{}

const char *invalid_dimension::what () const throw ()
{
    return "Attempted to access dimension not present in data set";
}
