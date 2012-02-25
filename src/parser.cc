#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <parser.hh>

typedef std::map<knn::entry::index_type,
                 knn::entry::value_type> valuemap_type;
void knn::visit_file (const std::string &path,
                      const std::function<void (valuemap_type &&,
                                                dataset::class_type)> &visitor)
{
    std::ifstream file (path);
    std::string line;

    while (getline (file, line)) {
        std::istringstream ss (line);

        valuemap_type values;

        dataset::class_type clss;
        ss >> clss;

        std::string token;
        while (ss >> token) {
            std::istringstream ss_token (token);

            entry::index_type index;
            entry::value_type value;

            ss_token >> index;
            assert (ss_token.get () == ':');
            ss_token >> value;

            values.insert ({index, value});
        }

        visitor (std::move (values), clss);
    }
}

knn::dataset knn::parse_file (const std::string &path)
{
    dataset retval;
    auto &dimensions = retval.dimensions ();

    visit_file (path, [&] (valuemap_type &&values, dataset::class_type clss)
                {
                    retval.insert ({dimensions, values}, clss);
                });

    return std::move (retval);
}
