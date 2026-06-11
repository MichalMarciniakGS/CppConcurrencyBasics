# CppConcurrencyBasics
A learning repository for small C++ concurrency and multithreading exercises.

This repo is meant to document steady progress in modern C++ concurrency, starting from very small examples and gradually moving toward more practical, medium-sized projects. The focus is on understanding concepts clearly, writing clean code, and building a visible learning path rather than jumping straight into large, production-style systems.

Purpose
The main goals of this repository are:

- Learn modern C++ concurrency step by step

- Practice with C++20 threading-related standard library features

- Build intuition around thread lifetime, synchronization, cancellation, and communication

- Keep small experiments organized in one place

- Create a public record of learning and progress over time

What this repository contains
This is a multi-project repository.

Each subfolder is a separate small exercise, experiment, or mini-project focused on one concurrency topic. Most folders should contain their own code, a short local README.

Examples of topics that belong here:

- Basic std::thread usage

- std::jthread and automatic joining

- std::osyncstream

- Race conditions

- std::mutex

- std::scoped_lock

- std::condition_variable

- Producer-consumer patterns

- std::future and std::async

- Cooperative cancellation with std::stop_token

- Simple thread pool experiments
