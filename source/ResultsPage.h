////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Strata.
//
// Strata is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Strata is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Strata.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2010-2018 Albert Kottke
//
////////////////////////////////////////////////////////////////////////////////

#ifndef RESULTS_PAGE_H_
#define RESULTS_PAGE_H_

#include "AbstractPage.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QPrinter>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>

#include <QwtLegend>
#include <QwtPlot>
#include <QwtPlotCurve>

class AbstractOutput;
class MyTableView;
class OutputCatalog;

//! Widget for the Results page.

class ResultsPage : public AbstractPage {
  Q_OBJECT

public:
  explicit ResultsPage(QWidget *parent = nullptr);

  virtual void setModel(SiteResponseModel *model);

public slots:
  void exportData();
  void print(QPrinter *printer);

protected slots:
  //! Show the context menu
  void showPlotContextMenu(const QPoint &point);

  //! Copy the plot to the clipboard
  void copyPlot();

  //! Update the plot when a specific output is selected
  void setSelectedSeries(const QModelIndex &current,
                         const QModelIndex &previous = QModelIndex());

  //! Update the data when a data series is selected
  void selectedDataChanged(const QModelIndex &current,
                           const QModelIndex &previous);

  void setSelectedOutput(int index);

  void pointSelected(const QPoint &point);

  void enableSelectedCurve(int index);

  void colorCurve(int index);
  void uncolorCurve(int index);

  void configurePlot();

  void setMotionEnabled(bool enabled);
  void setSiteEnabled(bool enabled);

  void recomputeStats();

  void setStatsNeedUpdate();

private:
  //! Create the table goup box
  auto createOutputGroup() -> QGroupBox *;

  //! Create the plot
  auto createDataTabWidget() -> QTabWidget *;

  //! Create context menu for the chart
  void createContextMenu();

  QComboBox *_outputComboBox;
  QTableView *_catalogTableView;

  QPushButton *_enableMotionPushButton;
  QPushButton *_enableSitePushButton;
  QPushButton *_recomputePushButton;
  QPushButton *_viewDataPushButton;

  QwtPlot *_plot;
  QList<QwtPlotCurve *> _curves;

  QMenu *_plotContextMenu;

  MyTableView *_outputTableView;

  //! Catalog containing all of the output
  OutputCatalog *_outputCatalog;

  //! Current selected output
  AbstractOutput *_selectedOutput;

  //! Currently selected row
  int _selectedRow;

  //! If the statistics need to be re-computed
  bool _statsNeedUpdate;
};
#endif
