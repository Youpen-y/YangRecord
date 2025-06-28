




















示例：
查看头文件属于哪个包？
```bash
dpkg -S 'FILE.h'

# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h
```