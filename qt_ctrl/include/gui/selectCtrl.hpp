/**
 * @file include/gui/selectCtrl.hpp
 *
 * @brief Qt dialog window to select the control used 
 *        by the @ref QtCtrlGUI "qt_ctrl GUI".
 *
 * @date October 2019
 **/

#ifndef QTCTRL_CTRL_SELECT
#define QTCTRL_CTRL_SELECT

#include <gui/ctrlWdgt.hpp>
#include <gui/inputDialog.hpp>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>

/** @brief  This class defines @ref OkCancelDialog 
 **         "an accept/cancel dialog" to create a controller 
 **         and its display widget which will be used 
 **         by the @ref QtCtrlGUI "qt_ctrl GUI".
 **
 ** Both controller and display widget are allocated dynamically
 ** (they will be used after this dialog has been freed), they have
 ** to be deleted by the @ref QtCtrlGUI "qt_ctrl GUI".
 **
 ** @version 0.3.2
 **/
class QtCtrlSelect : public OkCancelDialog {
  Q_OBJECT // This macro is needed to handle graphical events

  /// @brief  The number of ROS environments and sub-environments.
  static const int ROS_choices = 2;
  
  /// @brief  The ROS workspace group box (may be desabled).
  QGroupBox ROS_workspace;
  
  /// @brief  The radio buttons to select ROS environment. 
  QRadioButton  select_ROS[ROS_choices][ROS_choices];

  /// @brief  The pop-up menu with the list of possible controllers.
  QComboBox ctrl_select;

  /// @brief  The identification number of the selected controller.
  int ctrl_id; 

  /// @brief  The button to set the controller's parameters.
  QPushButton ctrl_param; 

  /// @brief  The name displayed when no save file is selected.
  static const char *default_save_name;

  /// @brief  The save file name.
  QString save_file_name; 

  /// @brief  The button displaying and changing save file.
  QPushButton save_file; 

  /// @brief The controller's time step. 
  const double time_step; 

  /// @brief The motion model of the controlled robot.  
  const MotionModel&  motion_model;

  /// @brief The controller created by this dialog.
  Controller* controller;

  /// @brief The widget showing the controller's velocities. 
  ControlWidget*  ctrl_wdgt;
	
public:
  /** @brief  The dialog is build with a given parent and a time step 
   **         for the future controller.
   **
   ** @param  parent  the parent widget of the dialog,
   ** @param  tm_stp  the future controller's time step,
   ** @param  motion  the controller motion model.
   **/
  QtCtrlSelect(QWidget* parent, const double tm_stp,
	       const MotionModel& motion);

  
  /** @brief   Indicates whether ROS should be started.
   ** @return  True if ROS should be started, 
   **          false if it already runs. */
  bool startROS() const { return select_ROS[0][1].isChecked(); }
 
  /** @brief   Indicates if starting ROS should use empty workspace.
   ** @return  True if ROS should be start with empty workspace, 
   **          false if it should be start with default one. */
  bool emptyROSworkspace() const
  { return select_ROS[1][0].isChecked(); }
   
  /** @brief   Gives the selected controller, or null pointer 
   **          if the dialog was cancelled.
   ** @return  A pointer on the build controller. */
  Controller* newController()  { return controller; }
 
  /** @brief   Gives the selected controller's widget, or 
   **          null pointer if the dialog was cancelled.
   ** @return  A pointer on the build controller's widget. */
  ControlWidget* newCtrlWidget()  { return ctrl_wdgt; }
  
  /// @brief  Gives the save file name.  @return  The save file name.
  const QString& saveFileName() const { return save_file_name; } 

private Q_SLOTS:
  /// @brief  Enables or disables the ROS workspace group box.
  void updateROSline() { ROS_workspace.setEnabled( startROS() ); }
  
  /// @brief  Changes the selected controller.
  /// @param  new_id  the new controller number, in the selection list.
  void updateCtrl(int new_id);  // Param. default value does not work.
    
  /// @brief  Updates the parameters of the current controller.
  /// @see updateCtrl(int)
  void updateCtrl() { updateCtrl(ctrl_id); }
    
  /// @brief  Updates the button displaying the save file.
  void updateFileName();
};  // end of class QtCtrlSelect

#endif // QTCTRL_CTRL_SELECT
