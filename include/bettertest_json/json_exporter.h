#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/output/exporter_interface.h"

namespace bt
{
    class JsonExporter final : public IExporter
    {
    public:
        static constexpr char type[] = "json";

        JsonExporter() = delete;

        JsonExporter(const JsonExporter&) = delete;

        JsonExporter(JsonExporter&&) = delete;

        explicit JsonExporter(std::filesystem::path directory);

        ~JsonExporter() noexcept override;

        JsonExporter& operator=(const JsonExporter&) = delete;

        JsonExporter& operator=(JsonExporter&&) = delete;

        void writeSuite(const TestSuite& suite) override;

        void writeUnitTestResults(const TestSuite& suite, const IUnitTest& test, const std::string& name) override;
    };
}  // namespace bt
