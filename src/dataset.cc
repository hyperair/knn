#include <map>
#include <limits>
#include <dataset.hh>

using knn::dataset;

dataset::dataset () :
    _dimensions (new std::set<index_type>)
{}

dataset::dataset (const dataset &other) :
    entries (other.entries),
    _dimensions (new std::set<index_type> (*other._dimensions))
{}

dataset &dataset::operator= (dataset copy)
{
    entries = std::move (copy.entries);
    _dimensions = std::move (copy._dimensions);

    return *this;
}

void dataset::insert (entry e, class_type clss)
{
    e.visit ([&] (index_type idx, value_type) {_dimensions->insert (idx);});
    entries.push_back (std::make_pair (std::move (e), clss));
}

std::ostream &knn::operator<< (std::ostream &out, const dataset &data)
{
    data.visit ([&] (const entry &e, dataset::class_type clss)
                {
                    out << clss << ' ' << e << std::endl;
                });
    return out;
}
