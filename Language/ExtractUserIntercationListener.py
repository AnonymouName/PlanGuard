from UserIntercationListener import UserIntercationListener
from UserIntercationParser import UserIntercationParser
import os, copy
import shutil

pattern_file_path = "pattern"
real_file_path = "real"

class UserIntercationError(Exception):
	pass


class ExtractUserIntercationListener(UserIntercationListener):

	weather_name = ""
	weather = ["rainy", "snowy", "foggy", "windy"]
	weather_pra = ["Speed_Reduction_ratio", "Longitudinal_Acceleration_Reduction_ratio", "Lateral_Acceleration_Reduction_ratio",
			"Distance_related_metric_expansion_factor"]
	# weather_value = {}
	# weather_value_use = {}

	set_pattern = False
	current_pattern = ""
	preference = ["Max_planning_speed", "Default_cruise_speed","Near_stop_speed", "Whether_priority_change_lane",
	       	      "Driving_side", "Rerouting_wait_time", "Near_stop_deceleration", "Whether_allow_borrow_lane"]
	# preference_value = {}

	constraints= ["Whether_trajectory_check", "Trajectory_max_speed", "Trajectory_min_speed"
		      "Trajectory_max_longitudinal_acceleration", "Trajectory_min_longitudinal_acceleration", 
		      "Trajectory_max_lateral_acceleration"]
	# constraints_value = {}

	scenario_name = ""
	both_pra = ["Preparation_distance", "Stopping_distance", "Wait_time", "Creep_time", "Turn_on_light"]
	traffic_signal = ["keep_clear_signal", "stop_signal", "yield_signal", "crosswalk_signal", "traffic_light_signal",
		   	  "speed_limit_signal"]
	traffic_signal_pra = ["Whether_check", "Min_speed", "Max_speed", "Whether_red_light_turn_right"]
	# traffic_signal_value = {}

	road = ["lane_follow", "change_lane", "borrow_lane", "emergency_pull_over", "emergency_stop", "pull_over",
	        "open_space_launch", "valet_park", "intersection", "ramp", "roundabout", "tunnel", "reach_destination"]
	road_pra = ["Time_interval", "Whether_environment_check", "Check_distance", "Whether_speed_check", 
	     	    "Use_distance", "Expected_speed", "Whether_pull_over", "Whether_stop", "Whether_creep",
		    "Permissible_maximum_heading_diffence", "Permissible_max_steering_percentage"]
	# road_value = {}

	obstacle_pra = ["Forward_buffer_distance", "Backward_buffer_distance", "Lateral_buffer_distance", 
		 	"Dynamic_Obstacle_Follow_distance", "Dynamic_Obstacle_Yield_distance", 
			"Min_stop_distance", "Static_Obstacle_Deceleration_ratio", "Dynamic_Obstacle_Deceleration_ratio",
			"Whether_declearation", "Dynamic_Obstacle_Overtake_distance"]
	# obstacle_value = {}

	# real_command = {}

	def __init__(self, parser: UserIntercationParser) -> None:
		self.parser = parser
		self.weather_value = {}
		self.weather_value_use = {}
		self.preference_value = {}
		self.constraints_value = {}
		self.traffic_signal_value = {}
		self.road_value = {}
		self.obstacle_value = {}
		self.real_command = {}
		self.current_pattern = ""

	def bool_2_double(self, value):
		if value == 'true':
			return '1.0'
		elif value == 'false':
			return '0.0'
		else:
			return value

	def trans_weather(self, Id, value):
		if "Whether" in Id or "whether" in Id:
			return value
		else:
			if ("speed" in Id or "Speed" in Id) and self.weather_value_use["Speed_Reduction_ratio"] != "":
				value = str(round(float(value) * self.weather_value_use["Speed_Reduction_ratio"], 2))
			elif "Acceleration" in Id or "acceleration" in Id:
				if ("longitudinal" in Id or "Longitudinal" in Id) and self.weather_value_use["Longitudinal_Acceleration_Reduction_ratio"] != "":
					value = str(round(float(value) * self.weather_value_use["Longitudinal_Acceleration_Reduction_ratio"], 2))
				elif ("lateral" in Id or "Lateral" in Id) and self.weather_value_use["Lateral_Acceleration_Reduction_ratio"] != "":
					value = str(round(float(value) * self.weather_value_use["Lateral_Acceleration_Reduction_ratio"], 2))
			elif ("Distance" in Id or "distance" in Id) and self.weather_value_use["Distance_related_metric_expansion_factor"] != "":
				value = str(round(float(value) * self.weather_value_use["Distance_related_metric_expansion_factor"], 2))
			return value

	def enterUserdrivingpattern(self, ctx: UserIntercationParser.UserdrivingpatternContext):
		pattern_name = ctx.String().getText() + '.txt'
		try:
			weather_name = ctx.Weather_list().getText()
		except:
			weather_name = ""
		
		if os.path.exists(os.path.join(pattern_file_path, pattern_name)):
			if os.path.exists("pattern.txt"):
				raise UserIntercationError("pattern.txt already exists!")
			
			new_file_name = pattern_name.split('.')[0] + '_.txt' 

			with open (os.path.join(pattern_file_path, new_file_name), 'w') as f:
				if weather_name == "":
					with open(os.path.join(pattern_file_path, pattern_name), 'r') as f1:
						for line in f1.readlines():
							if "pattern:weather" in line:
								continue
							else:
								f.write(line)
				else:
					for par in self.weather_pra:
						self.weather_value_use[par] = ""

					with open(os.path.join(pattern_file_path, pattern_name), 'r') as f1:
						for line in f1.readlines():
							if "pattern:weather:"+weather_name in line:
								self.weather_value_use[line.split('=')[0].split(':')[-1]] = float(line.split('=')[-1])
							else:
								continue
					
					with open(os.path.join(pattern_file_path, pattern_name), 'r') as f1:
						for line in f1.readlines():
							if "pattern:weather" in line:
								continue
							else:
								Id = line.split('=')[0]
								value = line.split('=')[-1]
								key = Id.split(':')[-1]
								value = self.trans_weather(key, value)
								f.write(Id + '=' + value + '\n')
					
			shutil.copy(os.path.join(pattern_file_path, new_file_name), "pattern.txt")
			os.remove(os.path.join(pattern_file_path, new_file_name))
			self.weather_value_use.clear()
		else:
			raise UserIntercationError("Pattern not found!")

	def enterCanclecurrentdrivingpattern(self, ctx: UserIntercationParser.CanclecurrentdrivingpatternContext):
		if os.path.exists("pattern.txt"):
			raise UserIntercationError("Wait for the completion of the previous driving pattern processing. !")
		else:
			shutil.copy(os.path.join(pattern_file_path, "default.txt"), "pattern.txt")

	def enterSetdrivingpattern(self, ctx: UserIntercationParser.SetdrivingpatternContext):
		pattern_name = ctx.String().getText() + '.txt'
		if os.path.exists(os.path.join(pattern_file_path, pattern_name)):
			raise UserIntercationError('The pattern exists!')
		else:
			self.current_pattern = pattern_name
			with open(os.path.join(pattern_file_path, pattern_name), 'w'):
				pass
	
	def enterSetpreferences(self, ctx: UserIntercationParser.SetpreferencesContext):
		if self.preference_value == {}:
			for i in range(len(self.preference)):
				self.preference_value[self.preference[i]] = ""
		else:
			pass

	def enterSetpreferencevalue(self, ctx: UserIntercationParser.SetpreferencevalueContext):
		Id = ctx.Preference_pra().getText()
		value = ctx.value().getText()
		value = self.bool_2_double(value)
		if "speed" in Id:
			value = str(round(float(value) / 3.6, 2))
		if Id in self.preference: #and self.preference_value[Id] == "":
			self.preference_value[Id] = value
		else:
			raise UserIntercationError("Set preference fail!")
	
	def enterSetconstraints(self, ctx: UserIntercationParser.SetconstraintsContext):
		if self.constraints_value != {}:
			raise UserIntercationError("Constraints init fail!")
		else:
			for i in range(len(self.constraints)):
				self.constraints_value[self.constraints[i]] = ""

	def enterSetconstraintvalue(self, ctx: UserIntercationParser.SetconstraintvalueContext):
		Id = ctx.Constraint_pra().getText()
		value = ctx.Value().getText()
		value = self.bool_2_double(value)
		if "speed" in Id:
			value = str(round(float(value) / 3.6, 2))
		if Id in self.constraints and self.constraints_value[Id] == "":
			self.constraints_value[Id] = value
		else:
			raise UserIntercationError("Set constraint fail!")
	
	def enterSetscenario(self, ctx: UserIntercationParser.SetscenarioContext):
		if self.current_pattern == "":
			raise UserIntercationError("Please set the pattern first!")
		if self.scenario_name != "":
			raise UserIntercationError("There is a scenario being configured!")
		else:
			for traffic_name in self.traffic_signal:
				self.traffic_signal_value[traffic_name] = {}
				for pra in self.traffic_signal_pra + self.both_pra:
					self.traffic_signal_value[traffic_name][pra] = ""
			for road_name in self.road:
				self.road_value[road_name] = {}
				for pra in self.road_pra + self.both_pra:
					self.road_value[road_name][pra] = ""

	def enterSettrafficsignal(self, ctx: UserIntercationParser.SettrafficsignalContext):
		if self.scenario_name != "":
			raise UserIntercationError("There is a scenario being configured!")
		self.scenario_name = ctx.Traffic_signal_list().getText()

	def enterSettrafficsignalvalue(self, ctx:UserIntercationParser.SettrafficsignalvalueContext):
		if self.scenario_name == "":
			raise UserIntercationError("Please set the scenario first!")
		pra = ctx.Traffic_signal_pra().getText()
		value = ctx.Value().getText()
		value = self.bool_2_double(value)
		if "speed" in pra:
			value = str(round(float(value) / 3.6, 2))
		if self.scenario_name == "all_traffic_signals":
			for traffic_name in self.traffic_signal:
				if pra in (self.traffic_signal_pra+self.both_pra) and self.traffic_signal_value[traffic_name][pra] == "":
					self.traffic_signal_value[traffic_name][pra] = value
		else:
			if pra in (self.traffic_signal_pra+self.both_pra):
				self.traffic_signal_value[self.scenario_name][pra] = value

	def exitSettrafficsignal(self, ctx: UserIntercationParser.SettrafficsignalContext):
		self.scenario_name = ""

	def enterSetroad(self, ctx: UserIntercationParser.SetroadContext):
		if self.scenario_name != "":
			raise UserIntercationError("There is a scenario being configured!")
		self.scenario_name = ctx.Road_list().getText()
	
	def enterSetroadvalue(self, ctx: UserIntercationParser.SetroadvalueContext):
		if self.scenario_name == "":
			raise UserIntercationError("Please set the scenario first!")
		pra = ctx.Road_pra().getText()
		value = ctx.Value().getText()
		value = self.bool_2_double(value)
		if "speed" in pra and "Wherther" not in pra:
			value = str(round(float(value) / 3.6, 2))
		if self.scenario_name == "all_roads":
			for road_name in self.road:
				if pra in (self.road_pra+self.both_pra) and self.road_value[road_name][pra] == "":
					self.road_value[road_name][pra] = value
		else:
			if pra in (self.road_pra+self.both_pra):
				self.road_value[self.scenario_name][pra] = value

	def exitSetroad(self, ctx: UserIntercationParser.SetroadContext):
		self.scenario_name = ""

	def enterObstacleconfig(self, ctx: UserIntercationParser.ObstacleconfigContext):
		if self.current_pattern == "":
			raise UserIntercationError("Please set the pattern first!")
		if self.obstacle_value != {}:
			raise UserIntercationError("Obstacle init fail!")
		else:
			for pra in self.obstacle_pra:
				self.obstacle_value[pra] = ""

	def enterSetobstaclevalue(self, ctx: UserIntercationParser.SetobstaclevalueContext):
		pra = ctx.Obstacle_pra().getText()
		value = ctx.Value().getText()
		value = self.bool_2_double(value)
		if pra in self.obstacle_pra and self.obstacle_value[pra] == "":
			self.obstacle_value[pra] = value

	def enterWeatherconfig(self, ctx: UserIntercationParser.WeatherconfigContext):
		if self.current_pattern == "":
			raise UserIntercationError("Please set the pattern first!")
		if self.weather_value != {}:
			raise UserIntercationError("Weather init fail!")
		else:
			for weather_name in self.weather:
				self.weather_value[weather_name] = {}
				for pra in self.weather_pra:
					self.weather_value[weather_name][pra] = ""

	def enterSetweather(self, ctx: UserIntercationParser.SetweatherContext):
		if self.weather_name != "":
			raise UserIntercationError("There is a weather being configured!")
		self.weather_name = ctx.Weather_list().getText()

	def enterSetweathervalue(self, ctx: UserIntercationParser.SetweathervalueContext):
		if self.weather_name == "":
			raise UserIntercationError("Please set the weather first!")
		pra = ctx.Weather_pra().getText()
		value = ctx.Value().getText()
		value = self.bool_2_double(value)
		if pra in self.weather_pra and self.weather_value[self.weather_name][pra] == 0.0:
			self.weather_value[self.weather_name][pra] = value
		else:
			raise UserIntercationError("Set weather fail!")

	def exitSetweather(self, ctx: UserIntercationParser.SetweatherContext):
		self.weather_name = ""
		# input_weather = input("Please input the weather: ")
		# self.weather_name = input_weather

	def exitSetdrivingpattern(self, ctx: UserIntercationParser.SetdrivingpatternContext):
		if self.current_pattern == "":
			raise UserIntercationError("Please set the pattern first!")
		else:
			with open(os.path.join(pattern_file_path, self.current_pattern), 'a') as f:
				for key, value in self.preference_value.items():
					if value != "":
						Id = 'pattern:preference:' + key
						f.write(Id + '=' + value + '\n')
				for key, value in self.constraints_value.items():
					if value != "":
						Id = 'pattern:constraint:' + key
						f.write(Id + '=' + value + '\n')
				for scenario_name, related_value in self.traffic_signal_value.items():
					for pra, value in related_value.items():
						if value != "":
							Id = 'pattern:scenario:' + scenario_name + ':' + pra
							f.write(Id + '=' + value + '\n')
				for scenario_name, related_value in self.road_value.items():
					for pra, value in related_value.items():
						if value != "":
							Id = 'pattern:scenario:' + scenario_name + ':' + pra
							f.write(Id + '=' + value + '\n')
				for obstacle_name, related_value in self.obstacle_value.items():
					for pra, value in related_value.items():
						if value != "":
							Id = 'pattern:obstacle:' + obstacle_name + ':' + pra
							f.write(Id + '=' + value + '\n')
				for weather_name, related_value in self.weather_value.items():
					for pra, value in related_value.items():
						if value != "":
							Id = 'pattern:weather:' + weather_name + ':' + pra
							f.write(Id + '=' + value + '\n')
		
	def enterRealtimecommand(self, ctx: UserIntercationParser.RealtimecommandContext):
		if os.path.exists("once.txt"):
			raise UserIntercationError("Wait for the completion of the previous real command processing!")
	
	def enterAdjustpreferencepra(self, ctx: UserIntercationParser.AdjustpreferencepraContext):
		if len(ctx.Preference_pra()) != len(ctx.Value()):
			raise UserIntercationError("Please check pragram and value!")
		
		for i in range(len(ctx.Preference_pra())):
			Id = ctx.Preference_pra()[i].getText()
			Id = 'real:preference:' + Id
			value = ctx.Value()[i].getText()
			value = self.bool_2_double(value)
			if "speed" in Id and "Wherther" not in Id:
				value = str(round(float(value) / 3.6, 2))
			if Id in self.preference and Id not in self.real_command:
				self.real_command[Id] = value
			else:
				raise UserIntercationError("Please check pragram and value!")
		
		with open("once.txt", 'w') as f:
			for key, value in self.real_command.items():
				f.write(key + '=' + value + '\n')
		self.real_command.clear()
	
	def enterAdjustconstraintpra(self, ctx: UserIntercationParser.AdjustconstraintpraContext):
		if len(ctx.Preference_pra()) != len(ctx.Value()):
			raise UserIntercationError("Please check pragram and value!")
		for i in range(len(ctx.Preference_pra())):
			Id = ctx.Preference_pra()[i].getText()
			Id = 'real:constraint:' + Id
			value = ctx.Value()[i].getText()
			value = self.bool_2_double(value)
			if "speed" in Id and "Wherther" not in Id:
				value = str(round(float(value) / 3.6, 2))
			if Id in self.constraints and Id not in self.real_command:
				self.real_command[Id] = value
			else:
				raise UserIntercationError("Please check pragram and value!")
		
		with open("once.txt", 'w') as f:
			for key, value in self.real_command.items():
				f.write(key + '=' + value + '\n')
		self.real_command.clear()

	def enterAdjusttrafficsignalpra(self, ctx: UserIntercationParser.AdjusttrafficsignalpraContext):
		if len(ctx.Preference_pra()) != len(ctx.Value()):
			raise UserIntercationError("Please check pragram and value!")
		traffic_light_signal = ctx.Traffic_signal_list().getText()
		for i in range(len(ctx.Preference_pra())):
			Id = ctx.Preference_pra()[i].getText()
			if traffic_light_signal == "all_traffic_signals":
				for traffic_name in self.traffic_signal:
					Id = 'real:scenario:' + traffic_name + ':' + Id
					value = ctx.Value()[i].getText()
					value = self.bool_2_double(value)
					if "speed" in Id and "Wherther" not in Id:
						value = str(round(float(value) , 2))
					if Id in (self.traffic_signal_pra+self.both_pra) and Id not in self.real_command:
						self.real_command[Id] = value
					else:
						raise UserIntercationError("Please check pragram and value!")
			else:
				Id = 'real:scenario:' + traffic_light_signal + ':' + Id
				value = ctx.Value()[i].getText()
				value = self.bool_2_double(value)
				if "speed" in Id and "Wherther" not in Id:
					value = str(round(float(value) / 3.6, 2))
				if Id in (self.traffic_signal_pra+self.both_pra):
					self.real_command[Id] = value
				else:
					raise UserIntercationError("Please check pragram and value!")
		
		with open("once.txt", 'w') as f:
			for key, value in self.real_command.items():
				f.write(key + '=' + value + '\n')
		self.real_command.clear()
	
	def enterAdjustroadpra(self, ctx: UserIntercationParser.AdjustroadpraContext):
		if len(ctx.Preference_pra()) != len(ctx.Value()):
			raise UserIntercationError("Please check pragram and value!")
		road = ctx.Road_list().getText()
		for i in range(len(ctx.Preference_pra())):
			Id = ctx.Preference_pra()[i].getText()
			if road == "all_roads":
				for road_name in self.road:
					Id = 'real:scenario:' + road_name + ':' + Id
					value = ctx.Value()[i].getText()
					value = self.bool_2_double(value)
					if "speed" in Id and "Wherther" not in Id:
						value = str(round(float(value) / 3.6, 2))
					if Id in (self.road_pra+self.both_pra) and Id not in self.real_command:
						self.real_command[Id] = value
					else:
						raise UserIntercationError("Please check pragram and value!")
			else:
				Id = 'real:scenario:' + road + ':' + Id
				value = ctx.Value()[i].getText()
				value = self.bool_2_double(value)
				if "speed" in Id and "Wherther" not in Id:
					value = str(round(float(value) / 3.6, 2))
				if Id in (self.road_pra+self.both_pra):
					self.real_command[Id] = value
				else:
					raise UserIntercationError("Please check pragram and value!")
		
		with open("once.txt", 'w') as f:
			for key, value in self.real_command.items():
				f.write(key + '=' + value + '\n')
		self.real_command.clear()

	def enterAdjustobstaclepra(self, ctx: UserIntercationParser.AdjustobstaclepraContext):
		if len(ctx.Preference_pra()) != len(ctx.Value()):
			raise UserIntercationError("Please check pragram and value!")
		for i in range(len(ctx.Preference_pra())):
			Id = ctx.Preference_pra()[i].getText()
			Id = 'real:obstacle:' + Id
			value = ctx.Value()[i].getText()
			value = self.bool_2_double(value)
			if "speed" in Id and "Wherther" not in Id:
				value = str(round(float(value) / 3.6, 2))
			if Id in self.obstacle_pra and Id not in self.real_command:
				self.real_command[Id] = value
			else:
				raise UserIntercationError("Please check pragram and value!")
		
		with open("once.txt", 'w') as f:
			for key, value in self.real_command.items():
				f.write(key + '=' + value + '\n')
		self.real_command.clear()

	def enterRerouting(self, ctx: UserIntercationParser.ReroutingContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:rerouting=1.0\n')

	def enterKeep_lane(self, ctx: UserIntercationParser.Keep_laneContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:lane_follow=1.0\n')
	
	def enterEnter_exit_lane(self, ctx: UserIntercationParser.Enter_exit_laneContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:change_lane=1.0\n')
			f.write('real:scenario:change_lane:l_r=3.0\n')
			f.write('real:scenario:change_lane:times=1.0\n')

	def enterMake_lane_change(self, ctx: UserIntercationParser.Make_lane_changeContext):
		l_r = ctx.Left_Right().getText()
		if (len(ctx.Num()) == 2):
			times = ctx.Num()[0].getText()
			distance = ctx.Num()[1].getText()
		else:
			times = ctx.Num()[0].getText()
			distance = '0.0'
		with open("once.txt", 'w') as f:
			f.write('real:trj:change_lane=1.0\n')
			f.write('real:scenario:change_lane:l_r=' + l_r + '\n')
			f.write('real:scenario:change_lane:times=' + times + '\n')
			f.write('real:scenario:change_lane:distance=' + distance + '\n')
	
	def enterEnter_most_lane(self, ctx: UserIntercationParser.Enter_most_laneContext):
		l_r_most = ctx.Left_Right().getText()
		if (len(ctx.Num()) == 1):
			distance = ctx.Num().getText()
		else:
			distance = '0.0'
		with open("once.txt", 'w') as f:
			f.write('real:trj:change_lane=1.0\n')
			f.write('real:scenario:change_lane:l_r_most=' + l_r_most + '\n')
			f.write('real:scenario:change_lane:distance=' + distance + '\n')
	
	def enterEnter_nth_lane(self, ctx: UserIntercationParser.Enter_nth_laneContext):
		l_r = ctx.Left_Right().getText()
		n = ctx.Ordinal_num().getText()
		n = n[:-2]
		if (len(ctx.Num()) == 1):
			distance = ctx.Num().getText()
		else:
			distance = '0.0'
		with open("once.txt", 'w') as f:
			f.write('real:trj:change_lane=1.0\n')
			f.write('real:scenario:change_lane:l_r=' + l_r + '\n')
			f.write('real:scenario:change_lane:num=' + n + '\n')
			f.write('real:scenario:change_lane:distance=' + distance + '\n')
	
	def enterEnter_turn_lane(self, ctx: UserIntercationParser.Enter_turn_laneContext):
		l_r_g = ctx.Left_Right().getText()
		if (len(ctx.Num()) == 1):
			distance = ctx.Num().getText()
		else:
			distance = '0.0'
		with open("once.txt", 'w') as f:
			f.write('real:trj:change_lane=1.0\n')
			f.write('real:scenario:change_lane:g_l_r=' + l_r_g + '\n')
			f.write('real:scenario:change_lane:distance=' + distance + '\n')

	def enterBorrow_lanes(self, ctx: UserIntercationParser.Borrow_lanesContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:borrowlane=1.0\n')
	
	def enterBorrow_lane(self, ctx: UserIntercationParser.Borrow_laneContext):
		l_r = ctx.Left_Right().getText()
		with open("once.txt", 'w') as f:
			f.write('real:trj:borrowlane=1.0\n')
			f.write('real:scenario:borrowlane:l_r=' + l_r + '\n')
	
	def enterFollow_car(self, ctx: UserIntercationParser.Follow_carContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:follow_car=1.0\n')

	def enterStop_before_intersection(self, ctx: UserIntercationParser.Stop_before_intersectionContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:whether_stop_before_intersection=1.0\n')
	
	def enterNot_stop_before_intersection(self, ctx: UserIntercationParser.Not_stop_before_intersectionContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:whether_stop_before_intersection=2.0\n')

	def enterValet_park(self, ctx: UserIntercationParser.Valet_parkContext):
		park_id = ctx.String().getText()
		with open("once.txt", 'w') as f:
			f.write('real:trj:valet_park=1.0\n')
			f.write('real:park_id=' + park_id + '\n')
	
	def enterOpen_space_launch(self, ctx: UserIntercationParser.Open_space_launchContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:open_space_launch=1.0\n')

	def enterPull_over(self, ctx: UserIntercationParser.Pull_overContext):
		l_r = ctx.Left_Right().getText()
		with open("once.txt", 'w') as f:
			f.write('real:trj:pull_over=1.0\n')
			#f.write('real:scenario:pull_over:l_r=' + l_r + '\n')
	
	def enterEmergency_pull_over(self, ctx: UserIntercationParser.Emergency_pull_overContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:emergency_pull_over=1.0\n')
	
	def enterEmergency_stop(self, ctx: UserIntercationParser.Emergency_stopContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:emergency_stop=1.0\n')

	def enterVehicle_launch(self, ctx: UserIntercationParser.Vehicle_launchContext):
		with open("once.txt", 'w') as f:
			f.write('real:trj:vehicle_launch=1.0\n')
	
	def enterKeep_current_speed(self, ctx: UserIntercationParser.Keep_current_speedContext):
		with open("once.txt", 'w') as f:
			f.write('real:speed:Keep_speed=1.0\n')
			f.write('real:speed:Flag=3.0\n')
			if ("WITHOUT CHECK" in  ctx.getText()) or ("without check" in ctx.getText()):
				f.write('real:speed:without_check=1.0\n')
	
	def enterKeep_speed(self, ctx: UserIntercationParser.Keep_speedContext):
		speed = ctx.Num().getText()
		speed = str(round(float(speed) / 3.6, 2))
		with open("once.txt", 'w') as f:
			f.write('real:speed:Speed=' + speed + '\n')
			f.write('real:speed:Keep_speed=1.0\n')
			f.write('real:speed:Flag=3.0\n')
			if ("WITHOUT CHECK" in  ctx.getText()) or ("without check" in ctx.getText()):
				f.write('real:speed:Without_check=1.0\n')
	
	def enterAccelerate_decelerate(self, ctx: UserIntercationParser.Accelerate_decelerateContext):
		speed = ctx.Num().getText()
		speed = str(round(float(speed) / 3.6, 2))
		with open("once.txt", 'w') as f:
			f.write('real:speed:Speed=' + speed + '\n')
			f.write('real:speed:Flag=3.0\n')
			if ("WITHOUT CHECK" in  ctx.getText()) or ("without check" in ctx.getText()):
				f.write('real:speed:Without_check=1.0\n')
	
	def enterAccelerate_decelerate_with(self, ctx: UserIntercationParser.Accelerate_decelerate_withContext):
		speed = ctx.Num()[0].getText()
		speed = str(round(float(speed) / 3.6, 2))
		acceleration = ctx.Num()[1].getText()
		with open("once.txt", 'w') as f:
			f.write('real:speed:Speed=' + speed + '\n')
			f.write('real:speed:Flag=3.0\n')
			if ("ACCLERATE" in  ctx.getText()) or ("acclerate" in ctx.getText()):
				f.write('real:speed:Acceleration=' + acceleration + '\n')
			elif ("DECELERATE" in  ctx.getText()) or ("decelerate" in ctx.getText()):
				f.write('real:speed:Acceleration=-' + acceleration + '\n')
			if ("WITHOUT CHECK" in  ctx.getText()) or ("without check" in ctx.getText()):
				f.write('real:speed:Without_check=1.0\n')

	def enterTurn_on_off_light(self, ctx: UserIntercationParser.Turn_on_off_lightContext):
		with open("once.txt", 'w') as f:
			if ("ON" in  ctx.getText()) or ("on" in ctx.getText()):
				for light in ctx.Light_list():
					f.write('real:stutes:' + light.getText() + '=1.0\n')
			elif ("OFF" in  ctx.getText()) or ("off" in ctx.getText()):
				for light in ctx.Light_list():
					f.write('real:stutes:' + light.getText() + '=2.0\n')

	def enterSwitch_light(self, ctx: UserIntercationParser.Switch_lightContext):
		with open("once.txt", 'w') as f:
			f.write('real:stutes:switch_light' + '=1.0\n')
	
	def enterHonk_horn(self, ctx: UserIntercationParser.Honk_hornContext):
		with open("once.txt", 'w') as f:
			f.write('real:stutes:honk=1.0\n')

	def enterClearrealtimesetting(self, ctx: UserIntercationParser.ClearrealtimesettingContext):
		with open("once.txt", 'w') as f:
			f.write('real:clear\n')

	def enterClearportsetting(self, ctx: UserIntercationParser.ClearportsettingContext):
		port_name = ctx.getText().split(' ')[1]
		with open("once.txt", 'w') as f:
			f.write('real:clear:' + port_name + '\n')

	def enterCleartrafficsignalroadsetting(self, ctx: UserIntercationParser.CleartrafficsignalroadsettingContext):
		port_name = ctx.getText().split(' ')[1]
		with open("once.txt", 'w') as f:
			f.write('real:clear:scenario:' + port_name + '\n')

	def exitRealtimecommand(self, ctx: UserIntercationParser.RealtimecommandContext):
		self.real_command = copy.deepcopy({})