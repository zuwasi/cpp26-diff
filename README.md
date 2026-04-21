# C++26 vs Pre-C++26 — Parasoft C++test Static Analysis Diff

## Purpose
Compare static analysis results between **pre-C++26** and **C++26** versions of the same
functionality to demonstrate how C++26 features reduce violations.

## Structure
```
cpp26-parasoft-diff/
├── pre_cpp26/          # C++17/20 version — expect MORE violations
│   └── src/
│       ├── sensor_data.h
│       ├── sensor_data.cpp
│       ├── data_processor.h
│       ├── data_processor.cpp
│       ├── config_manager.h
│       ├── config_manager.cpp
│       └── main.cpp
├── cpp26/              # C++26 version — expect FEWER violations
│   └── src/
│       ├── sensor_data.h
│       ├── sensor_data.cpp
│       ├── data_processor.h
│       ├── data_processor.cpp
│       ├── config_manager.h
│       ├── config_manager.cpp
│       └── main.cpp
└── README.md
```

## Targeted Parasoft Rules (Expected Diff)

| Rule ID | Description | Pre-C++26 | C++26 |
|---------|-------------|-----------|-------|
| MISRA-4.8.1 / CERT-EXP53 | Uninitialized variable read | VIOLATION | FIXED (erroneous behavior model) |
| MISRA-0.1.1 / CWE-563 | Unused variable | VIOLATION | FIXED (placeholder `_` variables) |
| CERT-CTR50 / MISRA-5.2.1 | Container bounds access | VIOLATION | FIXED (hardened library / `at()` + span) |
| CERT-ERR33 / MISRA-0.4.2 | Unchecked preconditions | VIOLATION | FIXED (contracts: `pre`/`post`) |
| CERT-DCL50 | Variadic function misuse | VIOLATION | FIXED (variadic templates / `= delete("reason")`) |
| AUTOSAR-A0.4.2 | Missing assertions | VIOLATION | FIXED (`contract_assert`) |
| MISRA-21.6.1 / CERT-ERR07 | Unsafe C-style I/O | VIOLATION | FIXED (modern `std::print`) |
| CWE-120 / MISRA-5.0.1 | Buffer overflow potential | VIOLATION | FIXED (`inplace_vector` / span) |
| AUTOSAR-M0.1.3 | Dead/unreachable code | VIOLATION | FIXED (`constexpr` improvements) |

## How to Scan

### Pre-C++26:
```bash
cpptestcli -config "builtin://MISRA C++ 2023" -input pre_cpp26/cpptestproject.bdf
```

### C++26:
```bash
cpptestcli -config "builtin://MISRA C++ 2023" -input cpp26/cpptestproject.bdf
```

### Compare Results:
Export both reports to CSV and diff, or use DTP for side-by-side comparison.

## Build (for syntax verification)
```bash
# Pre-C++26
g++ -std=c++20 -Wall -Wextra pre_cpp26/src/*.cpp -o pre_cpp26_demo

# C++26 (requires GCC 15+ or GCC trunk)
g++ -std=c++2c -Wall -Wextra cpp26/src/*.cpp -o cpp26_demo
```
