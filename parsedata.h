#ifndef PARSEDATA_H
#define PARSEDATA_H

#include "subsequence.h"

#include <QObject>

class ParseData : public QObject
{
  Q_OBJECT
public:
  explicit ParseData(QObject *parent = 0);

signals:

public slots:
  void setLeftText(const QVector<QString>& text);
  void setRightText(const QVector<QString>& text);
  void setCompareResult(const QVector<Actions>& comp_res);
  QVector<QString> Parsing();

private:
  QVector<QString> left_text_;
  QVector<QString> right_text_;
  QVector<Actions> compare_result_;
};

#endif // PARSEDATA_H
