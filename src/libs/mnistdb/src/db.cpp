#include "db.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <boost/endian/conversion.hpp>


namespace db {


bool ImageContainer::parseBuffer()
{
    bool result = false;
    if (mFileBuffer.size() >= HEADER_SIZE )
    {
        auto srcPtr = mFileBuffer.data();
        int offset = 0;

        std::memcpy(&mMagic, srcPtr + offset, sizeof(mMagic));
        offset += sizeof(mMagic);
        std::memcpy(&mCount, srcPtr + offset, sizeof(mCount));
        offset += sizeof(mCount);
        std::memcpy(&mWidth, srcPtr + offset, sizeof(mWidth));
        offset += sizeof(mWidth);
        std::memcpy(&mHeight, srcPtr + offset, sizeof(mHeight));
        offset += sizeof(mHeight);

        mMagic = boost::endian::big_to_native(mMagic);
        mCount = boost::endian::big_to_native(mCount);
        mWidth = boost::endian::big_to_native(mWidth);
        mHeight = boost::endian::big_to_native(mHeight);

        result = mMagic == 0x00000803
            && mWidth > 0
            && mHeight > 0
            && (HEADER_SIZE + mWidth * mHeight * mCount) <= mFileBuffer.size();
    }
    return result;
}

buffer::const_iterator ImageContainer::imageBegin(size_t index) const
{
    BOOST_ASSERT(index < mCount);
    size_t offset = HEADER_SIZE + index * mWidth * mHeight;
    BOOST_ASSERT(offset < mFileBuffer.size());
    return mFileBuffer.cbegin() + offset;
}

buffer::const_iterator ImageContainer::imageEnd(size_t index) const
{
    BOOST_ASSERT(index < mCount);
    size_t offset = HEADER_SIZE + (index + 1) * mWidth * mHeight;
    BOOST_ASSERT(offset <= mFileBuffer.size());
    return mFileBuffer.cbegin() + offset;
}


bool ImageIndexContainer::parseBuffer()
{
    bool result = false;
    if (mFileBuffer.size() >= HEADER_SIZE)
    {
        auto srcPtr = mFileBuffer.data();
        int offset = 0;

        std::memcpy(&mMagic, srcPtr + offset, sizeof(mMagic));
        offset += sizeof(mMagic);
        std::memcpy(&mCount, srcPtr + offset, sizeof(mCount));
        offset += sizeof(mCount);

        mMagic = boost::endian::big_to_native(mMagic);
        mCount = boost::endian::big_to_native(mCount);

        result = mMagic == 0x00000801
            && (HEADER_SIZE + mCount) <= mFileBuffer.size();
    }
    return result;
}

uint8_t ImageIndexContainer::getImageClass(size_t index) const
{
    BOOST_ASSERT(index < mCount);
    return mFileBuffer[HEADER_SIZE + index];
}


bool loadFileIntoBuffer(const boost::filesystem::path &path, buffer &dataBuffer)
{
    bool result = boost::filesystem::exists(path)
        && boost::filesystem::is_regular_file(path);

    if (result)
    {
        boost::filesystem::ifstream input;
        const size_t fileSize = boost::filesystem::file_size(path);

        input.open(path, std::ios_base::in | std::ios_base::binary);
        result &= input.good();
        if (input.good())
        {
            dataBuffer.resize(fileSize);
            input.read(reinterpret_cast<char *>(dataBuffer.data()), fileSize);
        }
    }
    return result;
}


bool DB::loadDB(const std::string& imageFilePath, const std::string& imageIndexPath)
{
    auto imagePtr = std::make_unique<db::ImageContainer>();
    const auto imageFileOk = loadFileIntoBuffer(imageFilePath, imagePtr->mFileBuffer) && imagePtr->parseBuffer();

    auto imageLabelPtr = std::make_unique<db::ImageIndexContainer>();
    const auto imageLabelOk = loadFileIntoBuffer(imageIndexPath, imageLabelPtr->mFileBuffer) && imageLabelPtr->parseBuffer();

    if (imageFileOk && imageLabelOk && imagePtr->mCount == imageLabelPtr->mCount)
    {
        mImages = std::move(imagePtr);
        mLabels = std::move(imageLabelPtr);
    }
    return imageFileOk && imageLabelOk;
}

bool DB::isDbLoaded() const
{
    return mImages && mLabels;
}

size_t DB::getImageCount() const
{
    BOOST_ASSERT(mImages && mLabels);
    return mImages->mCount;
}

size_t DB::getPixelCount() const
{
    BOOST_ASSERT(mImages);
    return mImages->mHeight * mImages->mWidth;
}

math::MatrixF DB::getImageMatrix(size_t index) const
{
    auto result = math::make_matrix<float>(mImages->mWidth * mImages->mHeight, 1);
    std::transform(mImages->imageBegin(index), mImages->imageEnd(index), result.begin(), [](uint8_t value) -> float {
        return static_cast<float>(value) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    });
    return result;
}

math::MatrixF DB::getImageLabel(size_t index) const
{
    BOOST_ASSERT(mLabels);
    auto result = math::make_matrix<float>(10, 1);
    result.set(mLabels->getImageClass(index), 0, 1);
    return result;
}

size_t DB::getImageLabelAsInt(size_t index) const
{
    BOOST_ASSERT(mLabels);
    return mLabels->getImageClass(index);
}

size_t DB::getImageWidth() const
{
    return mImages->mWidth;
}

size_t DB::getImageHeight() const
{
    return mImages->mHeight;
}

db::DBInterfacePtr createDB()
{
    return std::shared_ptr<db::DBInterface>(new db::DB());
}

}