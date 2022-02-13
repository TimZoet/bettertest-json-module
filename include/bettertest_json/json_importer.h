#pragma once

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "bettertest/output/importer_interface.h"

namespace bt
{
    class JsonImporter final : public IImporter
    {
    public:
        static constexpr char type[] = "json";

        JsonImporter() = delete;

        JsonImporter(const JsonImporter&) = delete;

        JsonImporter(JsonImporter&&) = delete;

        explicit JsonImporter(std::filesystem::path directory);

        ~JsonImporter() noexcept override = default;

        JsonImporter& operator=(const JsonImporter&) = delete;

        JsonImporter& operator=(JsonImporter&&) = delete;

        bool readSuiteFile(TestSuite& suite) override;
    };
}
