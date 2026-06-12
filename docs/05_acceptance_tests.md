# Acceptance Plan for Project B: CSV Mini Database & Query Engine

## Objective
Verify that the CSV Mini Database and Query Engine can ingest CSV data, execute queries accurately, handle invalid input cleanly, and produce expected results for common operations.

## Acceptance Criteria
- The system accepts standard CSV files with headers.
- The database can load multiple records and preserve column names.
- Simple SELECT queries return correct rows and columns.
- WHERE filters support comparison and logical operators.
- Aggregation functions produce correct summaries.
- Invalid CSV or malformed queries return clear error messages.

## Test Scenarios

### 1. CSV Ingestion
- Given a valid CSV file with headers and rows
- When the file is loaded into the database
- Then the system stores the data and confirms successful import
- And the schema matches the CSV header columns

### 2. Basic Query Execution
- Given loaded CSV data
- When a query selects one or more columns
- Then the result includes only the requested columns
- And the row order matches the source data unless otherwise specified

### 3. Filtering and Conditions
- Given loaded data with numeric and string fields
- When a query uses WHERE conditions (e.g., equality, inequality, greater than)
- Then only rows satisfying the condition are returned
- When multiple conditions are combined with AND/OR
- Then the engine returns rows matching the combined criteria

### 4. Aggregation and Grouping
- Given loaded numeric data
- When a query uses aggregation functions such as COUNT, SUM, AVG, MIN, MAX
- Then the results show correct aggregated values
- If grouping is supported, grouped results must be correct per group key

### 5. Error Handling
- Given an invalid CSV file format
- When the import is attempted
- Then the system reports a parse error and does not crash
- Given a malformed query
- When the query is executed
- Then the system returns a descriptive query error message

### 6. Output Verification
- Given a successful query execution
- When results are returned
- Then output is formatted consistently for downstream use or display
- And numeric and string values are preserved correctly

## Notes
- Keep acceptance tests focused on end-to-end behavior, not internal implementation details.
- Use representative CSV data sets that include header rows, numeric values, and text values.
- Confirm the system behaves predictably with empty result sets and no-matches.
