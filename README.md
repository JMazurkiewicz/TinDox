# TinDox - simple network filesystem

[![Server build](https://github.com/JMazurkiewicz/TinDox/actions/workflows/server.yaml/badge.svg)](https://github.com/JMazurkiewicz/TinDox/actions/workflows/server.yaml)

## Goals

* Design simple network filesystem protocol
* Implement efficient server with event-driven architecture
* Create clients for various platforms - command line, desktop and mobile

## Projects

* TinDox Server (TDS) - `server` directory
* TinDox mobile client for Android (unfinished) - `mobile` directory
* TinDox desktop client - `desktop` directory
* TinDox command line client for Linux - `cli` directory

## Requirements

TDS and command line client require CMake and C++20 compiler (with `<ranges>` support).

Mobile client requires any Java compiler.

## Team

* Server - [Jakub Mazurkiewicz](https://github.com/JMazurkiewicz)
* Mobile client - [Damian Piotrowski](https://github.com/dam1508)
* Desktop client - [Anna Pyrka](https://github.com/annapyrka)
* Console client - [Łukasz Reszka](https://github.com/LukaszReszka)
