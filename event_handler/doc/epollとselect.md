# epollとselectの違い

## イベント（トリガー）の比較

|select|epoll|意味|
|:--:|:--:|:--|:--|
|readfds|EPOLLIN|読み込みできる|
|writefds|EPOLLOUT|書き込みできる|
|exceptfds|EPOLLERR|エラーがある|
|-|EPOLLHUP|切断|

## イベントを処理しなかった場合

```cpp
  auto updated_fd_num = WaitEvent();
  if (updated_fd_num == -1) {
    perror("select_wait");
    return;
  }
  if (updated_fd_num == 0) {
    std::cout << "timeout" << std::endl;
    return;
  }

  std::set<fd_t> merged_fds;
  merged_fds.insert(read_fds_.begin(), read_fds_.end());
  merged_fds.insert(write_fds_.begin(), write_fds_.end());
  merged_fds.insert(except_fds_.begin(), except_fds_.end());
  std::cout << "fired fds: " << merged_fds << std::endl;
  for (const auto& fd : merged_fds) {
    if (FD_ISSET(fd, &read_mask_)) {
      std::cout << "read event: " << fd << std::endl;
      // reaction_[fd](fd); 処理しない
    }
    if (FD_ISSET(fd, &write_mask_)) {
      std::cout << "write event" << std::endl;
    }
    if (FD_ISSET(fd, &except_mask_)) {
      std::cout << "except event" << std::endl;
    }
  }
  std::cout << std::endl;
```

処理しない場合、そのFDはイベントを検知したままになる。  
`select`の戻っても、前の検知に反応してここに入ってくr。

```console
create trigger for 0...
set write trigger
server_fd: 3
create trigger for 3...
set write trigger
waiting event by select ...
read: 0 3 
write: 
except: 
fired fds: 0 3
read event: 3

waiting event by select ...
read: 0 3
write:
except:
fired fds: 0 3
read event: 3

waiting event by select ...
read: 0 3
write:
except:
fired fds: 0 3
read event: 3
```
