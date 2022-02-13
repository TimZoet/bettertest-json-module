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

        void writeSuiteFile(const TestSuite& suite) override;

        void writeUnitTestFile(const TestSuite& suite, const IUnitTest& test, const std::string& name) override;

        [[nodiscard]] bool supportsMultithreading() const noexcept override;
    };
}  // namespace bt
