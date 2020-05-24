#include "db_test.h"
#include "db/idb.h"
#include <boost/filesystem.hpp>


namespace db_test {

FileImageHeader getImageHeader(uint32_t numImages, uint32_t rows, uint32_t columns)
{
    FileImageHeader h;
    h.mMagic = 0x00000803;
    h.mImageCount = numImages;
    h.mRows = rows;
    h.mColumns = columns;
    return h;
}

FileLabelHeader getLabelHeader(uint32_t numLabels)
{
    FileLabelHeader h;
    h.mMagic = 0x00000801;
    h.mItemCount = numLabels;
    return h;
}

boost::filesystem::path generateTempFilePath()
{
    boost::filesystem::path result;
    do {
        result = boost::filesystem::temp_directory_path().append(boost::filesystem::unique_path().c_str());
    } while (boost::filesystem::exists(result));
    return result;
}


TEST(DBTests, InitCheck)
{
    auto db = db::createDB();
    ASSERT_TRUE(db);
    ASSERT_FALSE(db->isDbLoaded());
}

TEST(DBTests, ReadFromFile)
{
    const auto imagePath = generateTempFilePath();
    const auto labelPath = generateTempFilePath();

    {
        auto imageHeader = getImageHeader(0, 10, 10);
        boost::filesystem::ofstream fImage(imagePath, std::ios_base::out | std::ios_base::binary);
        fImage.write(reinterpret_cast<char *>(&imageHeader), sizeof(imageHeader));
        fImage.close();
    }
    {
        auto labelHeader = getLabelHeader(0);

        boost::filesystem::ofstream fLabel(labelPath, std::ios_base::out | std::ios_base::binary);
        fLabel.write(reinterpret_cast<char *>(&labelHeader), sizeof(labelHeader));
        fLabel.close();
    }

    auto db = db::createDB();
    auto result = db->loadDB(imagePath.generic_string(), labelPath.generic_string());

    EXPECT_TRUE(result);
    EXPECT_TRUE(db->isDbLoaded());
    EXPECT_EQ(db->getImageCount(), 0);

    boost::filesystem::remove(imagePath);
    boost::filesystem::remove(labelPath);
}


TEST(DBTests, NoPathFound)
{
    boost::filesystem::path imagePath = "non_existing_file.dat";
    boost::filesystem::path labelPath = "non_existing_label.dat";

    auto db = db::createDB();
    auto result = db->loadDB(imagePath.generic_string(), labelPath.generic_string());

    ASSERT_FALSE(result);
    ASSERT_FALSE(db->isDbLoaded());
}


}