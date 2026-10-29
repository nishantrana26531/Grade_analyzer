# Student Academic Performance & Grade Analyzer

A menu-driven command-line program in C that records student marks, calculates grades and GPA,
shows class statistics and a rank list, and saves data to a file.

**Course:** Project I (BITP 141), BIT Semester I, Sofan Western Academy
**Author:** _Suraj Rana_

## Features
- Add students (unique ID, name, marks in 5 subjects)
- Automatic total, percentage, grade, GPA and Pass/Fail
- Search by ID with subject-wise marks
- Class statistics: average, highest, lowest, pass rate, subject averages
- Rank list (sorted by percentage)
- Save/load records (`students.txt`)
- Input validation for every entry

## STREAME Connection
Science (statistics) · Technology (C automation) · Research (data analysis) ·
Engineering (modular design/testing) · Management (record keeping) · Entrepreneurship (low-cost alternative to paid software)

## Project Structure
```
grade-analyzer/
├── src/main.c            # source code
├── docs/Project_Report.md# report, pseudocode, flowcharts, test cases
├── README.md
└── .gitignore
```

## Build and Run
```bash
gcc -std=c99 -Wall -Wextra -o grade_analyzer src/main.c
./grade_analyzer          # on Windows: grade_analyzer.exe
```

## Grading Scale
| % | Grade | GPA |
|---|---|---|
| 90+ | A+ | 4.0 |
| 80+ | A | 3.6 |
| 70+ | B+ | 3.2 |
| 60+ | B | 2.8 |
| 50+ | C+ | 2.4 |
| 40+ | C | 2.0 |
| <40 | F | 0.0 |

A student must score at least 40 in every subject to pass.

## Sample Run
```
====== Student Grade Analyzer ======
1. Add student ...
Enter choice (0-6): 2

ID    Name                    Total  Percent Grade   GPA  Result
101   Suraj Rana             413.00   82.60% A      3.60  Pass
```

## Testing
9 test cases (valid input, invalid marks/name/menu, duplicate ID, failed subject, statistics, ranking, file persistence, empty data) are documented in `docs/Project_Report.md`.

## Limitations / Future Work
Max 50 students; no edit/delete yet; could add CSV export and a GUI.
