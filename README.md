# dns

Library to handle DNS messages

## Workflows

It is possible to build and test using `cmake` presets defined in [CMakePresets.json](CMakePresets.json), for example:

```
cmake --workflow --preset make-clang-debug
```

Use `cmake --workflow --list-presets`to se all presets available

### Code coverage

```bash
cmake --workflow --preset make-gcc-debug-coverage
```

## Examples

`dns_resolver_testapp` is an example application that resolves `A` records for a domain name passed as argument and describes the messages (request and response) unpacking the names from the records

```sh
$ ./build/make-clang-debug/bin dns_resolver_testapp www.example.com
```

```
query (33):
00 01 01 00 00 01 00 00
00 00 00 00 03 77 77 77
07 65 78 61 6d 70 6c 65
03 63 6f 6d 00 00 01 00
01
bytes_sent: 33
bytes_recv: 49
answer:
00 01 81 80 00 01 00 01
00 00 00 00 03 77 77 77
07 65 78 61 6d 70 6c 65
03 63 6f 6d 00 00 01 00
01 c0 0c 00 01 00 01 00
00 00 5f 00 04 5d b8 d8
22
id:            1
flags:         8180
question:
name:          www.example.com
type:          1
class:         1
answers:       1
name:
c0 0c
name unpacked: www.example.com
type:          1
class:         1
data:
5d b8 d8 22
authority:     0
additional:    0
```

DNS example with CNAME records

```sh
 ./build/make-clang-debug/bin/dns_resolver_testapp www.bing.com
```

```
query (30):
00 01 01 00 00 01 00 00
00 00 00 00 03 77 77 77
04 62 69 6e 67 03 63 6f
6d 00 00 01 00 01
bytes_sent: 30
bytes_recv: 279
answer:
00 01 81 80 00 01 00 0b
00 00 00 00 03 77 77 77
04 62 69 6e 67 03 63 6f
6d 00 00 01 00 01 c0 0c
00 05 00 01 00 00 42 3e
00 25 07 77 77 77 2d 77
77 77 04 62 69 6e 67 03
63 6f 6d 0e 74 72 61 66
66 69 63 6d 61 6e 61 67
65 72 03 6e 65 74 00 c0
2a 00 05 00 01 00 00 00
25 00 17 03 77 77 77 04
62 69 6e 67 03 63 6f 6d
07 65 64 67 65 6b 65 79
c0 4a c0 5b 00 05 00 01
00 00 42 3e 00 19 06 65
38 36 33 30 33 04 64 73
63 78 0a 61 6b 61 6d 61
69 65 64 67 65 c0 4a c0
7e 00 01 00 01 00 00 00
14 00 04 02 16 f4 e2 c0
7e 00 01 00 01 00 00 00
14 00 04 02 16 f4 da c0
7e 00 01 00 01 00 00 00
14 00 04 17 da 5c 61 c0
7e 00 01 00 01 00 00 00
14 00 04 58 dd 1b 33 c0
7e 00 01 00 01 00 00 00
14 00 04 02 16 f4 d9 c0
7e 00 01 00 01 00 00 00
14 00 04 02 16 f4 f1 c0
7e 00 01 00 01 00 00 00
14 00 04 17 da 5c 79 c0
7e 00 01 00 01 00 00 00
14 00 04 17 da 5c 5a
id:            1
flags:         8180
question:
name:          www.bing.com
type:          1
class:         1
answers:       11
name:
c0 0c
name unpacked: www.bing.com
type:          5
class:         1
data:
07 77 77 77 2d 77 77 77
04 62 69 6e 67 03 63 6f
6d 0e 74 72 61 66 66 69
63 6d 61 6e 61 67 65 72
03 6e 65 74 00
cname unpacked: www-www.bing.com.trafficmanager.net
name:
c0 2a
name unpacked: www-www.bing.com.trafficmanager.net
type:          5
class:         1
data:
03 77 77 77 04 62 69 6e
67 03 63 6f 6d 07 65 64
67 65 6b 65 79 c0 4a
cname unpacked: www.bing.com.edgekey.net
name:
c0 5b
name unpacked: www.bing.com.edgekey.net
type:          5
class:         1
data:
06 65 38 36 33 30 33 04
64 73 63 78 0a 61 6b 61
6d 61 69 65 64 67 65 c0
4a
cname unpacked: e86303.dscx.akamaiedge.net
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
02 16 f4 e2
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
02 16 f4 da
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
17 da 5c 61
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
58 dd 1b 33
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
02 16 f4 d9
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
02 16 f4 f1
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
17 da 5c 79
name:
c0 7e
name unpacked: e86303.dscx.akamaiedge.net
type:          1
class:         1
data:
17 da 5c 5a
authority:     0
additional:    0
```
