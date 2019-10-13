#include <iostream>
using std::cout;
using std::endl;
#include <string.h>
#include "DataMessage.h"
#include <gtest/gtest.h>

//Empty message test
DataMessage message = DataMessage();
const int capacity = 128;
char str[capacity];

TEST(messageTest, emptyMessage){
  message.ToString(str,capacity);

  ASSERT_EQ(NULL, str);
}

TEST(messageTest, intKeyValuePair){
  memset(str, 0, sizeof(str));
  message.Add(0, 0);
  message.Add(1235, 9393);
  message.Add(10, -7000);
  message.ToString(str,capacity);

  for (int i = 0; i < 128; i++){
    cout << str[i];
  }
  cout << endl;

  ASSERT_EQ(0, str[0]);
  ASSERT_EQ(0, str[1]);
  ASSERT_EQ(1235, str[2]);
  ASSERT_EQ(9393, str[3]);
  ASSERT_EQ(10, str[4]);
  ASSERT_EQ(-7000, str[5]);
}

TEST(messageTest, intKeyValueAndFloatPair){
  memset(str, 0, sizeof(str));
  message.Add(1, 12497.2f);
  message.Add(1923, 94.2f);
  message.Add(123, 0.8f);
  message.Add(85, -1.0f);
  message.ToString(str,capacity);
}

TEST(messageTest, bufferOverflow){
  memset(str, 0, sizeof(str));
  message.ToString(str,capacity);
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
