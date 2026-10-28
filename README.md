# Custom Smart Pointers — C++

## Overview

Custom implementations of unique and shared smart pointers, developed to explore how modern C++ manages dynamically allocated resources, ownership, and object lifetimes.

This project was created as a hands-on exercise to understand the underlying mechanisms of `std::unique_ptr` and `std::shared_ptr`, with a focus on RAII, move semantics, and reference counting.

## Features

### UniquePtr<T> — Exclusive Ownership

- Manages a dynamically allocated resource through exclusive ownership.
- Disables copying to prevent multiple pointers from independently owning the same resource.
- Supports move construction and move assignment to transfer ownership.
- Automatically releases the owned resource upon destruction.
- Provides `get()`, `release()`, and `reset()` operations.
- Supports pointer-like access through `operator*` and `operator->`.

### SharedPtr<T> — Shared Ownership

- Supports multiple pointers sharing ownership of the same dynamically allocated resource.
- Uses a dynamically allocated reference counter to track active owners.
- Implements copy construction and copy assignment to share ownership and update the reference count.
- Implements move construction and move assignment to transfer ownership without incrementing the reference count.
- Automatically releases the managed resource when the last owner is destroyed.
- Provides `get()`, `use_count()`, and `reset()` operations.
- Supports `operator*` and `operator->`.

## Technical Concepts

- C++ templates
- RAII (Resource Acquisition Is Initialization)
- Dynamic memory allocation
- Exclusive and shared resource ownership
- Reference counting
- Rule of Five
- Copy and move semantics
- Object lifetime and deterministic destruction
- Operator overloading

## Testing

The implementations were tested using custom objects with constructor and destructor logging to observe resource lifetimes.

Test scenarios include:

- Ownership transfer through move construction and assignment
- Copying and assigning shared owners
- Tracking reference counts across multiple owners
- Resetting and releasing managed resources
- Self-assignment
- Copying and moving empty smart pointers
- Verifying resource destruction when ownership ends

## Learning Objectives

The goal of this project was to gain a deeper understanding of how smart pointers manage memory and prevent common ownership-related errors.

Implementing these mechanisms manually helped me explore the responsibilities associated with managing dynamically allocated objects, transferring ownership safely, and tracking shared resources.

## Limitations

These are educational implementations rather than production-ready replacements for the standard library smart pointers.

They focus on fundamental ownership mechanisms and do not provide the complete functionality or guarantees of their standard library equivalents.
