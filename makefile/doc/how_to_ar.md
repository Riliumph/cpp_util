# arコマンドで静的ライブラリを確かめる

参考

- [1 ar](https://sourceware.org/binutils/docs/binutils/ar.html)
- [Ubuntu Manpage](https://manpages.ubuntu.com/manpages/focal/ja/man1/ar.1.html)
- [ar - 静的なライブラリファイルを作成する](https://linuxcommand.net/ar/#aropcode)

```console
$ man ar
AR(1)q     GNU Development Tools

NAME
       ar - create, modify, and extract from archives

SYNOPSIS
       ar [-X32_64] [-]p[mod] [--plugin name] [--target bfdname] [--output dirname] [--record-libdeps libdeps] [--thin] [relpos] [count] archive [member...]
```

## opcode

### `t` 内包されるオブジェクトの確認

```console
$ ar tv lib/libchrono.a
rw-r--r-- 0/0  47136 Jan  1 00:00 1970 operator_io.o
rw-r--r-- 0/0  65984 Jan  1 00:00 1970 sleep.o
```

### `s` シンボルテーブルの生成

デフォルトでシンボルテーブルは生成される

> `S`オプションでテーブルレスなライブラリが作れるが、リンク先でシンボルが見つけられない可能性が高い

```console
$ ar
```

#### 特殊編：シンボルテーブルの表示

```console
$ nm -s archive.a 
Archive index:
_ZNKSt6chrono8durationIlSt5ratioILl1ELl1000000000EEE5countEv in operator_io.o
_ZNSt6chrono3_V212system_clock9to_time_tERKNS_10time_pointIS1_NS_8durationIlSt5ratioILl1ELl1000000000EEEEEE in operator_io.o
DW.ref.__gxx_personality_v0 in operator_io.o
_ZNKSt6chrono10time_pointINS_3_V212system_clockENS_8durationIlSt5ratioILl1ELl1000000000EEEEE16time_since_epochEv in operator_io.o
_ZNSt6chrono13duration_castINS_8durationIlSt5ratioILl1ELl1EEEElS2_ILl1ELl1000000000EEEENSt9enable_ifIXsrNS_13__is_durationIT_EE5valueES8_E4typeERKNS1_IT0_T1_EE in operator_io.o
_ZNKSt6chrono8durationIlSt5ratioILl1ELl1EEE5countEv in operator_io.o
_ZNSt6chrono8durationIlSt5ratioILl1ELl1EEEC1IlvEERKT_ in operator_io.o
_ZlsRSoRKNSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE in operator_io.o
_ZNSt6chrono20__duration_cast_implINS_8durationIlSt5ratioILl1ELl1EEEES2_ILl1ELl1000000000EElLb1ELb0EE6__castIlS5_EES4_RKNS1_IT_T0_EE in operator_io.o
_ZNSt6chrono8durationIlSt5ratioILl1ELl1000000000EEEC1IlvEERKT_ in sleep.o
_ZNSt6chrono15duration_valuesIlE4zeroEv in sleep.o
_ZNKSt6chrono8durationIlSt5ratioILl1ELl1000000000EEE5countEv in sleep.o
_ZNSt6chrono8durationIlSt5ratioILl1ELl1EEEC1IlvEERKT_ in sleep.o
_ZNSt6chrono8durationIlSt5ratioILl1ELl1EEEC1IivEERKT_ in sleep.o
_Z10cron_sleepd in sleep.o
_ZNSt11this_thread9sleep_forIlSt5ratioILl1ELl1EEEEvRKNSt6chrono8durationIT_T0_EE in sleep.o
_ZNKSt6chrono8durationIlSt5ratioILl1ELl1EEE5countEv in sleep.o
_ZNSt6chrono8durationIlSt5ratioILl1ELl1EEE4zeroEv in sleep.o
_ZNSt6chronoleIlSt5ratioILl1ELl1EElS2_EEbRKNS_8durationIT_T0_EERKNS3_IT1_T2_EE in sleep.o
_ZNSt6chrono13duration_castINS_8durationIlSt5ratioILl1ELl1EEEElS3_EENSt9enable_ifIXsrNS_13__is_durationIT_EE5valueES7_E4typeERKNS1_IT0_T1_EE in sleep.o
_ZNSt6chronomiIlSt5ratioILl1ELl1EElS2_EENSt11common_typeIJNS_8durationIT_T0_EENS4_IT1_T2_EEEE4typeERKS7_RKSA_ in sleep.o
_ZNSt6chrono13duration_castINS_8durationIlSt5ratioILl1ELl1000000000EEEElS2_ILl1ELl1EEEENSt9enable_ifIXsrNS_13__is_durationIT_EE5valueES8_E4typeERKNS1_IT0_T1_EE in sleep.o
_ZNSt6chrono20__duration_cast_implINS_8durationIlSt5ratioILl1ELl1000000000EEEES2_ILl1000000000ELl1EElLb0ELb1EE6__castIlS2_ILl1ELl1EEEES4_RKNS1_IT_T0_EE in sleep.o
_ZNSt6chronoltIlSt5ratioILl1ELl1EElS2_EEbRKNS_8durationIT_T0_EERKNS3_IT1_T2_EE in sleep.o
_ZNSt6chrono20__duration_cast_implINS_8durationIlSt5ratioILl1ELl1EEEES3_lLb1ELb1EE6__castIlS3_EES4_RKNS1_IT_T0_EE in sleep.o

operator_io.o:
（省略）
```
