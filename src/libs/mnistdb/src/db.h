#pragma once

#include <db/idb.h>

namespace db {

using buffer = std::vector<uint8_t>;

struct ImageContainer
{
    static constexpr size_t HEADER_SIZE = 4 * sizeof(uint32_t);

    buffer mFileBuffer;
    uint32_t mMagic;
    uint32_t mCount;
    uint32_t mWidth;
    uint32_t mHeight;

    bool parseBuffer();

    buffer::const_iterator imageBegin(size_t index) const;
    buffer::const_iterator imageEnd(size_t index) const;
};

struct ImageIndexContainer
{
    static constexpr size_t HEADER_SIZE = 2 * sizeof(uint32_t);

    buffer mFileBuffer;
    uint32_t mMagic;
    uint32_t mCount;

    bool parseBuffer();

    uint8_t getImageClass(size_t index) const;
};


class DB
    : public db::DBInterface
{
    std::unique_ptr<ImageContainer> mImages;
    std::unique_ptr<ImageIndexContainer> mLabels;

public:
    DB() = default;
    DB(const DB &other) = delete;
    DB(DB && other) = delete;

    bool loadDB(const boost::filesystem::path& imageFilePath, const boost::filesystem::path& lableFilePath) override;
    bool isDbLoaded() const override;
    size_t getImageCount() const override;
    math::MatrixF getImageMatrix(size_t intdex) const override;
    uint8_t getImageLabel(size_t) const override;

};

}
