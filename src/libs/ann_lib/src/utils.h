#pragma once

#include <matrix/matrix.h>


namespace ann{

math::MatrixF getImageDataBasedOnRange(const db::DBInterface &db, const ShuffledRange::IndexedRange &range);
math::MatrixF getImageClassBasedOnRange(const db::DBInterface &db, const ShuffledRange::IndexedRange &range);

}