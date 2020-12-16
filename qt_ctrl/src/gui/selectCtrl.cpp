/**
 * @file src/gui/selectCtrl.cpp
 *
 * @brief Qt dialog window to select the control used 
 *        by the @ref QtCtrlGUI "qt_ctrl GUI".
 *
 * @date October 2019
 **/


#include <gui/selectCtrl.hpp>
#include <gui/window.hpp>
#include <ctrl/file.hpp>
#include <ctrl/smoothPath.hpp>
#include <ctrl/goPID.hpp>
#include <ctrl/imitate.hpp>
#include <ctrl/PID.hpp>
#include <ctrl/analytic.hpp>
#include <gui/teleop.hpp>
#include <QMessageBox>
//#include <iostream>

//modif
double coef[6];
void pid_param()
{
    using namespace std;

    ifstream file("param.txt");
    if(file.is_open())
    {
        string s;
        for(int i = 0; i < 6; ++i)
        {
            file >>s;
            coef[i]=std::stod(s);
        }
    }

}


// The name displayed when no save file is selected.
// modif
//const char *QtCtrlSelect::default_save_name = "None";
const char *QtCtrlSelect::default_save_name = "res";

/* The dialog is build with a given parent and a time step 
 * for the future controller.
 *
 * Parameter  parent  the parent widget of the dialog,
 *            tm_stp  the future controller's time step,
 *            motion  the controller motion model.
 */
QtCtrlSelect::QtCtrlSelect(QWidget* parent, const double tm_stp,
			   const MotionModel& motion)
  : OkCancelDialog(parent, "ROS qt_ctrl Controller Selector",
		   "Which qt_ctrl controller do you want\n"
		   "to start, and in which ROS environment?"),
    ROS_workspace( tr("ROS workspace") ), ctrl_id(2),//-1
    save_file( tr(default_save_name) ), time_step(tm_stp),
    motion_model(motion), controller(NULL), ctrl_wdgt(NULL) {
 
  int index, idx2;

  QWidget *ROS_line = new QWidget(); 
  QHBoxLayout *ROS_layout = new QHBoxLayout(ROS_line); 
  // the center part of the dialog contains several group boxes:
  // one for the ROS connection (containing a second one for the ROS
  //  workspace), one for the controller selection and one for output
  // file.
  QGroupBox
    *ROS_connect = new QGroupBox(tr("Connection to ROS"), ROS_line),
    *ctrl_line   = new QGroupBox(tr("Controller Selection"), this),
    *output_line = new QGroupBox(tr("Output File"), this);
  
  // === ROS environment selection ===================================
  QBoxLayout *ROS_layouts[ROS_choices];  // one layout for each group
  QWidget *ROS_widget[ROS_choices]       // the associated widget
    = {ROS_connect, &ROS_workspace};
  // Text of the ROS radio buttons & associated shortcuts
  static const char *ROS_select[ROS_choices][ROS_choices]
    = { {"ROS &already started", "&Start ROS with Gazebo"},
	{"&Empty workspace", "&Default workspace"} },
    ROS_shortcut[ROS_choices][ROS_choices]
      = { {'A', 'S'}, {'E', 'D'} }; 
  
  for(index = 0; index < ROS_choices; index++) {
    ROS_layout->addWidget(ROS_widget[index]); // add group box to line
    ROS_layouts[index]  // defining the layout
      = new QBoxLayout(QBoxLayout::LeftToRight, ROS_widget[index]);
    for(idx2 = 0; idx2 < ROS_choices; idx2++) {
      select_ROS[index][idx2].setText( tr(ROS_select[index][idx2]) );
      select_ROS[index][idx2].setShortcut(ROS_shortcut[index][idx2]);
      // changing the first two enables or disables the last two
      if (index == 0)
	connect( &select_ROS[index][idx2], SIGNAL( clicked() ),
		 this, SLOT( updateROSline() ) ); 
      select_ROS[index][idx2].setChecked(index + idx2 == 1);
      ROS_layouts[index]->addWidget( &(select_ROS[index][idx2]) ); 
    } } // end of for (all ROS buttons)
  insertWidget(*ROS_line); 
  // === controller selection ========================================
   
  QHBoxLayout *ctrl_layout = new QHBoxLayout(ctrl_line);  
  static const int ctrl_nb = 7; 
  static const char *ctrl_names[ctrl_nb]
    = { "Keyboard Operating Controller", "File Operating Controller",
	"PID Reaching Controller", "Analytic Reaching Controller",
	"Copy Tracking Controller", "PID Tracking Controller",
	"(Analytic Tracking Controller - not yet working)" };
  ctrl_select.setParent(ctrl_line);
  // fills the pop-up menu with the list of possible controllers
  for(index = 0; index < ctrl_nb; index++)
    ctrl_select.addItem( tr(ctrl_names[index]) );
  // connects the pop-up selection to the related method
  connect( &ctrl_select, SIGNAL( activated(int) ),
	   this, SLOT( updateCtrl(int) ) );
  // connects the controller parameters button to the method
  connect( &ctrl_param, SIGNAL( clicked() ),
	   this, SLOT( updateCtrl() ) );
  // adds the pop-up menu and its associated button to the ctrl line
  ctrl_layout->addWidget(&ctrl_select); 
  
  // modif select PID  0 -> 3(anal) , 2(PID)
  updateCtrl(2);             // set the label text
  
  ctrl_layout->addWidget(&ctrl_param); 
  insertWidget(*ctrl_line);  // adds the ctrl line to the dialog
  // === output file =================================================
  QHBoxLayout *output_layout = new QHBoxLayout(output_line);
  output_layout->addWidget( new QLabel( tr("Robot's motion"
					   " save file:") ) ); 
  
  output_layout->addWidget(&save_file);
  connect( &save_file, SIGNAL( clicked() ),
	   this, SLOT( updateFileName() ) );
  insertWidget(*output_line);

} // end of QtCtrlSelect::QtCtrlSelect(QWidget*) ---------------------

// Changes the selected controller.
// Parameter  new_id  the new controller number, in the selection list.
void QtCtrlSelect::updateCtrl(int new_id) {
  const bool tracking = new_id > 3; 
  QString input_file;         // for file and tracking controllers
  State goal;                 // for reaching controllers
  iSeeML::rob::OrPtConfig q;  // for tracking controllers
  static const int nb_coef = 6;  // for PID controllers...
  static const double h_coef = 1 / 0.35; // from [DP96]
  const double time_coef = 1 / time_step;


  //modif param PID
  /*
  double coef[nb_coef] =
    { h_coef * h_coef, h_coef, 0,
      time_coef * time_coef, time_coef, 0 }; */
   /* 
   double coef[nb_coef] =
    { 1,2, 3,
      4,5, 6 };*/
   pid_param();
   for(int i=0 ;i<6;i++){
    cout<<" // "<<coef[i];
   }
    

  // only the first controller do not need additional data
  bool valid_change = (new_id == 0);
  // an input file is needed for file operation or tracking
  if ( (new_id == 1) || tracking ) {
    const QString input_type = QString( tr("%1 File") )
      .arg(new_id == 1 ? "Accelerations" : "Trajectory");
    input_file = QFileDialog::getOpenFileName
      ( this, input_type, "", tr("CSV File (*.csv *.txt)") );
    valid_change = (! input_file.isNull() )
      && (! input_file.isEmpty() );
  } // end of if (input file needed)
  if ( (new_id > 1) && (new_id < 4) )  // reaching -> state needed 
    valid_change = getState
      (this, "State to reach",
       "Enter the goal for the controller to reach", goal);
  if (tracking && valid_change)        // configuration needed
    valid_change = getOrPtConfig(this, "Initial error setting", 
				 "Set the initial error", q);
  // PID parameters are needed for PID controllers
  if ( ( (new_id == 2) || (new_id == 5) ) && valid_change ) {
    static const char* name_coef[nb_coef]
      = { "Prop_trans", "Integ_trans", "Deriv_trans",
	  "Prop_rot", "Integ_rot", "Deriv_rot" };
    static const bool coef_Greek[nb_coef]
      = {false, false, false, false, false, false};
    valid_change = getDoubleArray
      (this, "PID Parameters Setting", "Enter the PID parameters:",
       2, nb_coef / 2, name_coef, coef_Greek, coef);
  } // end of if (PID parameters needed)
  if (valid_change) {
    // updates the parameters button's state, if needed
    if (ctrl_id != new_id) {
      ctrl_id = new_id; 
      ctrl_param.setText( tr("Controller #%1's parameters")
				.arg(ctrl_id) );
      ctrl_param.setEnabled(ctrl_id != 0);
    } // end of if (update of the parameters button's state needed)
    // gets the parent as a QtCtrlGUI
    QtCtrlGUI* ctrl_gui = qobject_cast<QtCtrlGUI*>( parent() );
    // this string needs to remain allocated?
    const std::string file_sname = input_file.toStdString();
    // transforms the input file name from a QString to a char*
    const char *file_name = file_sname.c_str();
    //std::cout << "Input File: '" << file_name << "'\n";
    if ( (*file_name != '\0') && (*file_name != '/') ) 
      QMessageBox::warning( this, tr("Abnormal Input File Name"),
			    tr("Input file name '%1' seems invalid")
			    .arg(file_name) );
    switch (ctrl_id) {  // defining the new controller & widget
    case 0: { // keyboard controller & associated widget
      KbdCtrl* new_ctrl = new KbdCtrl(motion_model, time_step);
      controller = new_ctrl; 
      ctrl_wdgt  = new TeleopWidget(*ctrl_gui, *new_ctrl,
				    motion_model);
    } break; 
    case 1:   // file controller & associated widget
      controller = new FileCtrl(motion_model, time_step, file_name);
      break; 
    case 2:   // PID reaching controller & associated widget
      controller = new PIDreachCtrl(motion_model, time_step,
				    goal, coef);
      break; 
    case 3: { // analytic reaching controller & associated widget
      SmoothPathCtrl *new_ctrl =
	new SmoothPathCtrl(motion_model, time_step, goal);
      controller = new_ctrl;
      if (ctrl_gui) // if so, makes the connection
	connect( new_ctrl, SIGNAL( pathChanged() ),
		 ctrl_gui, SLOT( updateTrajectory() ) );
    } break; 
    default: { // tracking controllers & associated widget
      TrackingCtrl *new_ctrl =  // needed for trajectory()
	ctrl_id == 6 ? (TrackingCtrl*) 
	new AnalyticCtrl(motion_model, time_step, file_name, q)
	: ctrl_id == 5 ? (TrackingCtrl*) 
	new PIDCtrl(motion_model, time_step, file_name, q, coef)
	: (TrackingCtrl*) 
	new ImitateCtrl(motion_model, time_step, file_name, q);
      controller = new_ctrl; 
      if (ctrl_gui) // if so, reset the tracked trajectory
	ctrl_gui->newTrajectory( new_ctrl->trajectory() ); }
    } // end of switch (defining the new controller & widget)
    //std::cout << "Input File: '" << file_name << "'\n";
    if (ctrl_id != 0)  // defines the new controller's widget
      ctrl_wdgt = new ViewCtrlWdgt(*ctrl_gui, *controller,
				   motion_model);
  } // end of if (if change is validated)
  else  // changes the pop-up menu back to the proper line
    ctrl_select.setCurrentIndex(ctrl_id); 

} // end of void QtCtrlSelect::updateCtrl()  -------------------------
    
// Updates the button displaying the save file.
void QtCtrlSelect::updateFileName() { 
  save_file_name = QFileDialog::getSaveFileName
    ( this, tr("Please select a save file"),
      "", tr("CSV File (*.csv *.txt)") );
  if ( save_file_name.isNull() || save_file_name.isEmpty() )
    save_file.setText( tr(default_save_name) );
  else {
    QFileInfo file_info(save_file_name); 
    save_file.setText( file_info.baseName() );
  } // end of else (file is defined)
} // end of void QtCtrlSelect::updateFileName() ----------------------
