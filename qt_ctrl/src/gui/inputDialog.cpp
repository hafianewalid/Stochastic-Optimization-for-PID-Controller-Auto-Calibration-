/**
 * @file src/gui/inputDialog.cpp
 *
 * @brief Qt dialogs to get an array of double values or 
 *        an oriented point, and also base class of the 
 *        @ref QtCtrlSelect "controller selection dialog".
 *
 * @date November 2019  @since 0.3.2
 **/


#include <stdio.h>
#include "gui/inputDialog.hpp"
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QPushButton>


/* The dialog is build with a given parent and title.
 *
 * Parameter  parent  the parent widget of the dialog,
 *            title   the title of the dialog,
 *            msg     the message displayed on the top.
 */
OkCancelDialog::OkCancelDialog(QWidget* parent, const char* title,
			       const char* msg)
  : QDialog(parent), layout(this), text( tr(msg) ), 
    buttons_layout(this), index(1) {
  
  setModal(true);                      // sets modality
  setWindowTitle( tr(title) );         // defines the title
  layout.addWidget(&text);             // inserts the upper label
  layout.addWidget(&buttons_layout);   // and the button layout
  const int nb_btn = 2, dft_btn = 1;   // number of buttons, default #
  QDialogButtonBox::StandardButton std_btn[nb_btn]  // the buttons 
    = {QDialogButtonBox::Cancel, QDialogButtonBox::Ok};
  // the array of methods associated to the buttons 
  const char* btn_slots[nb_btn]
    = {SLOT( reject() ), SLOT( accept() )};
  for(int btn_idx = 0; btn_idx < nb_btn; btn_idx++) {
    // creates each button, add it to the layout
    QPushButton* btn = buttons_layout.addButton(std_btn[btn_idx]);
    // set it to be the default or not
    btn->setDefault(btn_idx == dft_btn); 
    // connects the button to the right slot
    connect(btn, SIGNAL( clicked() ), this, btn_slots[btn_idx]);
  } // end of for (each button)

cout<<"acc \n";
accept();
cout<<"acc app\n";
} // end of OkCancelDialog::OkCancelDialog(QWidget*, ...) ------------

// Asks in a dialog for a set of doubles, and returns it.
//
// Parameter  parent    the parent widget of the dialog,
//            title     the dialog title,
//            msg       the dialog message,
//            nb_lines  the number of lines in the dialog,
//            nb_col    the number of columns in the dialog,
//            nameVal   the array of names displayed,
//            isGreek   the array indicating wether name is in Greek,
//            val       the array of doubles to get from the user.
//
// Precondition  the arrays need to have at least nb_lines x nb_col
//               elements, or a segmentation violation may arrise.
//
// Return  True if the array was changed, false otherwise.
bool OkCancelDialog::getDoubleArray
(QWidget* parent, const char* title, const char* msg, 
 const int nb_lines, const int nb_col, const char* nameVal[],
 const bool isGreek[], double val[]) {

  // modif 
   return 1;

  const int nbVal = nb_lines * nb_col; 
  OkCancelDialog dialog(parent, title, msg);   // the dialog
  QWidget values_wdgt(&dialog);                // the values widget
  QGridLayout values_layout(&values_wdgt);     // values layout (grid)
  QLineEdit valBox[nbVal];                     // its value boxes
  QDoubleValidator doubleValid(&values_wdgt);  // they take doubles
  int idx; 
  for(idx = 0; idx < nbVal; idx++) {  // fills the widget
    const int lg = idx / nb_col, col = 2 * (idx % nb_col); 
    char name[25];  // text label: emphasize if not greek
    sprintf(name, "%s%s%s", isGreek[idx] ? "<span>" : "<em>",
	    nameVal[idx], isGreek[idx] ? "</span>:" : "</em>:");
    values_layout.addWidget(new QLabel( tr(name) ), lg, col); 
    valBox[idx].setText( QString("%L1").arg(val[idx]) ); 
    valBox[idx].setValidator(&doubleValid);  // accepts only double
    values_layout.addWidget(&valBox[idx], lg, col + 1); 
  } // end of for (each value)
  dialog.insertWidget(values_wdgt);  // add the values widget
  // shows the dialog, waits until it closes
  // and check whether changes were set
  const bool res = (dialog.exec() == QDialog::Accepted);  
  for(idx = 0; res && (idx < nbVal); idx++)    // extracts the values
    val[idx] = valBox[idx].text().toDouble();  // from the boxes
  
  cout<<" je suit la 1  ["<<res<< "\n" ; 
  return res;

} // end of void OkCancelDialog::getDoubleArray(...) -----------------

// Asks in a dialog for an oriented point.
// 
// Parameter  parent  the parent widget of the dialog,
//            title   the dialog title,
//            msg     the dialog message,
//            q       the oriented point to be set.
// 
// Return  True if the array was changed, false otherwise. 
bool OkCancelDialog::getOrPtConfig(QWidget* parent, 
				   const char* title, const char* msg,
				   iSeeML::rob::OrPtConfig &q) {

  // modif 
   return 1;

  static const int nb_val = 3;  // config. -> 3 values
  static const char* name_val[nb_val]
    = {"x", "y", "&theta;"};
  static const bool val_Greek[nb_val]
    = {false, false, true};
  std::string ext_msg(msg);
  ext_msg += " (angle is in degres):";
  double val[nb_val] = {0, 0, 0}; 
  const bool res = getDoubleArray(parent, title, ext_msg.c_str(), 1, 
				  nb_val, name_val, val_Greek, val);
  if (res) {
    val[2] = iSeeML::Object::deg2rad(val[2]); 
    q = iSeeML::rob::OrPtConfig(iSeeML::geom::Point(val[0], val[1]),
				val[2]);
  }
  cout<<" je suit la 2  ["<<res<< "\n" ; 
  return res;
} // end of OkCancelDialog::getOrPtConfig(...) -----------------------

// Asks in a dialog for an state.
// 
// Parameter  parent  the parent widget of the dialog,
//            title   the dialog title,
//            msg     the dialog message,
//            state   the state to be set.
// 
// Return  True if the array was changed, false otherwise. 
bool OkCancelDialog::getState(QWidget* parent, const char* title, 
			      const char* msg, State &state) {
  
  //cout<<" je suit la 3 d  \n" ; 
  static const int nb_val = 6;  // state -> 6 values
  static const char* name_val[nb_val]
    = {"t", "x", "y", "&theta;", "v", "&omega"};
  static const bool val_Greek[nb_val]
    = {false, false, false, true, false, true};
  std::string ext_msg(msg);
  ext_msg += " (angle are in degres):";

  // modif goad pos 
  double val[nb_val] = {0, 10, 10, 0, 0, 0}; 

  const bool res = getDoubleArray(parent, title, ext_msg.c_str(), 2, 
				  nb_val / 2, name_val, val_Greek, val);
  if (res) {
    val[3] = iSeeML::Object::deg2rad(val[3]); 
    val[5] = iSeeML::Object::deg2rad(val[5]);
    state  = State( val[0], iSeeML::rob::OrPtConfig
		    (iSeeML::geom::Point(val[1], val[2]), val[3]),
		    val[4], val[5] );
  }
  //cout<<" je suit la 3  ["<<res<<"\n" ; 
  return res;
} // end of OkCancelDialog::getState(...) ----------------------------
