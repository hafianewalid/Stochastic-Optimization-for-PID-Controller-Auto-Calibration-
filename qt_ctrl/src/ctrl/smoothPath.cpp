/**
 ** @file  src/ctrl/smoothPath.cpp
 **
 ** @brief Oriented point reaching controller class 
 **        using a smooth path generator.
 **
 ** @date  June 2019
 **/

#include <ctrl/smoothPath.hpp>
#include <iSeeML/rob/ArrayPaths.hpp>

// Cf ReachingCtrl::setGoal(const State&)
void SmoothPathCtrl::setGoal(const State& goal) {
  // motion model cannot change => static const data
  static const double&
    max_trans_vel  = motion_model.maxTranslVel(),
    norm_dist      = max_trans_vel * max_trans_vel / 2,
    acc_dist       = norm_dist / motion_model.maxTranslAcc(),
    dec_dist       = - norm_dist / motion_model.minTranslAcc(),
    max_curv       = motion_model.maxRotVel() / max_trans_vel,
    max_rot_acc    = std::min( motion_model.maxRotAcc(),
			       -motion_model.minRotAcc() ),
    max_curv_deriv = max_rot_acc / max_trans_vel;
  
  ReachingCtrl::setGoal(goal);
  const iSeeML::rob::OrPtConfig start = state.configuration()
    + iSeeML::rob::OrPtConfig(acc_dist, 0, 0),
    end = getGoal().configuration()
    + iSeeML::rob::OrPtConfig(- dec_dist, 0, 0);
  path = iSeeML::rob::FscPath(start, end, max_curv, max_curv_deriv);
  total_distance = acc_dist + path.length() + dec_dist;
  distance = 0;
  pathChanged();  // signal the path changed
} // end of SmoothPathCtrl::setGoal(const State&) --------------------

// Get the path to the aimed oriented point.
// Return The path to the aimed oriented point.
const iSeeML::rob::Path& SmoothPathCtrl::getPath() const {
  // motion model cannot change => static const data
  static const double&
    max_trans_vel  = motion_model.maxTranslVel(),
    norm_dist      = max_trans_vel * max_trans_vel / 2,
    acc_dist       = norm_dist / motion_model.maxTranslAcc(),
    dec_dist       = - norm_dist / motion_model.minTranslAcc();
  // the path is made of three paths
  static iSeeML::rob::ArrayPaths globalPath(3);
  // first one is a segment
  const iSeeML::rob::OrPtConfig start = path.start() +
    iSeeML::rob::OrPtConfig(- acc_dist, 0, 0); 
  globalPath.setPath( 0, true, new iSeeML::rob::LinCurvPath
		      ( iSeeML::rob::CurvConfig(start, 0),
			0, acc_dist ) );
  // second one is the saved path
  globalPath.setPath( 1, false, (iSeeML::rob::Path*)( &path.clone() ) );
  // third one is another segment
  globalPath.setPath( 2, true, new iSeeML::rob::LinCurvPath
		      ( iSeeML::rob::CurvConfig(path.end(), 0),
			0, dec_dist ) );
} // end of const iSeeML::rob::Path& SmoothPathCtrl::getPath() const -
  
/// Transform the planned path into an aimed trajectory.
/// Return The trajectory obtained from the planned path.
const std::list<State*>& SmoothPathCtrl::getAimedTrajectory() const {
  // motion model cannot change => static const data
  static const double&
    max_trans_acc = motion_model.maxTranslAcc(),
    max_trans_dec = motion_model.minTranslAcc(),
    max_trans_vel  = motion_model.maxTranslVel(),
    norm_dist      = max_trans_vel * max_trans_vel / 2,
    acc_dist       = norm_dist / max_trans_acc,
    dec_dist       = - norm_dist / max_trans_dec;
  const double l = path.length(), dt2 = time_step / 2; 
  double t = 0, v = 0, om = 0, d = - acc_dist, tmp;
  // this static variable remains defined until the end
  static std::list<State*> traject;
  // empty the list, to recompute it
  while (! traject.empty() ) {
    free( traject.front() );
    traject.pop_front();
  } // recompute the list
  tmp = max_trans_acc * time_step; 
  while (d < 0) { // handle the first linear part
    iSeeML::rob::OrPtConfig q = path.start() +
      iSeeML::rob::OrPtConfig(d, 0, 0);
    traject.push_back( new State(t, q, v, 0) );
    // prepare the next one
    t += time_step;
    d += v * time_step + tmp * dt2;
    v += tmp;
  } // distance and velocity need correction
  d -= (v - max_trans_vel) * dt2;
  v = max_trans_vel;
  tmp = v * time_step;
  while (d < l) { // add the current state
    iSeeML::rob::CurvConfig q = path[d]; 
    traject.push_back( new State(t, q, v, q.curvature() * v) );
    // prepare the next one
    t += time_step;
    d += tmp;
  } // distance and velocity need correction
  d -= l; 
  tmp = sqrt(-2 * d * max_trans_dec);
  v -= tmp;
  d -= tmp * dt2;
  tmp = max_trans_dec * time_step; 
  while (v > 0) { // handle the last linear part
    iSeeML::rob::OrPtConfig q = path.end() +
      iSeeML::rob::OrPtConfig(d, 0, 0);
    traject.push_back( new State(t, q, v, 0) );
    // prepare the next one
    t += time_step;
    d += v * time_step + tmp * dt2;
    v += tmp;
  }
  iSeeML::rob::OrPtConfig q = path.end() +
    iSeeML::rob::OrPtConfig(dec_dist, 0, 0);
  traject.push_back( new State(t, q, 0, 0) );
  return traject; 
} // end of const SmoothPathCtrl::getAimedTrajectory() const
 
// Cf Controller::choose_velocities(double&, double&, ...)
void SmoothPathCtrl::chooseVelocities(double& trans_vel,
				      double& rot_vel,
				      std::ostream& log_str) {
  static const double&
    max_trans_acc = motion_model.maxTranslAcc(),
    max_trans_dec = motion_model.minTranslAcc(),
    max_trans_vel  = motion_model.maxTranslVel(),
    norm_dist      = max_trans_vel * max_trans_vel / 2,
    acc_dist       = norm_dist / max_trans_acc,
    dec_dist       = - norm_dist / max_trans_dec,
    max_rot_acc    = motion_model.maxRotAcc(),
    max_rot_dec    = motion_model.minRotAcc(),
    max_rot_vel    = motion_model.maxRotVel();
  static State last_state = state;
  trans_vel = state.translationVelocity();  // current velocity
  // update the covered distance
  distance += ( trans_vel + last_state.translationVelocity() )
    * ( state.date() - last_state.date() ) / 2;
  /* ### 1st solution: compute the desired velocities; NOT GOOD ######
  // === compute desired translation velocity ========================
  // choose between accelerate, remain or brake
  moving_velocity  // desired velocity is the min between accelerate,
    = std::min( std::min( trans_vel + max_trans_acc * time_step,
			  max_trans_vel),  // maximum velocity and
		// decelerate to stop at total distance
		sqrt(2 * (distance - total_distance) * max_trans_dec)
		);
  // === compute desired rotation velocity ===========================
  if ( (distance <= acc_dist) 
       || (distance >= total_distance - dec_dist) )
    rot_vel = 0;  // straight motion while accelerating or braking
  else {  // rotation velocity depends on the path
    rot_vel = state.translationVelocity();  // current velocity
    const double defl =  // desired change of orientation
      state.configuration().projection(path[distance - acc_dist])
      .orientation();
    // desired velocity should allow to reach deflection (line 1),
    // remaining higher than reachable minimum velocity (l. 1 & 2)
    // and smaller than reachable maximum velocity (l. 3 & 4)
    turning_velocity
      = std::min( std::max( std::max(defl / time_step, - max_rot_vel),
			    rot_vel + max_rot_dec * time_step),
		  std::min( max_rot_vel, 
				 rot_vel + max_rot_acc * time_step) );
  } 
  ### 2nd solution: compute the desired accelerations; better? #### */
  if (distance <= acc_dist)                        // first part
    motion_model.applyAccelerations(moving_velocity, turning_velocity,
				    max_trans_acc, 0, time_step);
  else if (distance >= total_distance)             // stop
    stopMotion();
  else if (distance >= total_distance - dec_dist)  // last part
    motion_model.applyAccelerations(moving_velocity, turning_velocity,
				    max_trans_dec, 0, time_step);
  else {                                          // path part
    /* 3rd solution: get directly from path with anticipation
    const double defl =  // desired change of orientation
      state.configuration().projection(path[distance - acc_dist])
      .orientation(),    // forgot next line in 1st solution
      rot_acc = defl / time_step - state.translationVelocity();
    motion_model.applyAccelerations(moving_velocity, turning_velocity,
    0, rot_acc, time_step); */
    const double lookahead = time_step * max_trans_vel,
      arc_length = distance - acc_dist + lookahead;
    // path's length is a little bit too much (seg. fault): - 10⁻⁶
    double length = path.length() - 1E-6, s = arc_length > length
      ? length : arc_length, curv = path[s].curvature();
    // -=# recompute the path to improve it #=- 
    if (fabs(curv) < path.maxCurv() * 1E-3)  // very small curvature
      /* next constraint not strong enough!!!
	 && (arc_length < length * 2 / 3) ) {  // and not too far
      // recompute from here
      path.connect( state.configuration(), path.end() );*/ {
      iSeeML::rob::FscPath
	new_path( state.configuration(), path.end(),
		  path.maxCurv(), path.maxCurvDeriv() );
      // not much longer than what remains along older path
      if (new_path.length() < 1.2 * (length - distance + acc_dist) ) { 
	path = new_path;  // no long loop => copy
	// change the considered distance (beginning of the path)
	distance = acc_dist;  // and the computed values
	length = path.length() - 1E-6;
	total_distance = acc_dist + length + dec_dist;
	s = lookahead > length ? length : lookahead;
	curv = path[s].curvature();
      } } // -=# NOW WORKs: remains closer without loops #=- 
    moving_velocity  = max_trans_vel;
    turning_velocity = max_trans_vel * curv;
  } /*
  // === log for debug ===============================================
  log_str << "Vel. @d = " << distance << " [" << acc_dist
	  << "|" << total_distance - dec_dist << "|" << total_distance
	  << "]: " << moving_velocity << ", " << turning_velocity; */
  // === updates the parameters and send the update signal ===========
  updateVelocities(trans_vel, rot_vel);
  // === updates the last state variable =============================
  last_state = state;
} // end of SmoothPathCtrl::chooseVelocities(double&, double&, ...)
