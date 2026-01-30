# C++ HTTP Server

## Overview
Lightweight HTTP/1.1 server built from scratch in **C++** that can server HTML, CSS, JS and even favicons.'

This project demonstrates low level networking, socket concepts and basic usage of HTTP without using any external libraries.

## Features
- Servers files:
  - root.html
  - style.css
  - script.js
  - favicon.ico
- Routes are based on the path
- Can handle 404 response
- Simple logging for connections
- C++17

---

## Requirements
- OS: Arch linux (Or any other linux distro, but this was written on Arch linux)
- Compiler: gcc or clang are fine as long as the version of C++ is minimum 17. This project used clang

### Clone Repo and change into its directory
```
git clone https://github.com/your-username/cpp-http-server.git
cd cpp-http-server
```

### Compile and run
```
g++ main.cpp -o server -std=c++17
./server
```

### URL for browser
```
http://127.0.0.1:8080/
```
