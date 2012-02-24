#include <dataset.hh>

using knn::dataset;

void dataset::insert (entry e, int clss)
{
    entries.push_back (std::make_pair (std::move (e), clss));
}

void
dataset::visit (const std::function<void (const entry &, int)> &functor) const
{
    for (const std::pair<entry, int> &i : entries)
        functor (i.first, i.second);
}
