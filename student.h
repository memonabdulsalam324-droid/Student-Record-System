#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QStringList>

/**
 * @brief Represents a single student record in the Student Record Management System.
 */
struct Student {
    QString name;
    QString rollNumber;
    QString fatherName;
    QString caste;
    QString district;
    double gpa = 0.0;

    /**
     * @brief Converts the student record into a single comma-separated value (CSV) line.
     * @return Formatted CSV string.
     */
    QString toCsv() const {
        // Format: Name,RollNumber,FatherName,Caste,District,GPA
        return QString("%1,%2,%3,%4,%5,%6")
            .arg(name.trimmed())
            .arg(rollNumber.trimmed())
            .arg(fatherName.trimmed())
            .arg(caste.trimmed())
            .arg(district.trimmed())
            .arg(QString::number(gpa, 'f', 2));
    }

    /**
     * @brief Parses a CSV line into a Student instance.
     * @param line CSV line from file.
     * @param ok Optional pointer to boolean flag reporting parsing success.
     * @return Populated Student instance, or empty on failure.
     */
    static Student fromCsv(const QString &line, bool *ok = nullptr) {
        Student s;
        if (ok) *ok = false;

        // Split by comma
        QStringList tokens = line.split(',');
        if (tokens.size() < 6) {
            return s; // Invalid CSV format
        }

        s.name = tokens[0].trimmed();
        s.rollNumber = tokens[1].trimmed();
        s.fatherName = tokens[2].trimmed();
        s.caste = tokens[3].trimmed();
        s.district = tokens[4].trimmed();

        bool gpaOk = false;
        s.gpa = tokens[5].trimmed().toDouble(&gpaOk);
        if (!gpaOk) {
            return s;
        }

        if (ok) *ok = true;
        return s;
    }
};

#endif // STUDENT_H
