#pragma once

#include <matrix/matrix.h>
#include <boost/filesystem.hpp>

namespace db {

class DBInterface
{
public:
    DBInterface() = default;
    virtual bool loadDB(const boost::filesystem::path& imageFilePath, const boost::filesystem::path &imageIndexPath ) = 0;
    virtual bool isDbLoaded() const = 0;
    virtual size_t getImageCount() const = 0;
    virtual math::MatrixF getImageMatrix(size_t index) const = 0;
    virtual math::MatrixF getImageLabel(size_t index) const = 0;
    virtual size_t getPixelCount() const = 0;
    virtual ~DBInterface() = default;
};

using DBInterfacePtr = std::unique_ptr<db::DBInterface>;

db::DBInterfacePtr createDB();

}
