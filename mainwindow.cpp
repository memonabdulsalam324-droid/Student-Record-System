#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QRegularExpressionValidator>
#include <QDoubleValidator>
#include <QFont>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_currentIndex(-1),
      m_currentState(AppState::InitialOrNew)
{
    // Resolve file path to "student_records.txt" in the current directory / app directory
    m_filePath = QDir::current().filePath("student_records.txt");

    // Initialize UI components and layout
    setupUi();

    // Apply clean styling
    setupStyles();

    // Load any existing records saved in the file on startup
    loadRecordsFromFile();

    // Set application to its initial state
    // (Add button enabled; Update and Delete buttons disabled)
    setAppState(AppState::InitialOrNew);

    // Update the counter display
    updateRecordCounterLabel();
}

/**
 * @brief Constructs and lays out all GUI widgets programmatically.
 */
void MainWindow::setupUi()
{
    // Configure main window properties
    setWindowTitle("Student Record Management System");
    setMinimumSize(580, 520);
    resize(620, 560);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(16);

    // -------------------------------------------------------------
    // Header Title and Counter Section
    // -------------------------------------------------------------
    QLabel *headerTitle = new QLabel("Student Record Management System", this);
    QFont titleFont("Segoe UI", 16, QFont::Bold);
    headerTitle->setFont(titleFont);
    headerTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerTitle);

    m_counterLabel = new QLabel("Records Loaded: 0", this);
    m_counterLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_counterLabel);

    // -------------------------------------------------------------
    // Form Input Group
    // -------------------------------------------------------------
    QGroupBox *formGroupBox = new QGroupBox("Student Information", this);
    QFormLayout *formLayout = new QFormLayout(formGroupBox);
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // 1. Student Name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter full student name (e.g., Abdul salam)");
    formLayout->addRow("<b>Student Name:</b>", m_nameEdit);

    // 2. Roll Number (Unique Identifier)
    m_rollEdit = new QLineEdit(this);
    m_rollEdit->setPlaceholderText("Enter unique roll number (e.g., K26SW025)");
    formLayout->addRow("<b>Roll Number:</b>", m_rollEdit);

    // 3. Father's Name
    m_fatherEdit = new QLineEdit(this);
    m_fatherEdit->setPlaceholderText("Enter father's name (e.g., Naveed Akhtar)");
    formLayout->addRow("<b>Father's Name:</b>", m_fatherEdit);

    // 4. Caste
    m_casteEdit = new QLineEdit(this);
    m_casteEdit->setPlaceholderText("Enter caste / community (e.g., Memon)");
    formLayout->addRow("<b>Caste:</b>", m_casteEdit);

    // 5. District
    m_districtEdit = new QLineEdit(this);
    m_districtEdit->setPlaceholderText("Enter district (e.g., Naushahro Feroze)");
    formLayout->addRow("<b>District:</b>", m_districtEdit);

    // 6. GPA
    m_gpaEdit = new QLineEdit(this);
    m_gpaEdit->setPlaceholderText("Enter GPA between 0.00 and 4.00 (e.g., 3.65)");
    formLayout->addRow("<b>GPA (0.00 - 4.00):</b>", m_gpaEdit);

    mainLayout->addWidget(formGroupBox);

    // -------------------------------------------------------------
    // Buttons Section
    // -------------------------------------------------------------
    QGroupBox *buttonGroupBox = new QGroupBox("Operations & Navigation", this);
    QVBoxLayout *btnGroupLayout = new QVBoxLayout(buttonGroupBox);
    btnGroupLayout->setSpacing(10);

    // Row 1: Action Buttons (Add, Update, Delete)
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(12);

    m_btnAdd = new QPushButton("➕ Add Record", this);
    m_btnUpdate = new QPushButton("💾 Update Record", this);
    m_btnDelete = new QPushButton("🗑️ Delete Record", this);

    actionLayout->addWidget(m_btnAdd);
    actionLayout->addWidget(m_btnUpdate);
    actionLayout->addWidget(m_btnDelete);
    btnGroupLayout->addLayout(actionLayout);

    // Row 2: Navigation and Utility Buttons (Previous, Next, Clear/New)
    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->setSpacing(12);

    m_btnPrevious = new QPushButton("◀ Previous", this);
    m_btnNext = new QPushButton("Next ▶", this);
    m_btnClear = new QPushButton("🔄 New / Clear Form", this);

    navLayout->addWidget(m_btnPrevious);
    navLayout->addWidget(m_btnNext);
    navLayout->addWidget(m_btnClear);
    btnGroupLayout->addLayout(navLayout);

    mainLayout->addWidget(buttonGroupBox);

    // -------------------------------------------------------------
    // Status Bar & Information Message
    // -------------------------------------------------------------
    m_statusMessageLabel = new QLabel("Ready. Fill details and click 'Add Record' or click 'Next' to browse.", this);
    m_statusMessageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusMessageLabel);

    // -------------------------------------------------------------
    // Signal - Slot Connections
    // -------------------------------------------------------------
    // Connect button click signals to their respective member slot functions
    connect(m_btnAdd,      &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_btnNext,     &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(m_btnPrevious, &QPushButton::clicked, this, &MainWindow::onPreviousClicked);
    connect(m_btnUpdate,   &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(m_btnDelete,   &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(m_btnClear,    &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

/**
 * @brief Sets clean, modern widget styling (QSS).
 */
void MainWindow::setupStyles()
{
    setStyleSheet(
        "QMainWindow { background-color: #F4F6F9; }"
        "QGroupBox { font-weight: bold; font-size: 13px; border: 1px solid #D0D7DE; "
        "            border-radius: 8px; margin-top: 12px; padding: 14px; background-color: #FFFFFF; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 14px; padding: 0 5px; color: #1E293B; }"
        "QLineEdit { border: 1px solid #CBD5E1; border-radius: 6px; padding: 7px 10px; font-size: 13px; background-color: #FAFAFA; }"
        "QLineEdit:focus { border: 1.5px solid #2563EB; background-color: #FFFFFF; }"
        "QLineEdit:read-only { background-color: #E2E8F0; color: #475569; }"
        "QPushButton { font-weight: 600; font-size: 12px; padding: 8px 14px; border-radius: 6px; border: none; }"
        "QPushButton:disabled { background-color: #E2E8F0; color: #94A3B8; }"
        "QPushButton#btnAdd { background-color: #16A34A; color: white; }"
        "QPushButton#btnAdd:hover { background-color: #15803D; }"
        "QPushButton#btnUpdate { background-color: #2563EB; color: white; }"
        "QPushButton#btnUpdate:hover { background-color: #1D4ED8; }"
        "QPushButton#btnDelete { background-color: #DC2626; color: white; }"
        "QPushButton#btnDelete:hover { background-color: #B91C1C; }"
        "QPushButton#btnNext, QPushButton#btnPrevious { background-color: #475569; color: white; }"
        "QPushButton#btnNext:hover, QPushButton#btnPrevious:hover { background-color: #334155; }"
        "QPushButton#btnClear { background-color: #64748B; color: white; }"
        "QPushButton#btnClear:hover { background-color: #475569; }"
        "QLabel { color: #334155; font-size: 13px; }"
    );

    // Assign object names for specific CSS styling
    m_btnAdd->setObjectName("btnAdd");
    m_btnUpdate->setObjectName("btnUpdate");
    m_btnDelete->setObjectName("btnDelete");
    m_btnNext->setObjectName("btnNext");
    m_btnPrevious->setObjectName("btnPrevious");
    m_btnClear->setObjectName("btnClear");
}

/**
 * @brief State Machine controller: toggles button states and field editability
 *        strictly adhering to the project's interactive specifications.
 *
 * State 1 (Initial / Adding New Record):
 *   - Add button is ENABLED.
 *   - Update and Delete buttons are DISABLED.
 *   - Roll Number field is EDITABLE (to enter new unique roll number).
 *
 * State 2 (Browsing Existing Records):
 *   - Add button is DISABLED.
 *   - Update and Delete buttons are ENABLED.
 *   - Roll Number field is READ-ONLY (preserves Roll Number as the unique primary key).
 */
void MainWindow::setAppState(AppState state)
{
    m_currentState = state;

    if (state == AppState::InitialOrNew) {
        m_btnAdd->setEnabled(true);
        m_btnUpdate->setEnabled(false);
        m_btnDelete->setEnabled(false);

        // Roll number can be entered for new students
        m_rollEdit->setReadOnly(false);

        m_statusMessageLabel->setText("Mode: Adding New Record. (Fill form & click 'Add Record')");
    } else { // AppState::Browsing
        m_btnAdd->setEnabled(false);
        m_btnUpdate->setEnabled(true);
        m_btnDelete->setEnabled(true);

        // Roll number acts as the unique identifier; lock it during browsing/updates
        m_rollEdit->setReadOnly(true);

        m_statusMessageLabel->setText("Mode: Browsing Records. (Use 'Next' / 'Previous' to navigate, or 'Update' / 'Delete')");
    }
}

/**
 * @brief Reads all student records from "student_records.txt" into memory.
 * @return True if operation was successful or file did not exist yet; False on read error.
 */
bool MainWindow::loadRecordsFromFile()
{
    m_records.clear();

    QFile file(m_filePath);
    if (!file.exists()) {
        // File does not exist yet; will be created when the first record is added
        return true;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error",
                              QString("Could not open file for reading:\n%1\nError: %2")
                                  .arg(m_filePath, file.errorString()));
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue; // Skip empty lines
        }

        bool ok = false;
        Student s = Student::fromCsv(line, &ok);
        if (ok) {
            m_records.append(s);
        }
    }

    file.close();
    return true;
}

/**
 * @brief Overwrites the entire "student_records.txt" with current in-memory records.
 *        Used after Update and Delete operations.
 * @return True on success; False on failure.
 */
bool MainWindow::saveAllRecordsToFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error",
                              QString("Could not open file for writing:\n%1\nError: %2")
                                  .arg(m_filePath, file.errorString()));
        return false;
    }

    QTextStream out(&file);
    for (const Student &s : m_records) {
        out << s.toCsv() << "\n";
    }

    file.close();
    return true;
}

/**
 * @brief Appends a single record to "student_records.txt" in CSV format.
 * @param student Record to append.
 * @return True on success; False on failure.
 */
bool MainWindow::appendRecordToFile(const Student &student)
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error",
                              QString("Could not open file to append record:\n%1\nError: %2")
                                  .arg(m_filePath, file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << student.toCsv() << "\n";
    file.close();
    return true;
}

/**
 * @brief Populates the GUI input fields with the student's data at the given index.
 */
void MainWindow::displayRecord(int index)
{
    if (index < 0 || index >= m_records.size()) {
        return;
    }

    const Student &s = m_records.at(index);
    m_nameEdit->setText(s.name);
    m_rollEdit->setText(s.rollNumber);
    m_fatherEdit->setText(s.fatherName);
    m_casteEdit->setText(s.caste);
    m_districtEdit->setText(s.district);
    m_gpaEdit->setText(QString::number(s.gpa, 'f', 2));

    updateRecordCounterLabel();
}

/**
 * @brief Clears all text input fields in the form.
 */
void MainWindow::clearFormFields()
{
    m_nameEdit->clear();
    m_rollEdit->clear();
    m_fatherEdit->clear();
    m_casteEdit->clear();
    m_districtEdit->clear();
    m_gpaEdit->clear();

    m_nameEdit->setFocus();
}

/**
 * @brief Validates all input fields, checking for emptiness, duplicate roll number,
 *        and GPA range [0.00 - 4.00].
 */
bool MainWindow::validateFields(Student &outStudent, bool isAddingNew)
{
    QString name = m_nameEdit->text().trimmed();
    QString roll = m_rollEdit->text().trimmed();
    QString father = m_fatherEdit->text().trimmed();
    QString caste = m_casteEdit->text().trimmed();
    QString district = m_districtEdit->text().trimmed();
    QString gpaStr = m_gpaEdit->text().trimmed();

    // 1. Check for empty fields
    if (name.isEmpty() || roll.isEmpty() || father.isEmpty() ||
        caste.isEmpty() || district.isEmpty() || gpaStr.isEmpty()) {
        QMessageBox::warning(this, "Validation Warning",
                             "All fields are mandatory. Please fill in all student details.");
        return false;
    }

    // Commas are used as CSV delimiters; prevent user input from breaking the file structure
    if (name.contains(',') || roll.contains(',') || father.contains(',') ||
        caste.contains(',') || district.contains(',')) {
        QMessageBox::warning(this, "Validation Warning",
                             "Fields cannot contain commas (',') as they are used as CSV delimiters.");
        return false;
    }

    // 2. Validate GPA format and numerical range
    bool gpaOk = false;
    double gpa = gpaStr.toDouble(&gpaOk);
    if (!gpaOk || gpa < 0.0 || gpa > 4.0) {
        QMessageBox::warning(this, "Validation Warning",
                             "Invalid GPA! Please enter a numeric value between 0.00 and 4.00.");
        m_gpaEdit->setFocus();
        m_gpaEdit->selectAll();
        return false;
    }

    // 3. Roll Number Uniqueness Check (only relevant when adding a new record)
    if (isAddingNew) {
        int existingIndex = findRecordIndexByRoll(roll);
        if (existingIndex != -1) {
            QMessageBox::warning(this, "Duplicate Roll Number",
                                 QString("A record with Roll Number '%1' already exists!\n"
                                         "Roll Number must be unique for each student.")
                                     .arg(roll));
            m_rollEdit->setFocus();
            m_rollEdit->selectAll();
            return false;
        }
    }

    // Populate validated student struct
    outStudent.name = name;
    outStudent.rollNumber = roll;
    outStudent.fatherName = father;
    outStudent.caste = caste;
    outStudent.district = district;
    outStudent.gpa = gpa;

    return true;
}

/**
 * @brief Searches in-memory records for a student with the matching roll number.
 * @return Index of matching student in m_records, or -1 if not found.
 */
int MainWindow::findRecordIndexByRoll(const QString &rollNumber) const
{
    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records.at(i).rollNumber.compare(rollNumber.trimmed(), Qt::CaseInsensitive) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Updates the record count label and status bar.
 */
void MainWindow::updateRecordCounterLabel()
{
    int total = m_records.size();
    if (m_currentState == AppState::Browsing && m_currentIndex >= 0 && m_currentIndex < total) {
        m_counterLabel->setText(QString("Viewing Record %1 of %2").arg(m_currentIndex + 1).arg(total));
    } else {
        m_counterLabel->setText(QString("Total Records Saved: %1").arg(total));
    }
}

// =========================================================================
// SLOTS IMPLEMENTATION
// =========================================================================

/**
 * @brief Handles clicking the "Add Record" button.
 * Validates the inputs, appends the record to file, updates in-memory records,
 * displays a confirmation dialog, and resets the form.
 */
void MainWindow::onAddClicked()
{
    Student student;
    if (!validateFields(student, /*isAddingNew=*/true)) {
        return; // Validation failed, error message already shown
    }

    // Append record directly to the text file
    if (!appendRecordToFile(student)) {
        return; // File error encountered, error message already shown
    }

    // Update in-memory vector
    m_records.append(student);

    // Show confirmation dialog
    QMessageBox::information(this, "Success",
                             QString("Student '%1' (Roll No: %2) added successfully!")
                                 .arg(student.name, student.rollNumber));

    // Clear fields and remain in Initial/New state ready for next entry
    clearFormFields();
    m_currentIndex = -1;
    setAppState(AppState::InitialOrNew);
    updateRecordCounterLabel();
}

/**
 * @brief Handles clicking the "Next" navigation button.
 * Navigates through records sequentially.
 * Switches state to Browsing: Add button disabled, Update & Delete buttons enabled.
 */
void MainWindow::onNextClicked()
{
    if (m_records.isEmpty()) {
        QMessageBox::information(this, "No Records",
                                 "No student records found. Please add a student record first.");
        return;
    }

    // Advance to next index; loop back to start if reaching end
    m_currentIndex++;
    if (m_currentIndex >= m_records.size()) {
        m_currentIndex = 0; // Wrap around to the first record
        m_statusMessageLabel->setText("Reached end of records. Wrapped around to the first record.");
    }

    // Display student data in fields
    displayRecord(m_currentIndex);

    // Toggle button states as required:
    // Add is disabled; Update and Delete are enabled
    setAppState(AppState::Browsing);
}

/**
 * @brief Handles clicking the "Previous" navigation button.
 * Navigates backwards through records sequentially.
 */
void MainWindow::onPreviousClicked()
{
    if (m_records.isEmpty()) {
        QMessageBox::information(this, "No Records",
                                 "No student records found. Please add a student record first.");
        return;
    }

    // Move backwards; wrap around to the last record if at the beginning
    m_currentIndex--;
    if (m_currentIndex < 0) {
        m_currentIndex = m_records.size() - 1;
        m_statusMessageLabel->setText("Reached beginning. Wrapped around to the last record.");
    }

    // Display student data in fields
    displayRecord(m_currentIndex);

    // Ensure state is browsing
    setAppState(AppState::Browsing);
}

/**
 * @brief Handles clicking the "Update Record" button.
 * Modifies the currently displayed student's record using Roll Number as unique identifier.
 */
void MainWindow::onUpdateClicked()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_records.size()) {
        QMessageBox::warning(this, "Selection Error", "No student record is currently selected to update.");
        return;
    }

    // Validate updated input fields (isAddingNew = false so roll number isn't flagged as duplicate with itself)
    Student updatedStudent;
    if (!validateFields(updatedStudent, /*isAddingNew=*/false)) {
        return;
    }

    // Ensure the roll number matches the current record's unique identifier
    const QString &currentRoll = m_records.at(m_currentIndex).rollNumber;
    if (updatedStudent.rollNumber.compare(currentRoll, Qt::CaseInsensitive) != 0) {
        QMessageBox::critical(this, "Identifier Error",
                              "Roll Number cannot be changed during an update as it is the unique identifier.");
        m_rollEdit->setText(currentRoll);
        return;
    }

    // Ask for user confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Update",
        QString("Are you sure you want to update details for Roll Number: %1?").arg(currentRoll),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Update in-memory record
    m_records[m_currentIndex] = updatedStudent;

    // Rewrite all records to the file
    if (!saveAllRecordsToFile()) {
        return;
    }

    QMessageBox::information(this, "Update Successful",
                             QString("Record for Roll No: %1 updated successfully!").arg(currentRoll));

    displayRecord(m_currentIndex);
}

/**
 * @brief Handles clicking the "Delete Record" button.
 * Removes the currently displayed student's record from memory and the text file.
 */
void MainWindow::onDeleteClicked()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_records.size()) {
        QMessageBox::warning(this, "Selection Error", "No student record is currently selected to delete.");
        return;
    }

    const Student &studentToDelete = m_records.at(m_currentIndex);

    // Ask for user confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion",
        QString("Are you sure you want to permanently delete student '%1' (Roll No: %2)?")
            .arg(studentToDelete.name, studentToDelete.rollNumber),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Remove from in-memory vector
    m_records.removeAt(m_currentIndex);

    // Persist changes to file
    if (!saveAllRecordsToFile()) {
        return;
    }

    QMessageBox::information(this, "Deleted", "Student record was deleted successfully.");

    // Determine next display state
    if (m_records.isEmpty()) {
        // If all records were deleted, reset form and return to Initial State
        m_currentIndex = -1;
        clearFormFields();
        setAppState(AppState::InitialOrNew);
    } else {
        // If the deleted record was the last in the list, clamp index to the new last element
        if (m_currentIndex >= m_records.size()) {
            m_currentIndex = m_records.size() - 1;
        }
        displayRecord(m_currentIndex);
        setAppState(AppState::Browsing);
    }

    updateRecordCounterLabel();
}

/**
 * @brief Handles clicking the "New / Clear Form" button.
 * Clears the form and restores the Initial State (Add enabled, Update/Delete disabled).
 */
void MainWindow::onClearClicked()
{
    m_currentIndex = -1;
    clearFormFields();
    setAppState(AppState::InitialOrNew);
    updateRecordCounterLabel();
}
