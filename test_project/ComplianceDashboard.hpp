#ifndef COMPLIANCEDASHBOARD_HPP
#define COMPLIANCEDASHBOARD_HPP

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>

class ComplianceDashboard : public QMainWindow {
    Q_OBJECT

public:
    ComplianceDashboard(QWidget *parent = nullptr);
    ~ComplianceDashboard();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Header
    QLabel *header;

    // Summary Cards
    QHBoxLayout *cardsLayout;
    QFrame *summaryCards[4];

    // Filters Section
    QHBoxLayout *filtersLayout;
    QComboBox *yearFilter;
    QComboBox *locationFilter;
    QComboBox *pollutantFilter;
    QComboBox *statusFilter;
    QPushButton *filterButton;

    // Main Content
    QHBoxLayout *contentLayout;
    QTableWidget *detailedTable;
    QTextEdit *importantInfo;

    // Footer
    QLabel *footer;

    void setupUI();
};

#endif // COMPLIANCEDASHBOARD_HPP
