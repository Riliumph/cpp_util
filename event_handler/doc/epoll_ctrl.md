# EPOLL_CTRL

EPOLLのインスタンスにイベントを追加・更新・削除を行うAPI。

## EPOLL_CTL_DEL

特定のFDに対して、すべてのイベントを削除するAPI。  
特定のFDの特定のイベントを削除するということはできない。

例えば、FD5番に`EPOLLIN|EPOLLRDHUP`の二つのイベントを登録していたとする。  
このFDから`EPOLLRDHUP`を削除したい。  
この場合は、`EPOLL_CTL_DEL`ではできない。  
`EPOLL_CTL_MOD`を使って`EPOLLIN`のみのイベントで変更することになる。
