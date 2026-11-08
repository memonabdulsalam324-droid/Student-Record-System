#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QString>
#include "student.h"

// Forward declaration of Qt classes
class QGroupBox;
class QFormLayout;

/**
 * @brief Enum defining the interactive states of the application.
 */
enum class AppState {
    InitialOrNew, // Initial state or adding new record: Add is enabled, Update/Delete are disabled
    Browsing      // Browsing existing records: Add is disabled, Update/Delete are enabled
};

/**
 * @class MainWindow
 * @brief Main application window for the Student Record Management System.
 *
 * Implements a complete GUI form using Qt Widgets, sequential file I/O (CSV format),
 * in-memory vector caching, input validation, and a state machine for button toggling.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Slot handlers for user interactions
    void onAddClicked();
    void onNextClicked();
    void onPreviousClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onClearClicked();

private:
    // UI Setup & Styling
    void setupUi();
    void setupStyles();

    // State machine management
    void setAppState(AppState state);

    // File I/O operations
    bool loadRecordsFromFile();
    bool saveAllRecordsToFile();
    bool appendRecordToFile(const Student &student);

    // Helper functions
    void displayRecord(int index);
    void clearFormFields();
    bool validateFields(Student &outStudent, bool isAddingNew);
    int findRecordIndexByRoll(const QString &rollNumber) const;
    void updateRecordCounterLabel();

    // UI Widgets - Input LineEdits
    QLineEdit *m_nameEdit;
    QLineEdit *m_rollEdit;
    QLineEdit *m_fatherEdit;
    QLineEdit *m_casteEdit;
    QLineEdit *m_districtEdit;
    QLineEdit *m_gpaEdit;

    // UI Widgets - Action Buttons
    QPushButton *m_btnAdd;
    QPushButton *m_btnNext;
    QPushButton *m_btnPrevious;
    QPushButton *m_btnUpdate;
    QPushButton *m_btnDelete;
    QPushButton *m_btnClear;

    // UI Widgets - Status & Information
    QLabel *m_counterLabel;
    QLabel *m_statusMessageLabel;

    // Data Storage & State
    QVector<Student> m_records;    // In-memory list of student records
    int m_currentIndex;           // Index of currently displayed student (-1 if none)
    QString m_filePath;           // File path for student_records.txt
    AppState m_currentState;      // Current application state
};

#endif // MAINWINDOW_H
