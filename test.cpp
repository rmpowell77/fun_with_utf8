#include <stdio.h>
#include <iostream>
#include <vector>

constexpr bool is_utf8(float f)
{
  const char* ptr = (const char*)&f;
  return ((ptr[0]&0xF0)==0xF0) &&
	((ptr[1]&0x80)==0x80) &&
	((ptr[2]&0x80)==0x80) &&
	((ptr[3]&0x80)==0x80);
}

constexpr float convert_utf8_float(int f)
{
  const char num[4] = { char(0xF0 | ((f>>18)&0x7)), char(0x80 | ((f>>12)&0x3F)), char(0x80 | ((f>>6)&0x3F)), char(0x80 | ((f)&0x3F)) };
  return *(float*)&num;
}

constexpr auto get_bottom_bits(uint8_t value, int num)
{
  return value & ((1<<num) - 1);
}

template<typename T>
constexpr auto shift(T in, int shift)
{
  return in << shift;
}
 
constexpr uint32_t get_offset(char const* ptr)
{
  return shift(get_bottom_bits(ptr[0], 3), 18) |
	shift(get_bottom_bits(ptr[1], 6), 12) |
	shift(get_bottom_bits(ptr[2], 6), 6) |
	shift(get_bottom_bits(ptr[3], 6), 0);
}

constexpr int convert_float_to_code(float f)
{
  const char* ptr = (const char*)&f;
  return get_offset(ptr);
}

std::string get_emoji(int i)
{
  const char emoji_code[5] = { char(0xF0 | ((i>>18)&0x7)), char(0x80 | ((i>>12)&0x3F)), char(0x80 | ((i>>6)&0x3F)), char(0x80 | ((i)&0x3F)), 0 };
  return emoji_code;
}

void print_out(int emoji)
{
  printf("%s : %x : %e", get_emoji(emoji).c_str(), emoji, convert_utf8_float(emoji));
}

const std::vector<std::pair<int, int>> favorite_emoji = {
  { 0x1F000, 0x1F02C },
  { 0x1F030, 0x1F094 },
  { 0x1F0A0, 0x1f0e0 },
  { 0x1F300, 0x1F650 },
  { 0x1F680, 0x1F700 },
  { 0x1F910, 0x1F96C },
  { 0x1F980, 0x1F998 },
  { 0x1F9C0, 0x1F9C1 },
  { 0x1F9D0, 0x1F9E7 },
};

bool what_were_looking_for(float f)
{
  if (!is_utf8(f)) return false;
  auto code = convert_float_to_code(f);
  for (auto const& r : favorite_emoji) {
    if ((code >= r.first) && (code < r.second))
      return true;
  }
  return false;
}


int main()
{
  const auto columns = 3;
  int a = columns;
  for (auto const& r : favorite_emoji) {
    for (auto c = r.first; c < r.second; ++c) {
      print_out(c);
      if (a-- > 0) {
        printf("\t");
      }
      else {
        printf("\n");
        a = columns;
      }
    }
  }
  for (auto const& r : favorite_emoji) {
    for (auto c = r.first; c < r.second; ++c) {
      auto a = convert_utf8_float(c);

      if (what_were_looking_for(a * a)) {
        printf("found a match with %d * %d\n", c, c);
        print_out(a*a);
      }

      if (what_were_looking_for(a + a)) {
        printf("found a match with %d + %d\n", c, c);
        print_out(a+a);
      }

      for (auto const& r2 : favorite_emoji) {
        for (auto c2 = r2.first; c2 < r2.second; ++c2) {
          auto b = convert_utf8_float(c2);
          if (what_were_looking_for(a * b)) {
            printf("found a match with %d * %d\n", c, c2);
            print_out(a*b);
          }
          if (what_were_looking_for(a - b)) {
            printf("found a match with %d - %d\n", c, c2);
            print_out(a-b);
          }
          if (what_were_looking_for(a / b)) {
            printf("found a match with %d / %d\n", c, c2);
            print_out(a/b);
          }
          if (what_were_looking_for(b - a)) {
            printf("found a match with %d - %d\n", c2, c);
            print_out(b-a);
          }
          if (what_were_looking_for(b / a)) {
            printf("found a match with %d / %d\n", c2, c);
            print_out(b/a);
          }
        }
      }
    }
  }
  printf("test a %d\n", what_were_looking_for(convert_utf8_float(0x1F680)));
  printf("test a %d\n", what_were_looking_for(convert_utf8_float(0x1F6F0)));



  return 0;
}

