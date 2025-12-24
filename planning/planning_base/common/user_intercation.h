#pragma once

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>

namespace apollo {

namespace planning{

struct Rule{
    std::map<std::string, double> yueshu;
    std::vector<std::string> xingwei;
};

struct PatternPlan {
    std::map<std::string, double> preference;
    std::map<std::string, double> constraint;
    std::map<std::string, std::map<std::string, double>> scenario;
    std::map<std::string, double> obstacle;
};

struct RealPlan {
    std::map<std::string, double> preference;
    std::map<std::string, double> constraint;
    std::map<std::string, std::map<std::string, double>> scenario;
    std::map<std::string, double> obstacle;
    std::map<std::string, double> trj;
    std::map<std::string, double> speed;
    std::map<std::string, double> status;
    // std::map<std::string, double> weather;
    std::string park_id;
};


class UserIntercation {
    public:
        UserIntercation() = default;
        ~UserIntercation() = default;

        bool Init();
        bool Init_pattern();
        bool Init_command();
        bool Init_command_port();
        bool Init_port(std::string s1, std::string s2);
        bool Init_port(std::string s1, std::string s2, std::string s3);
        bool Init_real_yueshu();
        bool Init_weather();

        bool Check_exit(std::string name);
        bool Check_trj();

        void Proc();
        bool Update_pattern();
        bool Read_real();
        bool Read_weather();
        void Update_real();
        bool Set_value(std::string s1, std::string s2, std::string s3, std::string s4, double value);
        bool Set_value(std::string s1, std::string s2, std::string s3);
        bool Set_yueshu(std::string s1, double s2);
        bool Set_weather(std::string s1, double s2);
        void record();
        void record_ys();

        //void RefreshChangeLane();

        const double Get_value(std::string s1, std::string s2, std::string s3);
        const double Get_value(std::string s1, std::string s2, std::string s3, std::string s4);
        const double Get_weather(std::string s1);
        const std::string Get_ParkId();

        PatternPlan pattern;
        RealPlan real_time_command;
        std::vector<Rule> always_rule;
        std::map<std::string, Rule> rule;
        std::map<std::string, double> real_yueshu;
        std::map<std::string, double> weather;


        std::vector<std::string> pre_pra = {"Max_planning_speed", "Default_cruise_speed", "Near_stop_speed", "Whether_priority_change_lane",
                                            "Driving_side", "Rerouting_wait_time", "Near_stop_deceleration", "Whether_allow_borrow_lane"};
        std::vector<std::string> con_pra = {"Whether_trajectory_check", "Trajectory_max_speed", "Trajectory_min_speed",
                                            "Trajectory_max_longitudinal_acceleration", "Trajectory_min_longitudinal_acceleration"
                                            "Trajectory_max_lateral_acceleration"};
        std::vector<std::string> obs_pra = {"Forward_buffer_distance", "Backward_buffer_distance", "Lateral_buffer_distance",
                                            "Dynamic_Obstacle_Follow_distance", "Static_Obstacle_Deceleration_ratio",
                                            "Dynamic_Obstacle_Deceleration_ratio", "Whether_declearation", "Dynamic_Obstacle_Overtake_distance","Min_stop_distance"};
        std::vector<std::string> sce_type= {"change_lane", "lane_follow", "borrow_lane", "keep_clear_signal", "crosswalk_signal",
                                            "reach_destination", "stop_signal", "traffic_light_signal", "yield_signal", "emergency_pull_over",
                                            "emergency_stop", "open_space_launch", "valet_park", "intersection", "pull_over"};


        std::vector<std::string> changelane_pra = {"l_r", "times", "g_l_r", "distance", "l_r_most", "num"};
        std::vector<std::string> changelane_pra_pattern = {"Expected_speed", "Time_interval", "Whether_environment_check", 
                                                           "Check_distance", "Use_distance", "Preparation_distance"};
        std::vector<std::string> keeplane_pra_pattern = {"Expected_speed"};
        std::vector<std::string> laneborrow_pra = {"l_r"};
        std::vector<std::string> laneborrow_pra_pattern ={"Expected_speed", "Whether_speed_check", "Whether_environment_check", "Check_distance"};
        std::vector<std::string> keepclear_pra_pattern = {"Min_speed", "Whether_check"};
        std::vector<std::string> crosswalk_pra_pattern = {"Whether_check", "Wait_time", "Stopping_distance"};
        std::vector<std::string> destination_pra_pattern = {"Whether_pull_over", "Preparation_distance", "Stopping_distance"};
        std::vector<std::string> stopsign_pra_pattern = {"Whether_check", "Preparation_distance", "Wait_time", "Check_distance",
                                                        "Stopping_distance", "Creep_time"};
        std::vector<std::string> trafficlight_pra_pattern = {"Whether_check", "Stopping_distance", "Whether_red_light_turn_right"};
        std::vector<std::string> yieldsign_pra_pattern = {"Whether_check", "Preparation_distance", "Stopping_distance", "Creep_time"};
        std::vector<std::string> emergencypull_pra_pattern = {"Stopping_distance", "Expected_speed"};
        std::vector<std::string> pull_pra_pattern = {"Expected_speed", "Stopping_distance"};
        std::vector<std::string> emergencystop_pra_pattern = {"Stopping_distance", "Expected_speed"};
        std::vector<std::string> vehiclelaunchinopenspace_pra_pattern = {"Permissible_maximum_heading_diffence", "Permissible_max_steering_percentage"};
        std::vector<std::string> valetpark_pra_pattern = {"Preparation_distance", "Expected_speed", "Stopping_distance"};
        std::vector<std::string> intersection_pra_pattern = {"Preparation_distance", "Expected_speed", "Whether_stop", "Check_distance",
                                                            "Stopping_distance", "Whether_creep", "Creep_time"};


        std::vector<std::string> trj_pra = {"change_lane", "lane_follow", "borrow_lane", "follow_car", "yield_car", "overtake_car", "rerouting",
                                            "emergency_pull_over", "vehicle_launch", "emergency_stop", "open_space_launch", "valet_park", "whether_stop_before_intersection",
                                            "pull_over", "wait"};
        std::vector<std::string> speed_pra = {"Keep_speed", "Speed", "Without_check", "Acceleration", "Flag"};
        std::vector<std::string> status_pra = {"light"};

        std::vector<std::string> weather_pra = {"rain", "snow", "fog", "cloud"};

        std::vector<std::string> yueshu_pra = {"rain", "snow", "fog", "cloud", "egospeed", "egoacceleration", "is_lane", "is_fast_lane",
                                                "cross_dis", "stop_dis", "yield_dis", "keepclear_dis", "intersection_dis", "traffic_light_dis",
                                                "whether_trafficlight", "interaction_zwlx", "is_intercation", "is_change_lane", "is_borrow_lane",
                                                "trafficlight_color", "is_e_stop", "is_pull_over", "v_f_dis", "v_f_speed", "v_b_dis", "v_b_speed",
                                                "v_l_dis", "v_l_speed", "v_r_dis", "v_r_speed", "r_f_dis", "r_f_speed", "r_b_dis", "r_b_speed", "r_l_dis",
                                                "r_l_speed", "r_r_dis", "r_r_speed", "o_f_dis", "o_b_dis", "o_l_dis", "o_r_dis", "is_jam"};
        


        std::string pattern_filename = "/apollo_workspace/modules/planning/planning_base/pattern.txt";
        std::string real_filename = "/apollo_workspace/modules/planning/planning_base/once.txt";
        std::string weather_filename = "/apollo_workspace/modules/planning/planning_base/weather.txt";
};

}
}