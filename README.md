# state-machine

[![CI](https://github.com/t-liu93/state-machine/actions/workflows/ci.yaml/badge.svg)](https://github.com/t-liu93/state-machine/actions)

A tiny header-only, fixed-capacity finite state machine for embedded use (ESP32/STM32).
No heap allocations, synchronous event handling by default.

Quick: implement State subclasses, register states/transitions, set initial state,
