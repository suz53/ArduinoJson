// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdio.h>
#include <catch.hpp>

static void check(const JsonObject& object, const char* expected_data,
                  size_t expected_len) {
  std::vector<char> expected(expected_data, expected_data + expected_len);
  std::vector<char> actual;
  size_t len = serializeMsgPack(object, actual);
  CAPTURE(object);
  REQUIRE(len == expected_len);
  REQUIRE(actual == expected);
}

template <size_t N>
static void check(const JsonObject& object, const char (&expected_data)[N]) {
  const size_t expected_len = N - 1;
  check(object, expected_data, expected_len);
}

/*static void check(const JsonObject& object, const std::string& expected) {
  check(object, expected.data(), expected.length());
}*/

TEST_CASE("serialize MsgPack object") {
  DynamicJsonDocument doc;
  JsonObject& object = doc.to<JsonObject>();

  SECTION("empty") {
    check(object, "\x80");
  }

  SECTION("fixmap") {
    object["hello"] = "world";

    check(object, "\x81\xA5hello\xA5world");
  }

  SECTION("map 16") {
    for (int i = 0; i < 16; ++i) {
      char key[16];
      sprintf(key, "i%X", i);
      object[key] = i;
    }

    check(object,
          "\xDE\x00\x10\xA2i0\x00\xA2i1\x01\xA2i2\x02\xA2i3\x03\xA2i4\x04\xA2i5"
          "\x05\xA2i6\x06\xA2i7\x07\xA2i8\x08\xA2i9\x09\xA2iA\x0A\xA2iB\x0B\xA2"
          "iC\x0C\xA2iD\x0D\xA2iE\x0E\xA2iF\x0F");
  }
}
