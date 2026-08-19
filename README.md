# Tree Editor

Desktop application for managing hierarchical object trees with database storage.

The application allows users to create, edit, filter, and manage tree structures with nodes and leaves. Data is stored in PostgreSQL, and the user interface is implemented using Qt Widgets.

## Features

- Hierarchical tree structure management
- PostgreSQL database storage
- Tree visualization using Qt Widgets
- Add, edit, and delete leaves
- Delete nodes with all child leaves
- Filter leaves by name
- Filter leaves by numeric value
- Import and export tree data
- Display tree statistics:
  - number of nodes
  - number of leaves
  - minimum value
  - maximum value
- Unit tests

## Technologies

- C++17
- Qt 6.10.3
- PostgreSQL
- CMake
- MinGW 13.1.0
- GoogleTest

## Database Structure

The application uses PostgreSQL with the following schema:

```sql
CREATE TABLE nodes (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL
);

CREATE TABLE leaves (
    id SERIAL PRIMARY KEY,
    node_id INTEGER NOT NULL REFERENCES nodes(id) ON DELETE CASCADE,
    name TEXT NOT NULL,
    value DOUBLE PRECISION NOT NULL
);
