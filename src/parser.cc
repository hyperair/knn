#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <parser.hh>

knn::dataset knn::parse_file (const std::string &path)
{
    dataset retval;
    auto &dimensions = retval.dimensions ();

    std::ifstream file (path);
    std::string line;

    while (getline (file, line)) {
        std::istringstream ss (line);

        std::map<entry::index_type, entry::value_type> values;

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

        retval.insert ({dimensions, std::move (values)}, clss);
    }

    return std::move (retval);
}
