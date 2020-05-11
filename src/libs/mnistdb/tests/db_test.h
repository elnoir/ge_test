#include <gtest/gtest.h>
#include <boost/endian/buffers.hpp>

namespace db_test {

struct FileImageHeader
{
    boost::endian::big_int32_buf_t mMagic;
    boost::endian::big_int32_buf_t mImageCount;
    boost::endian::big_int32_buf_t mRows;
    boost::endian::big_int32_buf_t mColumns;
};

struct FileLabelHeader
{
    boost::endian::big_int32_buf_t mMagic;
    boost::endian::big_int32_buf_t mItemCount;
};

}