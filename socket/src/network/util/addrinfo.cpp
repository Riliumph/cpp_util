#include "addrinfo.h"
// STL
#include <algorithm>
// C
#include <stdlib.h>
#include <string.h>
// Linux
#include <arpa/inet.h>

void
FreeAddrinfo(struct addrinfo* info)
{
  freeaddrinfo(info); // リンクリストのみ解放する
  if (info->ai_addr != nullptr) {
    free(info->ai_addr);
  }
  if (info->ai_canonname != nullptr) {
    free(info->ai_canonname);
  }
  free(info);
}

/// @brief addrinfo型のdeep copy
/// @param src コピー元のアドレス
/// @param dst コピー先のアドレス
/// @return 成否
int
DeepCopy(const struct addrinfo* src, struct addrinfo** dst)
{
  if (src == nullptr) {
    return -1;
  }

  *dst = (struct addrinfo*)malloc(sizeof(struct addrinfo));
  if (*dst == nullptr) {
    return -1;
  }

  // 構造体全体をshallow copy（ポインタ変数はアドレス値が複製）
  memcpy(*dst, src, sizeof(struct addrinfo));
  // ポインタ変数部分をdeep copy
  (*dst)->ai_next = nullptr;

  // ai_addrのコピー処理
  (*dst)->ai_addr = nullptr;
  (*dst)->ai_addr = (struct sockaddr*)malloc(src->ai_addrlen);
  if ((*dst)->ai_addr == nullptr) {
    free(*dst);
    *dst = NULL;
    return -1;
  }
  memcpy((*dst)->ai_addr, src->ai_addr, src->ai_addrlen);

  // ai_canonnameのコピー処理
  (*dst)->ai_canonname = nullptr;
  if (src->ai_canonname != nullptr) {
    (*dst)->ai_canonname = strdup(src->ai_canonname);
    if ((*dst)->ai_canonname == nullptr) {
      free((*dst)->ai_addr);
      free(*dst);
      *dst = nullptr;
      return -1;
    }
  }
  return 0;
}

/// @brief 構造体addrinfoからIPアドレスを取得する関数
/// 取得できるバイナリ形式からpresentation形式（文字列）への変換も行う。
/// @param inet アドレス構造体
/// @return IPアドレスの文字列
std::optional<std::string>
ConvertIPv4(struct addrinfo* inet)
{
  std::string buf;
  buf.resize(INET_ADDRSTRLEN);
  auto* ipv4 = reinterpret_cast<struct sockaddr_in*>(inet->ai_addr);
  const auto* p =
    inet_ntop(inet->ai_family, &ipv4->sin_addr, buf.data(), buf.size());
  if (p == nullptr) {
    perror("get ip");
    return std::nullopt;
  }
  // NULL文字が含まれていることがあるため削除
  buf.erase(std::remove(buf.begin(), buf.end(), '\0'), buf.end());
  buf.shrink_to_fit();
  return buf;
}

/// @brief 構造体addrinfoからポート番号を取得する関数
/// ネットワークバイトオーダーからホストバイトオーダーへの変換も行う。
/// @param inet アドレス構造体
/// @return ポート番号
int
ConvertPort(struct addrinfo* inet)
{
  int port = 0;
  for (auto* info = inet; info != nullptr; info = info->ai_next) {
    auto* ipv4 = (struct sockaddr_in*)info->ai_addr;
    port = static_cast<int>(ntohs(ipv4->sin_port));
  }
  return port;
}
