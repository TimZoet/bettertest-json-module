#include "bettertest_json/json_exporter.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <format>
#include <fstream>
#include <source_location>

////////////////////////////////////////////////////////////////
// External includes.
////////////////////////////////////////////////////////////////

#include "nlohmann/json.hpp"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/exceptions/export_error.h"
#include "bettertest/suite/suite_data.h"
#include "bettertest/suite/test_suite.h"

namespace nlohmann
{
    template<>
    struct adl_serializer<std::source_location>
    {
        static void to_json(json& j, const std::source_location& loc)
        {
            j["column"]   = loc.column();
            j["line"]     = loc.line();
            j["fileName"] = loc.file_name();
        }
    };
}  // namespace nlohmann

namespace bt
{
    void to_json(nlohmann::json& json, const TestSuite& suite)
    {
        json = nlohmann::json{{"dateCreated", suite.getData().dateCreated},
                              {"dateLastRun", suite.getData().dateLastRun},
                              {"name", suite.getName()},
                              {"passing", suite.getData().passing},
                              {"runIndex", suite.getData().runIndex},
                              {"version", suite.getData().version}};
    }

    void to_json(nlohmann::json& json, const TestDataPtr& state)
    {
        json = nlohmann::json{{"dateCreated", state->dateCreated},
                              {"dateLastRun", state->dateLastRun},
                              {"name", state->name},
                              {"passing", state->passing}};
    }

    void to_json(nlohmann::json& json, const Result& res)
    {
        switch (res.status)
        {
        case result_t::success: json["status"] = "success"; break;
        case result_t::failure: json["status"] = "failure"; break;
        case result_t::exception: json["status"] = "exception"; break;
        }
        json["location"] = res.location;
        json["error"]    = res.error;
    }

    void to_json(nlohmann::json& json, const IMixinResultsGetter& getter)
    {
        // Write global stats.
        json["stats"] = {{"total", getter.getTotal()},
                         {"success", getter.getSuccesses()},
                         {"failure", getter.getFailures()},
                         {"exception", getter.getExceptions()}};

        // Write per-call information.
        json["results"] = getter.getResults();
    }

    JsonExporter::JsonExporter(std::filesystem::path directory) : IExporter(std::move(directory)) {}

    JsonExporter::~JsonExporter() noexcept = default;

    void JsonExporter::writeSuite(const TestSuite& suite)
    {
        // Create directory if it does not exist.
        if (!exists(path)) create_directories(path);

        nlohmann::json data;

        // Write suite data.
        data["suite"] = suite;

        // Write unit test states.
        data["unitTests"] = suite.getUnitTestSuite().getData();

        // Write performance test states.
        // data["performanceTests"] = suite.getPerformanceTestSuite().getTestStates();

        // Write JSON data to file.
        std::ofstream file(path / "suite.json");
        if (!file) throw ExportError("Failed to open suite file");
        file << data;
        file.close();
    }

    void JsonExporter::writeUnitTestResults(const TestSuite& suite, const IUnitTest& test, const std::string& name)
    {
        // Create directory if it does not exist.
        const auto testPath = this->path / name;
        if (!exists(testPath)) create_directories(testPath);

        // Write test results.
        nlohmann::json data;
        const auto& mixins  = test.getMixins();
        const auto& getters = test.getResultsGetters();
        for (size_t i = 0; i < mixins.size(); i++)
        {
            const auto& getter = *getters[i];
            const auto& mixin  = mixins[i];
            data[mixin]        = getter;
        }

        // Generate filename as "unit_########.json".
        const std::string filename = std::format("unit_{:0>8}.json", suite.getData().runIndex);

        // Write JSON data to file.
        std::ofstream file(testPath / filename);
        if (!file) throw ExportError("Failed to open unit test file");
        file << data;
        file.close();
    }
}  // namespace bt