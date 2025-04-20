# spdlogのログ仕様

## 3層構造について

![3層構造](./specific_img/specific-logger.drawio.svg)

## logger layer

ロガー層は、ロガー全体の設定を担当する。  
単一のformatter、複数のsinkの管理、ログレベルの設定などを行う。

## formatter layer

フォーマッタ層は、ロガーに入ってきたログメッセージを統一的に成型する責任を持つ。  
メッセージの順番、メッセージのフォーマットに応じて成型する。

### plane text

最も簡便な平文ログ

```log
2025-04-20 14:35:12 INFO User login successful: user_id=1234
```

### syslog([RFC 5424](https://datatracker.ietf.org/doc/html/rfc5424))  

```syslog
<PRIORITY>VERSION TIMESTAMP HOSTNAME APP_NAME PROCESS_ID MSG_ID [STRUCTURED_DATA] MSG
ex)
<165>1 2025-04-20T14:35:00Z bastion-server ssh 1234 ID47 [exampleSDID@32473 iut="3" eventSource="Application"] Event message here
```

RFC名|タイトル|分類|発行年
|:--|:--|:--|--:|
RFC3164|The BSD syslog Protocol|Informational|2001年
RFC3195|Reliable Delivery for syslog|Standards Track|2001年
__RFC5424__|__The Syslog Protocol__|__Standards Track__|__2009年__
RFC5425|TLS Transport Mapping for Syslog|Standards Track|2009年
RFC5426|Transmission of Syslog Messages over UDP|Standards Track|2009年
RFC5427|Textual Conventions for Syslog Management|Standards Track|2009年
RFC5676|Definitions of Managed Objects for Mapping SYSLOG Messages to SNMP Notifications|Standards Track|2009年
RFC5848|Signed Syslog Messages|Standards Track|2010年
RFC6012|DTLS Transport Mapping for Syslog|Standards Track|2010年
RFC6587|Transmission of Syslog Messages over TCP|Historic|2012年

### json

クラウドなどのWEBシステムで良く用いられる構造化ログ。

```json
{"date": "2025-04-20T14:35:12Z", "level": "INFO", "msg": "User login successful", "user_id": 1234}
{"date": "2025-04-20T14:35:12Z", "level": "INFO", "msg": "User login successful", "user_id": 1111}
```

採用例

- fluentd
- LogStash
- Datadog
- CloudWatch Logs

### common log format / combine log format

WEBサーバーで使われるアクセスログの形式

```log
host ident authuser [date] "request" status bytes
ex)
127.0.0.1 - frank [10/Oct/2025:13:55:36 +0900] "GET /index.html HTTP/1.1" 200 1024
```

採用例

- Apache
- nginx

## sink layer

シンク層は、フォーマッタで整形されたログを受け取って目的のデバイスへI/Oを行う責任を持つ。  
主な機能はI/O関連のためasyncな書き込みを行うことやファイルローテーションなどが行われることが多い。

しかし、フォーマッタで整形された後も更なる整形が可能であり、コンソールへは平文、ファイルへはjsonで出力するなども可能である。
アクセスログでは暗号化などをこの層で行うこともある。

### console

`stdout`へ出力するシンク。  
色付け等を行う`colored console sink`なども存在する。

### file

所定のログファイルへ書き込むシンク。  
`rotating_file_sink`や`hourly_rotating_file_sink`など、ローテーション機能を持ったシンクなども存在する。
