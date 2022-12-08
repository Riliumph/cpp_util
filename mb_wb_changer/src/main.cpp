#include <iostream>
#include <memory>
#include <string>

std::string
ToNarrow(const std::wstring& wide_byte);
std::wstring
ToWiden(const std::string& multi_byte);

int
main(void)
{
  std::string str = "abc";
  // 何故かcoutしておかないとNARROWが出力されない……
  std::cout << "ORIGINAL: " << str << std::endl;
  auto wb_str = ToWiden(str);
  std::wcout << "WIDE: " << wb_str << std::endl;
  auto mb_str = ToNarrow(wb_str);
  std::cout << "NARROW: " << mb_str << std::endl;
  return 0;
}

/**
 * @brief Change multi byte string to wide byte string.
 *
 * @note Use std::locale() before execute this method.
 * @param multi_byte[in] converted string
 * @return wide byte string instance
 */
std::wstring
ToWiden(const std::string& multi_byte)
{
  constexpr int NULL_LENGTH = 1;
  const int WB_LENGTH = multi_byte.length() + NULL_LENGTH;
  std::wstring wide_byte;
  wide_byte.resize(WB_LENGTH);
  mbstowcs(wide_byte.data(), multi_byte.c_str(), wide_byte.size());
  return wide_byte;
}

/**
 * @brief Change wide byte string to multi byte string.
 *
 * @note Use std::locale() before execute this method.
 * @param wide_byte[in] converted string
 * @return multi byte string instance
 */
std::string
ToNarrow(const std::wstring& wide_byte)
{
  constexpr int NULL_LENGTH = 1;
  const int MB_LENGTH = wide_byte.length() * MB_CUR_MAX + NULL_LENGTH;
  std::string multi_byte;
  multi_byte.resize(MB_LENGTH);
  wcstombs(multi_byte.data(), wide_byte.c_str(), multi_byte.size());
  return multi_byte;
}
