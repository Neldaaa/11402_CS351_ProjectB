# Project B — CSV Mini Database & Query Engine: Test Plan
## 1. Overview
This document describes the test plan for the CSV Mini Database & Query Engine ("Project B"). It covers objectives, scope, test items, environments, detailed test cases, acceptance criteria, schedule, responsibilities, and risks.

## 2. Objectives
- Verify correctness of CSV parsing, storage, and indexing.
- Verify correctness and performance of query engine features (SELECT, WHERE, JOIN, GROUP BY, ORDER BY, aggregates).
- Validate robustness against malformed input, large files, and concurrency.
- Ensure proper error reporting and graceful failure modes.

## 3. Scope
- In scope: CSV import, schema inference (or schema definition), basic storage layer, query parser, planner, execution engine, simple indexes, basic concurrency behavior, and persistence (if implemented).
- Out of scope: full SQL standard compliance, distributed transactions, advanced query optimization.

## 4. Test Items
- CSV parser and loader
- Data storage and retrieval APIs
- Query parser and planner
- Execution engine (scan, filter, project, join, aggregate, sort)
- Index creation and usage
- Error handling and logging
- Performance and resource usage

## 5. Test Types
- Unit tests: individual modules (parser, lexer, planner, operators)
- Integration tests: end-to-end import -> query -> result
- System tests: large datasets, resource limits
- Regression tests: fixed suite to prevent reintroduction of bugs
- Stress tests: concurrency and large-file behavior
- Fuzz tests: malformed CSV and unexpected inputs

## 6. Detailed Test Cases
Each test case includes: ID, Purpose, Preconditions, Steps, Expected Result.

- TC-001: Parse simple CSV
	- Purpose: verify parser reads headers and rows
	- Preconditions: file simple.csv: header: id,name\n rows: 1,Alice\n2,Bob
	- Steps: load file and read first two rows
	- Expected: two rows returned with correct values and types as strings

- TC-002: Parse quoted fields and commas
	- Purpose: handle quoted fields containing commas and quotes
	- Preconditions: file quotes.csv: header: id,desc\n1,"a, \"quoted\" text"\n
	- Steps: load file and inspect desc for row 1
	- Expected: desc == 'a, "quoted" text'

- TC-003: Missing fields and variable columns
	- Purpose: handle rows with fewer or more fields than header
	- Preconditions: file missing.csv with inconsistent columns
	- Steps: load and inspect row mapping behavior
	- Expected: missing fields represented as NULL/empty; extra fields ignored or stored per spec

- TC-004: Type inference / schema definition
	- Purpose: confirm numeric/date detection (if implemented) or correct string fallback
	- Steps: load file with ints, floats, dates
	- Expected: values typed correctly or all as strings if no inference

- TC-005: Basic SELECT
	- Purpose: SELECT specific columns
	- Preconditions: table users(id,name,age)
	- Steps: run SELECT name FROM users WHERE id=1
	- Expected: returns row with name for id 1

- TC-006: WHERE filters and comparison operators
	- Purpose: test =, !=, <, >, <=, >=, AND, OR, NOT
	- Steps: run multiple queries combining ops
	- Expected: expected rows returned for each predicate

- TC-007: ORDER BY and LIMIT
	- Purpose: sorting correctness and limiting
	- Steps: SELECT * FROM users ORDER BY age DESC LIMIT 3
	- Expected: top 3 rows sorted by age descending

- TC-008: Aggregations and GROUP BY
	- Purpose: aggregation functions (COUNT, SUM, AVG, MIN, MAX)
	- Steps: SELECT country, COUNT(*) FROM users GROUP BY country
	- Expected: counts per country match dataset

- TC-009: INNER JOIN correctness
	- Purpose: join two tables on key
	- Preconditions: users(id,dept_id), depts(id,name)
	- Steps: SELECT u.name,d.name FROM users u JOIN depts d ON u.dept_id=d.id
	- Expected: matched rows only, no duplicates beyond relational result

- TC-010: LEFT/RIGHT/OUTER JOIN behavior
	- Purpose: null-preserving joins
	- Steps: run LEFT JOIN and check for nulls when no match
	- Expected: unmatched side has null columns

- TC-011: Complex query with filter, join, group, order
	- Purpose: end-to-end plan generation and execution
	- Steps: run a query combining features
	- Expected: correct result and reasonable execution time for dataset

- TC-012: Index usage and correctness
	- Purpose: verify index speeds up queries and returns correct rows
	- Steps: create index on users(id), run SELECT WHERE id=... and compare plan/latency
	- Expected: index used (if planner reports) and results correct

- TC-013: Concurrency and isolation (if applicable)
	- Purpose: basic concurrent reads and writes consistency
	- Steps: concurrent import and queries; concurrent updates if supported
	- Expected: reads not crash; defined isolation level respected

- TC-014: Large-file performance
	- Purpose: ensure system can ingest and query large CSVs
	- Steps: import 1M-row CSV and run representative queries
	- Expected: completes within acceptable time/memory bounds or documented limits

- TC-015: Malformed input and error messages
	- Purpose: graceful handling and clear errors
	- Steps: feed truncated quoted field, invalid encoding
	- Expected: descriptive error, no crash, partial progress is either rolled back or consistent

- TC-016: Persistence and restart (if implemented)
	- Purpose: durability of stored data across restarts
	- Steps: load data, stop system, restart, run queries
	- Expected: data available and intact

## 7. Acceptance Criteria
- All correctness unit/integration tests pass.
- No unhandled crashes on curated and malformed inputs.
- Performance: typical queries on medium datasets complete within target times (define specific SLA per project capacity).

