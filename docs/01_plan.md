# CSV Mini Database & Query Engine - Project Plan

## Project Overview
Implement a lightweight CSV database system with query capabilities to parse, store, and query data from CSV files using a custom query engine.

## Project Goals
1. Create a CSV parser that reads and validates CSV files
2. Implement an in-memory database structure to store tabular data
3. Build a query engine supporting basic SQL-like operations
4. Provide efficient data retrieval and filtering mechanisms
5. Handle edge cases and data validation

## Phase 1: CSV Parsing & Data Loading
### Tasks
- [ ] Implement CSV file reader with delimiter detection
- [ ] Handle quoted fields and escape characters
- [ ] Support multiple encodings (UTF-8, ASCII)
- [ ] Validate CSV structure (consistent column count)
- [ ] Create data type inference for columns (int, float, string, date)
- [ ] Load data into in-memory data structures

### Deliverables
- CSV parser module with error handling
- Data type inference engine
- Unit tests for parsing edge cases

## Phase 2: In-Memory Database Structure
### Tasks
- [ ] Design table structure with rows and columns
- [ ] Implement column metadata (name, type, constraints)
- [ ] Create row storage with efficient indexing
- [ ] Support primary key identification
- [ ] Implement data insertion and update operations
- [ ] Add data validation and constraint checking

### Deliverables
- Table class with column management
- Row insertion and update methods
- Index structures for fast lookups

## Phase 3: Query Engine - Basic Operations
### Tasks
- [ ] Implement SELECT operation (column projection)
- [ ] Implement WHERE clause (filtering conditions)
- [ ] Support comparison operators (=, !=, <, >, <=, >=)
- [ ] Support logical operators (AND, OR, NOT)
- [ ] Implement ORDER BY (ascending/descending)
- [ ] Implement LIMIT clause

### Deliverables
- Query parser for parsing query strings
- Filter/condition evaluation engine
- Sorting and limiting mechanisms

## Phase 4: Query Engine - Advanced Operations
### Tasks
- [ ] Implement aggregate functions (COUNT, SUM, AVG, MIN, MAX)
- [ ] Implement GROUP BY clause
- [ ] Implement JOIN operations (INNER, LEFT, RIGHT)
- [ ] Support DISTINCT keyword
- [ ] Implement query optimization strategies

### Deliverables
- Aggregate function calculator
- GROUP BY processor
- JOIN implementation

## Phase 5: Query Interface & API
### Tasks
- [ ] Design query API (execute query method)
- [ ] Implement result set structure
- [ ] Support result formatting (table view, JSON, etc.)
- [ ] Add query result caching (optional)
- [ ] Create REPL or command-line interface

### Deliverables
- Query execution API
- Result set class
- CLI or interactive shell

## Phase 6: Testing & Optimization
### Tasks
- [ ] Write comprehensive unit tests
- [ ] Perform integration testing
- [ ] Optimize query performance
- [ ] Test with large datasets
- [ ] Profile memory usage
- [ ] Document API and usage examples

### Deliverables
- Test suite with >80% coverage
- Performance benchmarks
- Documentation and usage guide

## Technical Architecture

### Core Components
1. **CSVParser**: Handles file reading and parsing
2. **Table**: In-memory table representation
3. **QueryParser**: Parses query strings into AST
4. **QueryExecutor**: Executes parsed queries
5. **ResultSet**: Manages and formats query results

## Implementation Details

### CSV Parser
- Read file line by line
- Handle different delimiters (,;|\t)
- Support quoted fields
- Type inference on first pass
- Error handling for malformed data

### Table Structure
- Column metadata: name, type, constraints
- Row storage: list/array of Row objects
- Index structures for fast lookups
- Support for NULL values

### Query Engine
- Tokenizer: break query into tokens
- Parser: build Abstract Syntax Tree (AST)
- Evaluator: traverse AST and execute operations
- Result builder: format results
