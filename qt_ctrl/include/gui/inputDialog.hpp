/**
 * @file include/gui/inputDialog.hpp
 *
 * @brief Qt dialogs to get an array of double values or 
 *        an oriented point, and also base class of the 
 *        @ref QtCtrlSelect "controller selection dialog".
 *
 * @date November 2019  @since 0.3.2
 **/


#ifndef QTCTRL_INPUT_DIALOG
#define QTCTRL_INPUT_DIALOG

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <model/state.hpp>

/** @brief  This class defines a sub-class of dialog windows 
 **         which lower part is a horizontal button panel with 
 **         only two possibilities, to accept or to cancel.
 **
 ** @version 0.3.2
 **/
class OkCancelDialog : public QDialog {
  QVBoxLayout layout;       ///< The main layout (vertic.)
  QLabel text;              ///< The upper label
  QDialogButtonBox buttons_layout;  ///< The button layout
  int index;                ///< The index of additional widgets 
  
public:
  /** @brief  The dialog is build with a given parent and title.
   **
   ** @param  parent  the parent widget of the dialog,
   ** @param  title   the title of the dialog,
   ** @param  msg     the message displayed on the top.
   **/
  OkCancelDialog(QWidget* parent, const char* title, const char* msg);
  
  /** @brief  Asks in a dialog for a set of doubles, and returns it.
   ** 
   ** @param  parent    the parent widget of the dialog,
   ** @param  title     the dialog title,
   ** @param  msg       the dialog message,
   ** @param  nb_lines  the number of lines in the dialog,
   ** @param  nb_col    the number of columns in the dialog,
   ** @param  nameVal   the array of names displayed,
   ** @param  isGreek   the array telling whether names are in Greek,
   ** @param  val       the array of doubles to get from the user.
   ** 
   ** @pre    the arrays need to have at least nb_lines x nb_col
   **         elements, or a segmentation violation may arrise.
   ** 
   ** @return  True if the array was changed, false otherwise.
   ** @since 0.3.1 */
  static bool getDoubleArray(QWidget* parent,  const char* title,
			     const char* msg,  const int nb_lines,
			     const int nb_col, const char* nameVal[], 
			     const bool isGreek[], double val[]);

  /** @brief  Asks in a dialog for an oriented point.
   ** 
   ** @param  parent  the parent widget of the dialog,
   ** @param  title   the dialog title,
   ** @param  msg     the dialog message,
   ** @param  q       the oriented point to be set.
   ** 
   ** @return  True if the array was changed, false otherwise. 
   ** @since 0.3.1 */
  static bool getOrPtConfig(QWidget* parent, 
			    const char* title, const char* msg,
			    iSeeML::rob::OrPtConfig &q);
 
  /** @brief  Asks in a dialog for a state.
   ** 
   ** @param  parent  the parent widget of the dialog,
   ** @param  title   the dialog title,
   ** @param  msg     the dialog message,
   ** @param  q       the state to be set.
   ** 
   ** @return  True if the array was changed, false otherwise. 
   ** @since 0.3.2 */
  static bool getState(QWidget* parent, const char* title, 
		       const char* msg, State &state); 
 
protected:
  /// @brief Inserts the given widget (containing the inputs) 
  ///        in the dialog, between message and buttons.
  /// @param  widget  the widget containing the inputs.
  void insertWidget(QWidget& widget) {
    layout.insertWidget(index, &widget);
    index++; 
  }
  
};  // end of class OkCancelDialog

#endif // QTCTRL_INPUT_DIALOG
