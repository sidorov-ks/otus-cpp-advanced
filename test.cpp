#include <gtest/gtest.h>

#include "ip.h"

TEST(test_version, test_valid_version
)
{
EXPECT_GT(version(),

0);
}