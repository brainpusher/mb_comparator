#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subsequence.h"

#include <QFile>
#include <QTextStream>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      default_font_(QFont("Ubuntu Mono", 12)),
      ui_(new Ui::MainWindow),
      font_dialog_(new QFontDialog(this)),
      left_file_dialog_(new QFileDialog(this)),
      right_file_dialog_(new QFileDialog(this)) {
  // Setup user interface
  ui_->setupUi(this);
  ui_->mainLayout->setMargin(10);
  ui_->centralWidget->setLayout(ui_->mainLayout);
  // Move main window to center
  QRect rect = frameGeometry();
  rect.moveCenter(QDesktopWidget().availableGeometry().center());
  move(rect.topLeft());
  // Setup default font
  SetDefautFont();
  // Connect signals to slots
  connect(font_dialog_, SIGNAL(fontSelected(QFont)),
          this, SLOT(ChangeFont(QFont)));
  connect(left_file_dialog_, SIGNAL(fileSelected(QString)),
          this, SLOT(OpenLeftFile(QString)));
  connect(right_file_dialog_, SIGNAL(fileSelected(QString)),
          this, SLOT(OpenRightFile(QString)));
}

MainWindow::~MainWindow() {
  delete ui_;
  delete font_dialog_;
  delete left_file_dialog_;
  delete right_file_dialog_;
}

void MainWindow::ChangeFont(const QFont& font) {
  ui_->leftText->setFont(font);
  ui_->rightText->setFont(font);
}

void MainWindow::on_actionChangeFont_activated() {
  font_dialog_->show();
}

void MainWindow::on_actionDefaultFont_activated() {
  SetDefautFont();
}

void MainWindow::OpenLeftFile(const QString& file_name) {
  ui_->leftFileName->setText(file_name);
  if (0 != ui_->rightFileName->text().size()) {
    ui_->compareButton->setEnabled(true);
  }
}

void MainWindow::OpenRightFile(const QString& file_name) {
  ui_->rightFileName->setText(file_name);
  if (0 != ui_->leftFileName->text().size()) {
    ui_->compareButton->setEnabled(true);
  }
}

void MainWindow::on_leftOpenFileButton_clicked(){
  left_file_dialog_->show();
}

void MainWindow::on_rightOpenFileButton_clicked() {
  right_file_dialog_->show();
}

void MainWindow::SetDefautFont() {
  font_dialog_->setCurrentFont(default_font_);
  ui_->leftText->setFont(default_font_);
  ui_->rightText->setFont(default_font_);
}

void MainWindow::on_compareButton_clicked() {
  QVector<QString> left_text = ReadFile(ui_->leftFileName->text());
  int left_index = 0;
  QVector<QString> right_text = ReadFile(ui_->rightFileName->text());
  int right_index = 0;
  QVector<Actions> compare_result =
      Subsequence::getLongestCommonSubsequence(left_text, right_text);
  ui_->leftText->clear();
  ui_->rightText->clear();
  for (int i = 0; i < compare_result.size(); ++i) {
    switch (compare_result[i]) {
      case none:
        ui_->leftText->insertPlainText(left_text[left_index++] + "\n");
        ui_->rightText->insertPlainText(right_text[right_index++] + "\n");
      break;
      case away:
        ui_->leftText->insertPlainText(left_text[left_index++] + "\n");
        ui_->rightText->insertPlainText("----\n");
      break;
      case insert:
        ui_->leftText->insertPlainText("++++\n");
        ui_->rightText->insertPlainText(right_text[right_index++] + "\n");
      break;
    }
  }
}

QVector<QString> MainWindow::ReadFile(const QString& file_name) {
  QFile file(file_name);
  file.open(QFile::ReadOnly);
  QTextStream file_stream(&file);
  QVector<QString> result;
  while (!file_stream.atEnd()) {
    result.push_back(file_stream.readLine());
  }
  return result;
}
