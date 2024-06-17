#include "addrinfo.h"
#include <stdlib.h>
#include <string.h>

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
