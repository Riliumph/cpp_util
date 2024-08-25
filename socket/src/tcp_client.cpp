#include <algorithm>   // std::reverse
#include <arpa/inet.h> // ソケット関連
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <netinet/in.h> // ソケットアドレス構造体
#include <sys/socket.h> // ソケット関連
#include <type_traits>  // std::enable_if, std::is_scalar
#include <unistd.h>     // close関数
// 3rd
#include "endian.hpp"

int
main()
{
  int sock = 0;
  struct sockaddr_in serv_addr;

  int32_t data1 = -10; // 4バイト
  int32_t data2 = 125; // 4バイト
  int16_t data3 = 1;   // 2バイト
  int16_t data4 = -8;  // 2バイト

  // データをバッファにコピー
  std::array<uint8_t, 12> buffer = { 0 };
  int start_point = 0;
  std::memcpy(buffer.data() + start_point, &data1, sizeof(data1));
  start_point += sizeof(data1);
  std::memcpy(buffer.data() + start_point, &data2, sizeof(data2));
  start_point += sizeof(data2);
  std::memcpy(buffer.data() + start_point, &data3, sizeof(data3));
  start_point += sizeof(data3);
  std::memcpy(buffer.data() + start_point, &data4, sizeof(data4));

  // 送信データをビッグエンディアンに変換
  auto data1_be = endian::Reverse(data1);
  auto data2_be = endian::Reverse(data2);
  auto data3_be = endian::Reverse(data3);
  auto data4_be = endian::Reverse(data4);

  // データをバッファにコピー（ビッグエンディアン）
  start_point = 0;
  std::memcpy(buffer.data() + start_point, &data1_be, sizeof(data1_be));
  start_point += sizeof(data1_be);
  std::memcpy(buffer.data() + start_point, &data2_be, sizeof(data2_be));
  start_point += sizeof(data2_be);
  std::memcpy(buffer.data() + start_point, &data3_be, sizeof(data3_be));
  start_point += sizeof(data3_be);
  std::memcpy(buffer.data() + start_point, &data4_be, sizeof(data4_be));

  // ソケットの作成
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(50000);

  // localhostに接続
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address or Address not supported" << std::endl;
    return -1;
  }

  // サーバーに接続
  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Connection Failed" << std::endl;
    return -1;
  }

  // データの送信
  if (send(sock, buffer.data(), buffer.size(), 0) != buffer.size()) {
    std::cerr << "Send failed" << std::endl;
  } else {
    std::cout << "Data sent successfully" << std::endl;
  }

  // ソケットを閉じる
  close(sock);

  return 0;
}
