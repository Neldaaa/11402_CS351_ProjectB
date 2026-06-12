# Software Design Specification 

## 1. Overview

Project B: CSV Mini Database & Query Engine is a lightweight system for storing CSV data and executing simple SQL-like queries. The engine reads CSV files, maintains in-memory tables, and supports select, filter, and projection operations.

## 2. Scope

- Input: CSV files with header rows.
- Output: query results as formatted rows or exported CSV.
- Supported operations: SELECT, WHERE, ORDER BY, AND/OR, basic comparisons.
- Primary users: developers and analysts needing a small local query engine.

## 3. Architecture

The system is organized in three main layers:

- Storage Layer: CSV parsing and table representation.
- Query Engine: parsing query strings and executing operations.
- Interface Layer: CLI or programmatic API for loading data and running queries.

## 4. Components

1. CSV Loader
   - Reads CSV files.
   - Validates headers and row consistency.
   - Converts rows into typed records or string values.

2. Table Model
   - Represents tables as collections of rows with named columns.
   - Supports schema metadata and row access.

3. Query Parser
   - Parses SQL-like syntax.
   - Builds an internal query representation (select list, source, filters, order clause).

4. Execution Engine
   - Applies filters to table rows.
   - Projects selected columns.
   - Sorts results when requested.

5. Result Formatter
   - Formats query output as table text or CSV.

## 5. Data Model

- Table: header columns + rows.
- Row: list of values mapped to column names.
- Value: string or typed primitive for comparisons.

## 6. Query Processing

- Parse query into AST-like structure.
- Validate column references against table schema.
- Evaluate WHERE conditions row by row.
- Generate selected output columns.
- Apply ORDER BY if present.

## 7. Interfaces

- Command-line interface for loading CSV and executing queries.
- Optional functions for programmatic use.

## 8. Nonfunctional requirements

- Performance: efficient in-memory processing for moderate sized CSV files.
- Usability: simple syntax and clear error messages.
- Portability: runs on standard environments with minimal dependencies.

## 9. Constraints and assumptions

- CSV files are well-formed with consistent columns.
- No distributed storage or indexing required.
- Data fits in available memory for targeted use cases.

## 10. Future extension points

- Support JOIN operations.
- Add aggregate functions (SUM, COUNT).
- Enable persistence or caching of loaded tables.
