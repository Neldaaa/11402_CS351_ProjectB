# Software Requirements Specification

## 1. Introduction

### 1.1 Purpose

Define the requirements for Project B: CSV Mini Database & Query Engine, a lightweight tool for loading CSV data and running SQL-like queries in memory.

### 1.2 Scope

The system will:
- Load CSV files into memory
- Infer schema from headers and values
- Execute SELECT queries with WHERE, ORDER BY, and basic aggregates
- Display results and optionally export them as CSV

## 2. Requirements

### Functional Requirements
- Load CSV files and validate structure.
- Infer column types and store table metadata.
- Parse queries with SELECT, FROM, WHERE, ORDER BY.
- Support simple aggregates: COUNT, SUM, AVG, MIN, MAX.
- Output results in table form or export to CSV.

### Nonfunctional Requirements
- Implemented in a high-level language (e.g. Python, Java, C#).
- Handle invalid input with clear messages.
- Run on standard desktop environments.
- Keep architecture modular for future enhancements.

## 3. Use Cases
- Load CSV: User selects a CSV file and system loads it.
- Query Data: User submits a SQL-like query and system returns matching rows.
- Export Results: User saves query results to a CSV file.

## 4. System Overview
- CSV loader reads files, infers schema, and stores rows.
- Query parser interprets simple SQL-like commands.
- Execution engine filters, sorts, and aggregates data.
- Interface presents results and supports export.

## 5. Future Enhancements
- Add JOIN support.
- Allow INSERT/UPDATE/DELETE.
- Support multiple delimiters and advanced CSV quoting.
- Add persistence or caching.
