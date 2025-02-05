#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

std::atomic<bool> running(true);

void
signalHandler(int signal)
{
  std::cout << "\nSignal received: " << signal
            << ". Initiating graceful shutdown...\n";
  running = false; // 終了フラグをセット
}

int
main()
{
  // シグナルハンドラを登録
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  // メイン処理ループ
  while (1) {
    std::cout << "Running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "Graceful shutdown completed.\n";
  return 0;
}
