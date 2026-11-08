# Student Record Management System (C++ & Qt)

A modular, GUI-based Student Record Management System developed in C++ using the **Qt Widgets** framework. This project was developed as a university programming fundamentals semester project, demonstrating object-oriented programming, sequential CSV file I/O, event-driven programming (Qt Signals & Slots), and UI state machine management.

---

## 📋 Features & Data Fields

The application provides an intuitive graphical interface to manage student records with the following fields:
1. **Student Name**: Full name of the student.
2. **Roll Number**: Unique identifier (Primary Key) used to locate, update, and manage records.
3. **Father's Name**: Father's name.
4. **Caste**: Caste / Community.
5. **District**: Domicile / Residential district.
6. **GPA**: Grade Point Average (validated between `0.00` and `4.00`).

---

## 🗄️ File Handling Specification

- All records are saved sequentially in `student_records.txt`.
- Data is formatted as standard **Comma-Separated Values (CSV)**:
  ```csv
  Name,RollNumber,FatherName,Caste,District,GPA
  ```
- **File Persistence**:
  - **Add Record**: Appends new records to `student_records.txt` and updates in-memory cache.
  - **Update Record**: Updates the record in-memory and rewrites the file using Roll Number as the unique identifier.
  - **Delete Record**: Removes the record from memory and rewrites the updated list to the file.
  - **Startup**: Automatically reads existing records from `student_records.txt` into memory.

---

## 🔄 State Machine & Button Logic

The application implements state-based button toggling:

| Button | Initial / New Record State | Browsing Records State (via Next / Prev) |
|---|---|---|
| **Add Record** | **ENABLED** | **DISABLED** |
| **Update Record** | **DISABLED** | **ENABLED** |
| **Delete Record** | **DISABLED** | **ENABLED** |
| **Next / Previous** | **ENABLED** (Navigates if records exist) | **ENABLED** (Cycles through records) |
| **Roll Number Field** | **EDITABLE** (Input new unique ID) | **READ-ONLY** (Protects primary key) |
| **New / Clear Form** | **ENABLED** (Clears inputs) | **ENABLED** (Returns to Initial State) |

---

## 🛡️ Input Validation & Error Handling

- **Mandatory Fields**: Ensures no field is left blank before adding or updating.
- **Delimiter Protection**: Rejects input containing commas (`,`) to prevent corrupting the CSV format.
- **GPA Range Verification**: Validates that GPA is a valid floating-point number between `0.00` and `4.00`.
- **Primary Key Uniqueness**: Prevents adding a student with a Roll Number that already exists in the records.
- **File Access Safeguards**: Displays descriptive error dialogs (`QMessageBox::critical`) if the file fails to open for reading or writing.
- **Confirmation Prompts**: Requests explicit user confirmation (`QMessageBox::question`) before updating or deleting a record.

---

## 📁 Project Structure

```text
STUDENT RECORD PROJECT/
│
├── student.h               # Data struct & CSV parsing/serialization helpers
├── mainwindow.h            # Main window class declaration & UI state definitions
├── mainwindow.cpp          # Full UI layout, signal-slot connections, and business logic
├── main.cpp                # Application entry point (main Qt event loop)
├── StudentRecordSystem.pro # qmake configuration file
├── CMakeLists.txt          # Modern CMake configuration file
├── student_records.txt     # Persistent CSV data storage file
└── README.md               # Documentation and compilation guide
```

---

## 🚀 How to Build and Run

### Option 1: Using Qt Creator (Recommended)
1. Open **Qt Creator**.
2. Click **File** > **Open File or Project...**.
3. Select `StudentRecordSystem.pro` (or `CMakeLists.txt`).
4. Choose your configured Qt Kit (Qt 5 or Qt 6 with MinGW / MSVC / GCC).
5. Click the green **Run** button (or press `Ctrl + R`).

### Option 2: Using CMake from Terminal
```bash
mkdir build
cd build
cmake ..
cmake --build .
./StudentRecordSystem
```

### Option 3: Using qmake from Terminal
```bash
qmake
make      # or 'mingw32-make' on Windows
./StudentRecordSystem
```
