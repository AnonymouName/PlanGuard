grammar UserIntercation;

//start
prog : intercation EOF          #start;

intercation : driving_pattern           #drivingpattern
	    | real_time_command         #realtimecommand
	    ;

driving_pattern : user_defined_driving_pattern    #userdefineddrivingpattern
		| predefined_driving_pattern      #predefineddrivingpattern
		;

user_defined_driving_pattern : 'USE ' String ' DRIVING PATTERN' (' WITH ' Weather_list)? ';' #userdrivingpattern
			     | 'CANCLE CURRENT DRIVING PATTERN;' #canclecurrentdrivingpattern
			     ;

predefined_driving_pattern : 'SET ' String ' DRIVING PATTERN' (predefined_driving_pattern_set)+ #setdrivingpattern
			   ;

predefined_driving_pattern_set : preferences_config       #preferencesconfig
			       | security_config          #securityconfig
			       | scenario_config          #scenarioconfig
			       | obstacle_config          #obstacleconfig
			       | weather_config           #weatherconfig
			       ;

preferences_config : 'SET PREFERENCES' (preferences)+ 'END' #setpreferences
		   ;
preferences : Preference_pra '=' value ';'     #setpreferencevalue
	    ;
Preference_pra : 'Max_planning_speed'
	       | 'Default_cruise_speed'
	       | 'Near_stop_speed'
	       | 'Whether_priority_change_lane'
	       | 'Driving_side'
	       | 'Rerouting_wait_time'
	       | 'Near_stop_deceleration'
	       | 'Whether_allow_borrow_lane'
	       ;

security_config : 'SET CONSTRAINTS' (constraints)+ 'END' #setconstraints
		  ;
constraints : Constraint_pra '=' value ';'     #setconstraintvalue
	    ;
Constraint_pra : 'Whether_trajectory_check'
	       | 'Trajectory_max_speed'
	       | 'Trajectory_min_speed'
	       | 'Trajectory_max_longitudinal_acceleration'
	       | 'Trajectory_min_longitudinal_acceleration'
	       | 'Trajectory_max_lateral_acceleration'
	       ;

scenario_config : 'SET SCENARIO' (scenario)+ 'END' #setscenario
		;
scenario : traffic_signal_scenario       #trafficsignalscenario
	 | road_scenario                 #roadscenario
	 ;

traffic_signal_scenario : 'FOR TRAFFIC SIGNAL ' Traffic_signal_list (traffic_signal)+ 'END' #settrafficsignal
			;
Traffic_signal_list : 'all_traffic_signals'
		    | 'keep_clear_signal'
		    | 'stop_signal'
		    | 'yield_signal'
		    | 'crosswalk_signal'
		    | 'traffic_light_signal'
		    | 'speed_limit_signal'
		    ;
traffic_signal : Traffic_signal_pra '=' value ';'     #settrafficsignalvalue
	       ;
Traffic_signal_pra : 'Whether_check'
		   | Both_pra
		   | 'Min_speed'
		   | 'Max_speed'
		   | 'Whether_red_light_turn_right'
		   ;

road_scenario : 'IF ENTER ' Road_list (road)+ 'END' #setroad
	      ;
Road_list : 'all_roads'
	  | 'lane_follow'
	  | 'change_lane'
	  | 'borrow_lane'
	  | 'emergency_pull_over'
	  | 'emergency_stop'
	  | 'pull_over'
	  | 'open_space_launch'
	  | 'valet_park'
	  | 'intersection'
	  | 'ramp'
	  | 'roundabout'
	  | 'tunnel'
	  | 'reach_destination'
	  ;
road : Road_pra '=' value ';'     #setroadvalue
     ;
Road_pra : Both_pra
	 | 'Time_interval'
	 | 'Whether_environment_check'
	 | 'Check_distance'
	 | 'Whether_speed_check'
	 | 'Use_distance'
	 | 'Expected_speed'
	 | 'Whether_pull_over'
	 | 'Whether_stop'
	 | 'Whether_creep'
	 | 'Permissible_maximum_heading_diffence'
	 | 'Permissible_max_steering_percentage'
	 ;
Both_pra : 'Preparation_distance'
	 | 'Stopping_distance'
	 | 'Wait_time'
	 | 'Creep_time'
	 | 'Turn_on_light'
	 ;

obstacle_config : 'DETECT Obstacle THEN' (obstacle)+ 'END' #setobstacle
		;
obstacle : Obstacle_pra '=' value ';'     #setobstaclevalue
	 ;
Obstacle_pra : 'Forward_buffer_distance'
	     | 'Backward_buffer_distance'
	     | 'Lateral_buffer_distance'
	     | 'Dynamic_Obstacle_Follow_distance'
	     | 'Dynamic_Obstacle_Yield_distance'
	     | 'Dynamic_Obstacle_Overtake_distance'
	     | 'Min_stop_distance'
	     | 'Whether_deceleration'
	     | 'Static_Obstacle_Deceleration_ratio'
	     | 'Dynamic_Obstacle_Deceleration_ratio'
	     ;

weather_config : 'ENCOUNTERING ' Weather_list (weather)+ 'END' #setweather
	       ;
Weather_list : 'rainy'
	     | 'snowy'
	     | 'foggy'
	     | 'windy'
	     ;
weather : Weather_pra '=' value ';'     #setweathervalue
	;

Weather_pra : 'Speed_Reduction_ratio'
	    | 'Longitudinal_Acceleration_Reduction_ratio'
	    | 'Lateral_Acceleration_Reduction_ratio'
	    | 'Distance_related_metric_expansion_factor'
	    ;

real_time_command : adjust_driving_pattern       #adjustdrivingpattern
 		  | receive_trajectory_command   #receivetrajectorycommand
 		  | receive_speed_command        #receivespeedcommand 
 		  | receive_stutes_command       #receivestutescommand
 		  | clear_real_time_setting      #clearrealtimecommand   
 		  ;

adjust_driving_pattern : 'SET ' Preference_pra (', ' Preference_pra)* ' AS ' value (', ' value)* ' IN preference;' #adjustpreferencepra
		       | 'SET ' Constraint_pra (', ' Constraint_pra)* ' AS ' value (', ' value)* ' IN constraint;' #adjustconstraintpra
		       | 'SET ' Traffic_signal_pra (', ' Traffic_signal_pra)* ' AS ' value (', ' value)* ' IN ' Traffic_signal_list ';' #adjusttrafficsignalpra
		       | 'SET ' Road_pra (', ' Road_pra)* ' AS ' value (', ' value)* ' IN ' Road_list ';' #adjustroadpra
		       | 'SET ' Obstacle_pra (', ' Obstacle_pra)* ' AS ' value (', ' value)* ' IN  obstacle;' #adjustobstaclepra
		       ;

receive_trajectory_command : 'RE-ROUTING AND RE-PLANNING;'  #rerouting
			   | 'KEEP IN CURRENT LANE;'       #keep_lane
			   | 'EXIT THE CURRENT LANE;'      #enter_exit_lane
			   | 'MAKE ' Num ' ' Left_Right ' LANE CHANGES' (' WITHIN ' Num 'METERS')? ';' #make_lane_change
			   | 'ENTER THE ' Left_Right ' MOST LANE' (' WITHIN ' Num 'METERS')? ';' #enter_most_lane
			   | 'ENTER THE ' Ordinal_num ' LANE FROM THE ' Left_Right (' WITHIN ' Num 'METERS')? ';' #enter_nth_lane
			   | 'ENTER THE ' (Left_Right ' TURN' | 'straight') ' LANE' (' WITHIN ' Num 'METERS')? ';' #enter_turn_lane
			   | 'BORROW LANES;' #borrow_lanes
			   | 'BORROW THE ' Left_Right ' LANE;'  #borrow_lane
			   | 'FOLLOW THE CAR IN FRONT;' #follow_car
			   | 'STOP BEFORE THE INTERSECTION;' #stop_before_intersection
			   | 'NOT STOP BEFORE THE INTERSECTION;' #not_stop_before_intersection
			   | 'PARK THE CAR IN PARKING SPACE ' String ';' #valet_park
			   | 'OPEN SPACE LAUNCH;' #open_space_launch
			   | 'PULL OVER TO THE ' Left_Right ';' #pull_over
			   | 'EMERGENCY PULL OVER;' #emergency_pull_over
			   | 'EMERGENCY STOP;' #emergency_stop
			   | 'VEHICLE LAUNCH;' #vehicle_launch
			   ;

Left_Right : 'left'
	   | 'right'
	   ;

receive_speed_command : 'KEEP THE CURRENT SPEED' (' WITHOUT CHECK')? ';'  #keep_current_speed
		      | 'KEEP A SPEED OF ' Num ' KM/H' (' WITHOUT CHECK')? ';'                   #keep_speed
		      | ('ACCELERATE' | 'DECELERATE') ' TO ' Num ' KM/H' (' WITHOUT CHECK')? ';' #accelerate_decelerate
		      | ('ACCELERATE' | 'DECELERATE') ' TO ' Num ' KM/H WITH ' Num ' M/S^2 ' ('ACCELERATION' | 'DECELERATION') (' WITHOUT CHECK')? ';' #accelerate_decelerate_with
		      ;

receive_stutes_command : ('TURN ON' | 'TURN OFF') ' THE ' Vehicle_light ';'   #turn_on_off_light
		       | 'SWITCH;' #switch_light
		       | 'HONK THE HORN;' #honk_horn
		       ;

Vehicle_light : 'high beam' 
	| 'low beam'
	| 'fog light' 
	| 'left turn light' 
	| 'right turn light' 
	| 'warning flash' 
	;

clear_real_time_setting : 'CLEAR REAL TIME SETTING;' #clearrealtimesetting
			| 'CLEAR ' ('speed' | 'stutes' | 'trajectory' | 'scenario' | 'obstacle' | 'preference') ' related SETTING;'    #clearportsetting
			| 'CLEAR ' (Traffic_signal_list | Road_list) ' related SETTING;' #cleartrafficsignalroadsetting
			;

value : Num              #num
      | Bool             #bool
      | '('Vehicle_light (', ' Vehicle_light)*')'        #vehiclelight
      ;

Num : [0-9]+ ('.' [0-9]+)?;
Bool : 'true' | 'false';
String : [a-z]([a-zA-Z0-9_])*;

Ordinal_num : '1st' | '2nd' | '3rd' | '4th' | '5th' | '6th' | '7th' | '8th' | '9th' | '10th';


WHITESPACE : [ \t\r\n]+ -> skip;



