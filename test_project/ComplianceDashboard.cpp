#include "ComplianceDashboard.hpp"
#include "dataset.hpp"
#include "WaterSample.hpp"

ComplianceDashboard::ComplianceDashboard(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    populateTable("Y-2024.csv");
}

ComplianceDashboard::~ComplianceDashboard() {}

void ComplianceDashboard::setupUI() {
    // Central Widget
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout();

    // Header
    header = new QLabel("Compliance Dashboard");
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(header);

    // Summary Cards
    cardsLayout = new QHBoxLayout();
    for (int i = 0; i < 4; ++i) {
        summaryCards[i] = new QFrame();
        summaryCards[i]->setFrameShape(QFrame::StyledPanel);
        summaryCards[i]->setStyleSheet("background-color: #f2f2f2; border: 1px solid #d9d9d9; padding: 40px;"); // Increased padding
        summaryCards[i]->setMinimumHeight(200); // Doubled height
        summaryCards[i]->setMinimumWidth(400);  // Doubled width
        QVBoxLayout *cardLayout = new QVBoxLayout();
        QLabel *cardTitle = new QLabel(QString("Summary Card %1").arg(i + 1));
        cardTitle->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(cardTitle);
        summaryCards[i]->setLayout(cardLayout);
        cardsLayout->addWidget(summaryCards[i]);
    }
    mainLayout->addLayout(cardsLayout);

    // Filters Section
    filtersLayout = new QHBoxLayout();
    yearFilter = new QComboBox();
    yearFilter->addItems({"All Years", "2020", "2021", "2022", "2023", "2024"});
    locationFilter = new QComboBox();
    locationFilter->addItems({"All Locations", "London", "Manchester", "Yorkshire"});
    pollutantFilter = new QComboBox();
    pollutantFilter->addItems({"All Pollutants", "Ammonia", "Lead", "Zinc"});
    statusFilter = new QComboBox();
    statusFilter->addItems({"All Statuses", "Compliant", "Non-Compliant"});
    filterButton = new QPushButton("Filter");

    filtersLayout->addWidget(yearFilter);
    filtersLayout->addWidget(locationFilter);
    filtersLayout->addWidget(pollutantFilter);
    filtersLayout->addWidget(statusFilter);
    filtersLayout->addWidget(filterButton);
    mainLayout->addLayout(filtersLayout);

    // Main Content Area
    contentLayout = new QHBoxLayout();

    // Detailed Table
    detailedTable = new QTableWidget(0, 5); // Start with 0 rows, 5 columns
    detailedTable->setHorizontalHeaderLabels({"Location", "Pollutant", "Level", "Unit", "Compliance"});
    detailedTable->setMinimumSize(600, 300); // Keep table smaller
    contentLayout->addWidget(detailedTable, 2);

    // Summary of Important Info
    importantInfo = new QTextEdit();
    importantInfo->setPlaceholderText("Summary of important information...");
    importantInfo->setReadOnly(true);
    importantInfo->setMinimumSize(200, 300);
    contentLayout->addWidget(importantInfo, 1);

    mainLayout->addLayout(contentLayout);

    // Footer
    footer = new QLabel("Data provided by UK Environmental Agency.");
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("font-size: 12px; color: gray;");
    mainLayout->addWidget(footer);

    // Set Layout
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    resize(1200, 800); // Adjust window size to fit larger cards

    // Connect filter button to applyFilters
    connect(filterButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilters);
}

void ComplianceDashboard::populateTable(const std::string& filename) {
    WaterDataset dataset;
    dataset.loadData(filename);

    const auto& samples = dataset.getData();
    detailedTable->setRowCount(samples.size());
    detailedTable->setColumnCount(5); // Number of columns
    detailedTable->setHorizontalHeaderLabels({"Location", "Pollutant", "Level", "Unit", "Compliance"});

    for (size_t i = 0; i < samples.size(); ++i) {
        const auto& sample = samples[i];
        detailedTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sample.getLocation())));
        detailedTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sample.getPollutant())));
        detailedTable->setItem(i, 2, new QTableWidgetItem(QString::number(sample.getLevel())));
        detailedTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(sample.getUnit())));
        detailedTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(sample.getComplianceStatus())));
    }
}

void ComplianceDashboard::applyFilters() {
    // Retrieve filter criteria
    QString selectedYear = yearFilter->currentText();
    QString selectedLocation = locationFilter->currentText();
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedStatus = statusFilter->currentText();

    // Load the dataset
    WaterDataset dataset;
    dataset.loadData("Y-2024.csv");

    const auto& samples = dataset.getData();
    detailedTable->setRowCount(0); // Clear the table for filtered results

    // Iterate through samples and apply filters
    int row = 0;
    for (const auto& sample : samples) {
        // Apply Year Filter (if applicable)
        if (selectedYear != "All Years" && sample.getYear() != std::stoi(selectedYear.toStdString())) continue;

        // Apply Location Filter
        if (selectedLocation != "All Locations" && sample.getLocation() != selectedLocation.toStdString()) continue;

        // Apply Pollutant Filter
        if (selectedPollutant != "All Pollutants" && sample.getPollutant() != selectedPollutant.toStdString()) continue;

        // Apply Compliance Status Filter
        if (selectedStatus != "All Statuses" && sample.getComplianceStatus() != selectedStatus.toStdString()) continue;

        // Add the matching row to the table
        detailedTable->insertRow(row);
        detailedTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(sample.getLocation())));
        detailedTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(sample.getPollutant())));
        detailedTable->setItem(row, 2, new QTableWidgetItem(QString::number(sample.getLevel())));
        detailedTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(sample.getUnit())));
        detailedTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(sample.getComplianceStatus())));
        row++;
    }
}
