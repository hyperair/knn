#include <utility>
#include <entry.hh>

using knn::entry;
using knn::invalid_dimension;

entry::entry (std::map<int, double> values) :
    values (std::move (values))
{}

entry::value_type entry::operator[] (const index_type index) const
{
    auto iter = values.find (index);

    if (iter == values.end ())
        throw invalid_dimension (index);

    return iter->second;
}

entry::value_type &entry::operator[] (const index_type index)
{
    if (values.find (index) == values.end ())
        throw invalid_dimension (index);

    return values[index];
}

void entry::visit (const std::function<void (index_type, value_type)> &functor)
    const
{
    for (auto i : values)
        functor (i.first, i.second);
}

std::ostream &knn::operator<< (std::ostream &out, const entry &e)
{
    e.visit ([&] (const entry::index_type index, const entry::value_type value)
             {
                 out << index << ':' << value << ' ';
             });
    return out;
}


invalid_dimension::invalid_dimension (const entry::index_type dimension) :
    _dimension (dimension)
{}

const char *invalid_dimension::what () const throw ()
{
    return "Attempted to access dimension not present in data set";
}
