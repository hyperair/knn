#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <parser.hh>

typedef std::map<knn::entry::index_type,
                 knn::entry::value_type> valuemap_type;
void knn::visit_file (const std::string &path,
                      const std::function<void (entry &&,
                                                dataset::class_type)> &visitor)
{
    std::ifstream file (path);

    if (!file) {
        throw file_open_exception (path);
    }

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
            char separator = ss_token.get ();
            (void) separator;
            assert (separator == ':');
            ss_token >> value;

            values.insert ({index, value});
        }

        visitor (entry (values), clss);
    }
}

knn::dataset knn::parse_file (const std::string &path)
{
    dataset retval;

    visit_file (path, [&] (entry &&entry, dataset::class_type clss)
                {
                    retval.insert (entry, clss);
                });

    const auto &dimensions = retval.dimensions ();
    retval.visit ([&] (entry &e, dataset::class_type)
                  {
                      e.expand_dimensions (dimensions.begin (),
                                           dimensions.end ());
                  });

    return std::move (retval);
}
