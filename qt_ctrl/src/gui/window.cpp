/**
 * @file /src/gui/window.cpp
 *
 * @brief Qt based GUI for qt_ctrl.
 *
 * @date July 2018
 **/

#include <gui/window.hpp>
#include <gui/viewCtrl.hpp>
#include <gui/selectCtrl.hpp>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QDialogButtonBox>


const double QtCtrlGUI::time_step = .2;

/* The constructor requires <tt>main()</tt>'s arguments (they can 
 *  be forwarded by the @ref ROSnode "ROS node" to ROS).
 *
 * Parameter argc    the arguments' count,
 *           argv    the arguments' value,
 *           parent  the main widget parent, if any (optional).
 *
 * See ROSnode::ROSnode().
 */
QtCtrlGUI::QtCtrlGUI(const int argc, char** argv, QWidget* parent)
  : QMainWindow(parent),   menu_bar(this),  app_menu(&menu_bar), 
    help_menu(&menu_bar), tool_bar(this), start_stop_action(this), 
    about_action(this), about_Qt_action(this), ROS_server(this),
    motion_model(-.5, .5, M_PI/4, -.6, .5, -M_PI/8, M_PI/8),
    controller( new NoCtrl(motion_model) ), ctrl_node(argc, argv), 
    ctrl_wdgt( new ViewCtrlWdgt(*this, *controller, motion_model) ), 
    display(*this), logs(this) { // setup the UI
  setWindowTitle( tr("ROS - Qt Controlers Benchmark") );
  setWindowIcon( QIcon(":/files/icon.png") );
  // set the Start, "About Qt Ctrl" and "About Qt" actions
  start_stop_action.setText( tr("Start Ctrl") ); 
  start_stop_action.setShortcut(Qt::Key_S); 
  connect( &start_stop_action, SIGNAL( triggered() ),
	   this, SLOT( buttonStartStop() ) );
  about_action.setText( tr("About Qt &Ctrl") ); 
  connect( &about_action, SIGNAL( triggered() ),
	   this, SLOT( actionAbout() ) );
  about_Qt_action.setText( tr("About &Qt") ); 
  connect( &about_Qt_action, SIGNAL( triggered() ),
	   qApp, SLOT( aboutQt() ) );
  // set the menu bar and the menus
  setMenuBar(&menu_bar);
  app_menu.setTitle( tr("&App") );
  app_menu.addAction(&start_stop_action);
  menu_bar.addMenu(&app_menu);
  help_menu.setTitle( tr("&Help") );
  help_menu.addAction(&about_action);
  help_menu.addAction(&about_Qt_action);
  menu_bar.addMenu(&help_menu);
  // set the tool bar and add start button
  addToolBar(&tool_bar);
  tool_bar.addAction(&start_stop_action);
  // set the main widget, with teleoperation and logging
  QWidget *central_widget = new QWidget(this); 
  setCentralWidget(central_widget);
  // create a layout for the central widget
  QGridLayout *central_layout = new QGridLayout(central_widget);
  central_layout->addWidget(&( ctrl_wdgt->widget() ), 0, 0);
  central_layout->addWidget(&display, 0, 1);
  QGroupBox *log_box = new QGroupBox(tr("Logs"), central_widget);
  QBoxLayout *log_layout =   // create a layout for the log box
    new QBoxLayout(QBoxLayout::LeftToRight, log_box);
  logs.setMinimumSize(400, 100);
  log_layout->addWidget(&logs);
  central_layout->addWidget(log_box, 1, 0, 1, 2);
  // restore the settings
  readSettings();
  // initialises the state's display
  ctrl_wdgt->updateState( State() );
  // idem for the commands
  ctrl_wdgt->updateCommands(0, 0);
  // connects log signal of the ROS node to the GUI method
  logs.setModel(&( ctrl_node.loggingModel() ) );
  connect( &(ctrl_node), SIGNAL( loggingUpdated() ),
	   this, SLOT( updateLogging() ) );
  // needed after Q_DECLARE_METATYPE and before the connects
  qRegisterMetaType<State>();
  connectCtrl(); // connects the controller signals to the GUI methods

// modif add 
buttonStartStop();


} // end of QtCtrlGUI::QtCtrlGUI(int, char**, QWidget*) --------------

// Load up Qt program settings at startup.
void QtCtrlGUI::readSettings() {
    QSettings settings("Qt-Ros Package", "qt_ctrl");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value
     ("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value
      ("host_url", QString("192.168.1.3")).toString(); /*
    //QString topic_name = settings.value
    //("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
	} */
} // end of void QtCtrlGUI::readSettings() ---------------------------

// Save Qt program settings when closing.
void QtCtrlGUI::writeSettings() {
    QSettings settings("Qt-Ros Package", "qt_ctrl"); /*
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant
		      (ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant
		      (ui.checkbox_remember_settings->isChecked()));
						     */
} // end of void QtCtrlGUI::writeSettings() --------------------------

// Method activated by the "About" menu item (not yet used).
void QtCtrlGUI::actionAbout() {
    QMessageBox::about
      (this, tr("About ROS-Qt Control GUI"),
       tr("<h2>ROS-Qt Control GUI</h2>\
           <p>Copyright <a href=\"http://members.loria.fr/AScheuer\"\
	      >Alexis Scheuer</a>, <a href=                         \
              \"http://www.univ-lorraine.fr\">UL</a>|<a href=       \
              \"http://fst.univ-lorraine.fr\">FST</a> &amp; <a href=\
              \"http://www.loria.fr\">Loria</a></p>		    \
           <p>This package proposes several controllers for ROS     \
              with a Qt GUI, in order to compare them.</p>"));
} // end of void QtCtrlGUI::actionAbout() ----------------------------

/// @brief Method activated by the "Start/Stop" button.
void QtCtrlGUI::buttonStartStop() {
  bool not_connected, trying, tracking, reaching;
  start_stop_action.setEnabled(false);// prevent the button activation
  if ( ctrl_node.connected() ) {  // ### stop the control ############
    ctrl_node.end();                // stop ROS node (= server + ctrl)
    if ( ROS_server.isRunning() ) { // should we stop the ROS master?
      const QMessageBox::StandardButton ans = QMessageBox::question
	(this, tr("Stopping ROS master?"),  // title of the dialog
	 tr("Should this GUI stop the TurtleBot simulation "
	    "(you may not be able to start it again)?"),
       // 3 buttons, Cancel is the default
       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
      if (ans == QMessageBox::Yes)
	ROS_server.stop();  // stop ROS server's thread
    } // en of if (ROS server is running)
    start_stop_action.setText( tr("Start Ctrl") ); 
  } else {  // ### start the control #################################
    do {
      QtCtrlSelect dialog(this, time_step, motion_model);

      // modif ( com )
      //trying = dialog.exec() == QDialog::Accepted;
      
      //modif 
      trying=true;  

      if (trying) { /* not yet working 
	const char*msg = "Do you want to clear the motion history?"; 
	if (QMessageBox::question(this, tr("Clear history?"), tr(msg),
				  QMessageBox::Yes | QMessageBox::No)
	    == QMessageBox::No)  display.newTrajectory(); */
	if ( dialog.startROS() ) {  // starts ROS with a workspace
	  ROS_server.setEmptyWorkspace( dialog.emptyROSworkspace() );
	  ROS_server.start();
	  // previous call returns immediatly, but needs...
	  ROS_server.sleep(5);  // ... about 5 s to be effective
	} // end of if (start ROS)
	// memorize old pointers before reallocation (free later)
	Controller*    old_ctrl = controller;
	ControlWidget* old_wdgt = ctrl_wdgt;
	controller = dialog.newController();
	ctrl_wdgt  = dialog.newCtrlWidget(); 
	connectCtrl(); // connects controller signals to GUI methods
	hide();        // hide the window before modifying it
	// add the new control widget to the main window
	QGridLayout *central_layout =
	  (QGridLayout*)( centralWidget()->layout() );
	old_wdgt->widget().hide(); // remove not enough, hide needed
	central_layout->removeWidget( &( old_wdgt->widget() ) );
	central_layout->addWidget(&( ctrl_wdgt->widget() ), 0, 0);
	show();  // show/redraw window once modifications are done
	// change has been done, let's free old pointers
	delete old_wdgt;
	delete old_ctrl;
	// valid save file => set it
	const QString& saveFile = dialog.saveFileName(); 
        
        //modif
	/*if ( (! saveFile.isNull() ) && (! saveFile.isEmpty() ) )
	  display.setOutput( saveFile.toStdString().c_str() );*/
       
        display.setOutput("res.csv"); 

	ctrl_node.init(*controller);  // try to start control node
	not_connected = ! ctrl_node.connected();
      } // end of if (trying)
    } while (trying && not_connected);
    if (trying)  start_stop_action.setText( tr("Stop Ctrl") );
  } // end of else (start control)
  start_stop_action.setEnabled(true);
} // end of void QtCtrlGUI::buttonStart() ----------------------------

/** @brief This is <tt>@ref index "qt_ctrl"</tt> main function, 
 **        starting a @ref QtCtrlGUI "Qt control GUI" 
 **        in a @ref refs_qt_application "Qt application".
 **
 ** @param argc  The parameters' count, and
 ** @param argv  the parameters' values, are both forwarded 
 **                  to Qt and ROS.
 **
 ** @return  The result of @ref refs_qt_application "Qt application"'s
 **          execution.
 **/
int main(int argc, char** argv) {
  QApplication app(argc, argv);  // start the Qt main loop
  QtCtrlGUI   ctrl(argc, argv);  // create the Qt control main window
  ctrl.show();                   // show this window
  // stop the Qt main loop when last window (= main window) is closed
  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  return app.exec();             // return Qt main loop's return value
} // end of int main(int, char**) ------------------------------------
