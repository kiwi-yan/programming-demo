# programming-demo
## Introduction
这是一个编程DEMO 'Library' {实现语言：C/C++}. 主要是某些编程模型说明, 或者是一些API的最简化使用方法. 必要时会以附带笔记说明. 还有一些基本数据结构的C语言实现.

## File List
- [x] [`pattern-demo/demo-tcp.c`](https://github.com/kiwi-yan/programming-demo/blob/master/pattern-demo/demo-tcp.c):
        一个简单的多进程并发TCP服务器
- [x] [`pattern-demo/master-process.c`](https://github.com/kiwi-yan/programming-demo/blob/master/pattern-demo/master-process.c):
        一个简单的主-从(Master-Slave)模型(进程池)
- [x] [`api-demo/select-demo.c`](https://github.com/kiwi-yan/programming-demo/blob/master/api-demo/select-demo.c):
        select函数的用法说明
- [x] [`api-demo/epoll-lt.c`](https://github.com/kiwi-yan/programming-demo/blob/master/api-demo/epoll-lt.c):
        epoll水平触发简单用法说明
- [x] [`api-demo/epoll-et.c`](https://github.com/kiwi-yan/programming-demo/blob/master/api-demo/epoll-et.c):
        epoll边缘触发简单用法说明
- [ ] `api-demo/poll-demo.c`:
        poll函数的用法说明
- [ ] `notes/multiplexing.md`:
        I/O多路复用笔记
- [ ] `pattern-demo/master-process2.c`:
        使用Master-Slave模型的简单网络服务器(进程池)
- [ ] `data-structures/linkedlist/`:
        链表的实现

## License
The MIT License (MIT)

Copyright (c) 2015 kiwi-yan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

许可证请查看 [`LICENSE`](https://github.com/kiwi-yan/programming-demo/blob/master/LICENSE).
