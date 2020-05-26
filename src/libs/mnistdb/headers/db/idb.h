#pragma once

#include <string>
#include <matrix/matrix.h>


namespace db {

class DBInterface
{
public:
    DBInterface() = default;
    virtual bool loadDB(const std::string &imageFilePath, const std::string &imageIndexPath ) = 0;
    virtual bool isDbLoaded() const = 0;
    virtual size_t getImageCount() const = 0;
    virtual math::MatrixF getImageMatrix(size_t index) const = 0;
    virtual math::MatrixF getImageLabel(size_t index) const = 0;
    virtual size_t getImageLabelAsInt(size_t index) const = 0;
    virtual size_t getPixelCount() const = 0;
    virtual ~DBInterface() = default;
};

using DBInterfacePtr = std::shared_ptr<db::DBInterface>;

db::DBInterfacePtr createDB();

}
