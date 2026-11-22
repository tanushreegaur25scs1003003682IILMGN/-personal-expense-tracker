# Personal Expense Tracker

## Project Overview
Personal Expense Tracker is a console-based application developed using C++ and SQLite. It helps users record, manage, and track their daily expenses.

## Features
- Add new expenses
- View all expenses
- Calculate total spending
- Delete expenses using ID
- Search expenses by category
- Store data permanently using SQLite

## Technologies Used
- C++
- SQLite
- SQL
- GitHub Codespaces

## Database
The project uses an SQLite database named `expenses.db` to store expense records.

The expenses table contains:
- ID
- Amount
- Category
- Description

## How to Compile
```bash
g++ main.cpp -o expense_tracker -lsqlite3
```

## How to Run
```bash
./expense_tracker
```

## Purpose
This project demonstrates C++ programming, functions, conditional statements, loops, SQL queries, and database connectivity.

## Author
Tanushree Gaur