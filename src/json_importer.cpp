#include "bettertest_json/json_importer.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <fstream>

////////////////////////////////////////////////////////////////
// External includes.
////////////////////////////////////////////////////////////////

#include "nlohmann/json.hpp"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/exceptions/import_error.h"
#include "bettertest/suite/suite_data.h"
#include "bettertest/suite/test_data.h"
#include "bettertest/suite/test_suite.h"

namespace nlohmann
{
    // Function to handle pointer to TestData.
    template<typename T>
    struct adl_serializer<std::unique_ptr<T>>
    {
        static void to_json(json& j, const std::unique_ptr<T>& opt)
        {
            if (opt.get())
                j = *opt;
            else
                j = nullptr;
        }

        static void from_json(const json& j, std::unique_ptr<T>& opt)
        {
            if (j.is_null())
                opt = nullptr;
            else
                opt = std::make_unique<T>(j.get<T>());
        }
    };
}  // namespace nlohmann

namespace bt
{
    void from_json(const nlohmann::json& json, SuiteData& s)
    {
        json.at("dateCreated").get_to(s.dateCreated);
        json.at("dateLastRun").get_to(s.dateLastRun);
        json.at("name").get_to(s.name);
        json.at("passing").get_to(s.passing);
        json.at("runIndex").get_to(s.runIndex);
        json.at("version").get_to(s.version);
    }

    void from_json(const nlohmann::json& json, TestData& t)
    {
        json.at("dateCreated").get_to(t.dateCreated);
        json.at("dateLastRun").get_to(t.dateLastRun);
        json.at("name").get_to(t.name);
        json.at("passing").get_to(t.passing);
    }

    JsonImporter::JsonImporter(std::filesystem::path directory) : IImporter(std::move(directory)) {}

    bool JsonImporter::readSuite(TestSuite& suite)
    {
        if (!exists(path / "suite.json")) return false;

        // Read JSON data from file.
        std::ifstream file(path / "suite.json");
        if (!file) throw ImportError("Failed to open suite file");
        nlohmann::json data;
        file >> data;
        file.close();

        // Read suite data.
        if (const auto it = data.find("suite"); it != data.end()) it->get_to(suite.getData());

        // Read unit test states.
        if (const auto it0 = data.find("unitTests"); it0 != data.end()) it0->get_to(suite.getUnitTestSuite().getData());

        return true;
    }
}  // namespace bt
