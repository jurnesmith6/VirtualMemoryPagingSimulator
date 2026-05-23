# Virtual Memory Paging Simulator

## Overview

This project is a virtual memory management simulator written in C++ that models how an operating system translates virtual addresses to physical memory using a **multilevel page table** and **page replacement policies**.

The simulator processes real memory access traces, performs address translation, tracks page hits and misses, and simulates physical frame allocation and replacement using an **NFU Aging page replacement algorithm**.

It demonstrates core operating systems concepts including hierarchical page tables, address translation, memory residency tracking, page eviction, and performance analysis.

---

## Key Features

- Configurable **multilevel page table architecture**
- Virtual-to-physical address translation
- Demand paging simulation
- Page hit / miss tracking
- Physical frame allocation management
- NFU Aging page replacement algorithm
- Bitstring aging updates for page recency tracking
- Victim page eviction with tie-breaking by access time
- Configurable memory frame limits
- Memory trace processing from binary trace files
- Performance summary and statistics reporting

---

## Operating System Concepts Simulated

This project models core virtual memory system behavior including:

- Virtual page number extraction
- Offset extraction
- Multilevel page table traversal
- Page table entry creation and invalidation
- Physical frame mapping
- Page faults
- Page replacement
- Memory residency tracking
- Approximate LRU behavior using aging bitstrings
