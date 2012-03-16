#include <utility>
#include <cassert>
#include <entry.hh>

using knn::entry;

entry::entry (std::unordered_map<int, double> values) :
    values (std::move (values))
{}

entry::value_type entry::operator[] (const index_type index) const
{
    auto iter = values.find (index);

    assert (iter != values.end ());

    return iter->second;
}

entry::value_type &entry::operator[] (const index_type index)
{
    assert (values.find (index) != values.end ());

    return values[index];
}

std::ostream &knn::operator<< (std::ostream &out, const entry &e)
{
    e.visit ([&] (const entry::index_type index, const entry::value_type value)
             {
                 out << index << ':' << value << ' ';
             });
    return out;
}
