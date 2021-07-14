/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#ifndef AVOGADRO_QTGUI_JSONWIDGET_H
#define AVOGADRO_QTGUI_JSONWIDGET_H

#include "avogadroqtguiexport.h"

#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QWidget>

class QJsonValue;
class QTextEdit;
class QWidget;

namespace Avogadro {
namespace QtGui {
class Molecule;

/**
 * @class JsonWidget jsonwidget.h
 * <avogadro/qtgui/jsonwidget.h>
 * @brief The JsonWidget class provides a user interface for
 * running external scripts
 * @sa InterfaceWidget
 *
 * The JsonWidget creates a GUI to represent the options given by an
 * script, turning JSON from the script into a form and passing the results
 * back to the script via command-line.
 *
 * It's used by a range of different scripts, including:
 * - Commands
 * - InterfaceGenerators
 */
class AVOGADROQTGUI_EXPORT JsonWidget : public QWidget
{
  Q_OBJECT

public:
  /**
   * Construct a widget that dynamically generates a GUI
   */
  explicit JsonWidget(QWidget* parent_ = nullptr);
  ~JsonWidget() override;

  /**
   * Set the molecule used in the simulation.
   */
  virtual void setMolecule(QtGui::Molecule* mol);

  /**
   * Collect all of the user-specified options into a JSON object, to be sent
   * to the generator script.
   */
  QJsonObject collectOptions() const;

  /**
   * Apply the options in the passed QJsonObject to the GUI. Any widgets changed
   * by this method will have their signals blocked while modifying their
   * values.
   */
  void applyOptions(const QJsonObject& opts);

  bool isEmpty() const { return m_empty; }

protected:
  /**
   * Given the name of a user-option in m_options, return the type string.
   * If an error occurs, an empty string will be returned.
   */
  QString lookupOptionType(const QString& name) const;

  /**
   * Used to construct the script-specific GUI.
   * @{
   */
  virtual void updateOptions();
  void buildOptionGui();
  void combinedOptionRow(const QString& label1, const QString& label2,
                         const QString& tr1, const QString& tr2,
                         QJsonObject& options);
  void addOptionRow(const QString& label, const QJsonValue& option);

  QWidget* createOptionWidget(const QJsonValue& option);
  QWidget* createStringListWidget(const QJsonObject& obj);
  QWidget* createStringWidget(const QJsonObject& obj);
  QWidget* createFilePathWidget(const QJsonObject& obj);
  QWidget* createIntegerWidget(const QJsonObject& obj);
  QWidget* createFloatWidget(const QJsonObject& obj);
  QWidget* createBooleanWidget(const QJsonObject& obj);
  /**@}*/

  /**
   * Set the simulation settings to their default values.
   * @{
   */
  void setOptionDefaults();
  void setOption(const QString& name, const QJsonValue& defaultValue);
  void setStringListOption(const QString& name, const QJsonValue& value);
  void setStringOption(const QString& name, const QJsonValue& value);
  void setFilePathOption(const QString& name, const QJsonValue& value);
  void setIntegerOption(const QString& name, const QJsonValue& value);
  void setFloatOption(const QString& name, const QJsonValue& value);
  void setBooleanOption(const QString& name, const QJsonValue& value);
  /**@}*/

  /**
   * @brief Search for an option named @a option and convert its value to a
   * string.
   * @param option The name of the option.
   * @param value String to overwrite with option value.
   * @return True if value is overwritten, false if the option is not found or
   * cannot be converted to a string.
   */
  bool optionString(const QString& option, QString& value) const;

  /**
   * Update the autogenerated job title in the GUI.
   */
  QString generateJobTitle() const;

  QtGui::Molecule* m_molecule;
  QJsonObject m_options;
  QJsonObject m_optionCache; // For reverting changes
  QList<QTextEdit*> m_dirtyTextEdits;

  bool m_empty;
  bool m_batchMode;
  QFormLayout* m_currentLayout;
  QWidget* m_centralWidget;
  QMap<QString, QWidget*> m_widgets;
  QMap<QString, QTextEdit*> m_textEdits;
};

} // namespace QtGui
} // namespace Avogadro

#endif // AVOGADRO_QTGUI_JSONWIDGET_H
