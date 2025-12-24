#include "modules/planning/planning_base/common/user_intercation.h"

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "cyber/common/file.h"

namespace apollo {

namespace planning {

bool UserIntercation::Init() {
    changelane_pra.insert(changelane_pra.end(), changelane_pra_pattern.begin(), changelane_pra_pattern.end());
    laneborrow_pra.insert(laneborrow_pra.end(), laneborrow_pra_pattern.begin(), laneborrow_pra_pattern.end());
    return (Init_pattern() && Init_command() && Update_pattern() && Init_real_yueshu() && Init_weather());
}

bool UserIntercation::Init_pattern() {
    for (const std::string& sce : sce_type) {
        if (!Init_port("pattern", "scenario", sce)) {
            //AINFO << "ppppppppppppppppppp" << sce;
            return false;
        }
    }
    return ( Init_port("pattern", "preference") && Init_port("pattern", "obstacle") && Init_port("pattern", "constraint"));
}

bool UserIntercation::Init_command(){
    for(const std::string& sce : sce_type) {
        if (!Init_port("real", "scenario", sce)){
            //AINFO << "rrrrrrrrrrrrrrrr" << sce;
            return false;
        }
    }
    return (Init_port("real", "preference") &&
            Init_port("real", "constraint") &&
            Init_port("real", "obstacle") &&
            Init_port("real", "trj") &&
            Init_port("real", "speed") &&
            Init_port("real", "status"));
}

bool UserIntercation::Init_command_port(){
    for(const std::string& sce : sce_type) {
        if (!Init_port("real", "scenario", sce)){
            //AINFO << "rrrrrrrrrrrrrrrr" << sce;
            return false;
        }
    }
    return (Init_port("real", "preference") &&
            Init_port("real", "constraint") &&
            Init_port("real", "obstacle"));
}


bool UserIntercation::Init_port(std::string s1, std::string s2){
    if (s1 == "pattern"){
        if (s2 == "preference") {
            for (const std::string& par : pre_pra){
                pattern.preference[par] = 0.0;
            }
            return true;
        } else if (s2 == "obstacle"){
            for (const std::string& par : obs_pra){
                pattern.obstacle[par] = 0.0;
            }
            return true;
        } else if (s2 == "constraint"){
            for (const std::string& par : con_pra){
                pattern.constraint[par] = 0.0;
            }
            return true;
        }
    } else if (s1 == "real") {
        if (s2 == "preference") {
            for (const std::string& par : pre_pra){
                real_time_command.preference[par] = 0.0;
            }
            return true;
        } else if (s2 == "obstacle"){
            for (const std::string& par : obs_pra){
               real_time_command.obstacle[par] = 0.0;
            }
            return true;
        } else if (s2 == "trj"){
            for (const std::string& par : trj_pra){
                real_time_command.trj[par] = 0.0;
            }
            return true;
        } else if (s2 == "speed"){
            for (const std::string& par : speed_pra){
                real_time_command.speed[par] = 0.0;
            }
            return true;
        } else if (s2 == "status"){
            for (const std::string& par : status_pra){
                real_time_command.status[par] = 0.0;
            }
            return true;
        } else if (s2 == "constraint"){
            for (const std::string& par : con_pra){
                real_time_command.constraint[par] = 0.0;
            }
            return true;
        }
    }
    return false;
}

bool UserIntercation::Init_port(std::string s1, std::string s2, std::string s3){
    if (s1 == "pattern"){
        if (s3 == "change_lane"){
            for (const std::string& par : changelane_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "lane_follow"){
            for (const std::string& par : keeplane_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "borrow_lane"){
            for (const std::string& par : laneborrow_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "keep_clear_signal"){
            for (const std::string& par : keepclear_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "crosswalk_signal"){
            for (const std::string& par : crosswalk_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "reach_destination"){
            for (const std::string& par : destination_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "stop_signal"){
            for (const std::string& par : stopsign_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "traffic_light_signal"){
            for (const std::string& par : trafficlight_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "yield_signal"){
            for (const std::string& par : yieldsign_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "emergency_pull_over"){
            for (const std::string& par : emergencypull_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "emergency_stop"){
            for (const std::string& par : emergencystop_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "open_space_launch"){
            for (const std::string& par : vehiclelaunchinopenspace_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "valet_park"){
            for (const std::string& par : valetpark_pra_pattern) {
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "intersection"){
            for (const std::string& par : intersection_pra_pattern){
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "pull_over"){
            for (const std::string& par : pull_pra_pattern){
                pattern.scenario[s3][par] = 0.0;
            }
            return true;
        }
    } else if (s1 == "real") {
        if (s3 == "change_lane"){
            for (const std::string& par : changelane_pra) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "lane_follow"){
            for (const std::string& par : keeplane_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "borrow_lane"){
            for (const std::string& par : laneborrow_pra) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "keep_clear_signal"){
            for (const std::string& par : keepclear_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "crosswalk_signal"){
            for (const std::string& par : crosswalk_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "reach_destination"){
            for (const std::string& par : destination_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "stop_signal"){
            for (const std::string& par : stopsign_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "traffic_light_signal"){
            for (const std::string& par : trafficlight_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "yield_signal"){
            for (const std::string& par : yieldsign_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "emergency_pull_over"){
            for (const std::string& par : emergencypull_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "emergency_stop"){
            for (const std::string& par : emergencystop_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "open_space_launch"){
            for (const std::string& par : vehiclelaunchinopenspace_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "valet_park"){
            for (const std::string& par : valetpark_pra_pattern) {
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "intersection"){
            for (const std::string& par : intersection_pra_pattern){
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
        if (s3 == "pull_over"){
            for (const std::string& par : pull_pra_pattern){
                real_time_command.scenario[s3][par] = 0.0;
            }
            return true;
        }
    }
    return false;
}

bool UserIntercation::Init_real_yueshu(){
    for (const std::string& par : yueshu_pra){
        real_yueshu[par] = 0.0;
    }
    return true;
}

bool UserIntercation::Init_weather(){
    for (const std::string& par : weather_pra){
        weather[par] = 0.0;
    }
    return true;
}

void UserIntercation::Proc() {
    if (Check_exit("real")) {
        if (!Read_real()) {
            AINFO << "Read real time action failed!";
        } else{
            record_ys();
        }
    }
    if (Check_exit("weather")) {
        if (!Read_weather()) {
            AINFO << "Update weather failed!";
        }
    }
}

bool UserIntercation::Check_exit(std::string name) {
    if (name == "pattern"){
        return (std::system(("test -e " + pattern_filename).c_str()) == 0);
    } else if (name == "real") {
        return (std::system(("test -e " + real_filename).c_str()) == 0);
    } else if (name == "weather") {
        return (std::system(("test -e " + weather_filename).c_str()) == 0);
    }
    return false;
}

bool UserIntercation::Update_pattern(){
    //default value
    if (Set_value("pattern", "preference", "", "Max_planning_speed", 20.00) && 
    Set_value("pattern", "preference", "", "Near_stop_speed", 0.5) &&
    Set_value("pattern", "preference", "", "Default_cruise_speed", 11.18) &&
    Set_value("pattern", "preference", "", "Whether_priority_change_lane", 1.0) &&
    Set_value("pattern", "constraint", "", "Trajectory_min_speed", -0.1) &&
    Set_value("pattern", "constraint", "", "Trajectory_max_speed", 40.0) &&
    Set_value("pattern", "constraint", "", "Trajectory_min_longitudinal_acceleration", -6.0) &&
    Set_value("pattern", "constraint", "", "Trajectory_max_longitudinal_acceleration", 4.0) &&
    Set_value("pattern", "constraint", "", "Trajectory_max_lateral_acceleration", 4.0) &&
    Set_value("pattern", "constraint", "", "Whether_trajectory_check", 2.0) &&
    Set_value("pattern", "obstacle", "", "Lateral_buffer_distance", 0.4) &&
    Set_value("pattern", "obstacle", "", "Forward_buffer_distance", 3.0) &&
    Set_value("pattern", "obstacle", "", "Backward_buffer_distance", 2.0) &&
    Set_value("pattern", "obstacle", "", "Dynamic_Obstacle_Follow_distance", 3.0) &&
    Set_value("pattern", "obstacle", "", "Dynamic_Obstacle_Yield_distance", 5.0) &&
    Set_value("pattern", "obstacle", "", "Min_stop_distance", 6.0)){
        return true;
    }
    return false;
}

bool UserIntercation::Read_real(){
    std::ifstream ifs;
    ifs.open(real_filename, std::ios::in);
    std::string str;
    std::string key_name;
    std::string key;
    Rule rules;
    while (getline(ifs, str)){
        int pos = str.find("}");
        if (pos != -1) {
            if (!rules.xingwei.empty()) {
                if (key_name == "always") {
                    always_rule.push_back(rules);
                } else {
                    rule[key] = rules;
                }
                rules.xingwei.clear();
                rules.yueshu.clear();
            }
            key_name.clear();
            key.clear();
            continue;
        }
        
        int pos1 = str.find("[");
        int pos2 = str.find("]");
        if (pos1 != -1 && pos2 != -1){
            key_name = str.substr(pos1+1, pos2-pos1+1);
            int poss = key_name.find(":");
            if (poss != -1){
                key = key_name.substr(poss+1);
                key_name = "always";
            } else {
                key = key_name;
            }
            std::istringstream iss(key);
            std::vector<std::string> tokens;
            while (std::getline(iss, key, '&')) {
                tokens.push_back(key);
            }
            for (const auto& yueshu : tokens){
                int pos_ = yueshu.find("(");
                int pos_1 = yueshu.find(")");
                std::string yueshuname = yueshu.substr(0, pos_);
                double value = std::stod(yueshu.substr(pos_+1, pos_1 - pos_ + 1));
                rules.yueshu[yueshuname] = value;
            }
        } else {
            int pos__ = str.find("=");
            if (pos__ != -1){
                rules.xingwei.push_back(str);
            }
        }
    }
    //     int pos = str.find("[");
    //     int pos1 = str.find("]");
    //     if (pos != -1 && pos1 != -1){
    //         if (key_name == "always"){
    //             if (!rules.xingwei.empty()){
    //                 always_rule.push_back(rules);
    //             }
    //         } else {
    //             if (!rules.xingwei.empty()){
    //                 rule[key] = rules;
    //             }
    //         }
    //         rules.xingwei.clear();
    //         rules.yueshu.clear();
    //         key_name.clear();
    //         key.clear();
    //         key_name = str.substr(pos+1, pos1-pos+1);
    //         int poss = key_name.find(":");
    //         if (poss != -1){
    //             key = key_name.substr(poss+1);
    //             key_name = "always";
    //         } else{
    //             key = key_name;
    //         }
    //         std::istringstream iss(key);
    //         std::vector<std::string> tokens;
    //         while (std::getline(iss, key, '&')) {
    //             tokens.push_back(key);
    //         }
    //         for (const auto& yueshu : tokens){
    //             int pos_ = yueshu.find("(");
    //             int pos_1 = yueshu.find(")");
    //             std::string yueshuname = yueshu.substr(0, pos_);
    //             double value = std::stod(yueshu.substr(pos_+1, pos_1 - pos_ + 1));
    //             rules.yueshu[yueshuname] = value;
    //         }
    //     } else {
    //         int pos = str.find("=");
    //         if (pos != -1){
    //             rules.xingwei.push_back(str);
    //         }
    //     }
    // }

    // if (key_name == "always" && !rules.xingwei.empty()){
    //     always_rule.push_back(rules);
    // } else {
    //     if (!rules.xingwei.empty()) {
    //         rule[key] = rules;
    //     }
    // }
    // key_name.clear();
    // key.clear();
    // rules.yueshu.clear();
    // rules.xingwei.clear();
    return (std::remove(real_filename.c_str()) == 0);
}

bool UserIntercation::Read_weather(){
    std::ifstream ifs;
    ifs.open(weather_filename, std::ios::in);
    std::string str;
    while (getline(ifs, str)){
        int pos = str.find("=");
        if (pos != -1) {
            std::string key = str.substr(0, pos-1);
            double value = std::stod(str.substr(pos+1));
            Set_weather(key, value);
        }
    }
    return true;
}

void UserIntercation::Update_real(){
    Init_command();
    bool flag;
    for (const Rule& rule_p : always_rule) {
        flag = true;
        std::map<std::string, double> yueshu = rule_p.yueshu;
        std::vector<std::string> xingwei = rule_p.xingwei;
        for (const auto& ysp : yueshu){
            std::string name = ysp.first;
            double value = ysp.second;
            int pos = name.find("L");
            int pos1 = name.find("H");
            if (pos!=-1 && real_yueshu[name.substr(0,pos)] > value){
                flag = false;
                break;
            }
            if (pos1!=-1 && real_yueshu[name.substr(0,pos1)] < value){
                flag = false;
                break;
            }
            if (pos == -1 && pos1 == -1&& real_yueshu[name] != value){
                flag = false;
                break;
            }
        }
        if (!flag){
            continue;
        }
        for (const std::string& str : xingwei){
            int pos = str.find("=");
            std::string key = str.substr(0, pos);
            std::istringstream iss(key);
            std::vector<std::string> tokens;
            while (std::getline(iss, key, ':')) {
                tokens.push_back(key);
            }
            if (tokens.size() == 2) {
                std::string value = str.substr(pos + 1);
                if (!Set_value("real", tokens[1], value)) {
                    return;
                }
            } else if (tokens.size() == 3) {
                double value = std::stod(str.substr(pos + 1).c_str());
                if (!Set_value("real", tokens[1], "", tokens[2], value)) {
                    return;
                }
            } else if (tokens.size() == 4) {
                double value = std::stod(str.substr(pos + 1).c_str());
                if (!Set_value("real", tokens[1], tokens[2], tokens[3], value)) {
                    return;
                }
            }
        }
    }

    for (const auto& rule_p : rule) {
        flag = true;
        std::map<std::string, double> yueshu = rule_p.second.yueshu;
        std::vector<std::string> xingwei = rule_p.second.xingwei;
        for (const auto& ysp : yueshu){
            AINFO << "wk:yueshu:" << ysp.first << "-" << ysp.second;
            std::string name = ysp.first;
            double value = ysp.second;
            int pos = name.find("L");
            int pos1 = name.find("H");
            if (pos!=-1 && real_yueshu[name.substr(0,pos)] > value){
                flag = false;
                break;
            }
            if (pos1!=-1 && real_yueshu[name.substr(0,pos1)] < value){
                flag = false;
                break;
            }
            if (pos == -1 && pos1 == -1&& real_yueshu[name] != value){
                flag = false;
                break;
            }
        }
        
        AINFO << "wk:whether_set:" << flag;
        if (!flag){
            continue;
        }
        for (const std::string& str : xingwei){
            int pos = str.find("=");
            std::string key = str.substr(0, pos);
            std::istringstream iss(key);
            std::vector<std::string> tokens;
            while (std::getline(iss, key, ':')) {
                tokens.push_back(key);
            }
            if (tokens.size() == 2) {
                std::string value = str.substr(pos + 1);
                if (!Set_value("real", tokens[1], value)) {
                    return;
                }
            } else if (tokens.size() == 3) {
                double value = std::stod(str.substr(pos + 1).c_str());
                if (!Set_value("real", tokens[1], "", tokens[2], value)) {
                    return;
                }
            } else if (tokens.size() == 4) {
                double value = std::stod(str.substr(pos + 1).c_str());
                if (!Set_value("real", tokens[1], tokens[2], tokens[3], value)) {
                    return;
                }
            }
        }
    }

    AINFO << "wk:speed1:" << real_time_command.speed["Speed"];
    AINFO << "wk:acc1:" << real_time_command.speed["Acceleration"];

    AINFO << "wk:sy_trafficlight_color:" << real_yueshu["trafficlight_color"];
    AINFO << "wk:sy_traffic_light_dis:" << real_yueshu["traffic_light_dis"];
    AINFO << "wk:trace_s_stop:" << real_time_command.trj["emergency_stop"];


    Init_real_yueshu();
    return;
}


bool UserIntercation::Check_trj(){
    for (const auto& pair : real_time_command.trj) {
        if (pair.second != 0.0) {
            return false;
        }
    }
    return true;
}

bool UserIntercation::Set_value(std::string s1, std::string s2, std::string s3, std::string s4, double value) {
   if (s1 == "pattern"){
    if (s2 == "preference") {
        pattern.preference[s4] = value;
        return true;
    } else if (s2 == "scenario") {
        pattern.scenario[s3][s4] = value;
        return true;
    } else if (s2 == "obstacle") {
        pattern.obstacle[s4] = value;
        return true;
    } else if (s2 == "constraint") {
        pattern.constraint[s4] = value;
        return true;
    }
    return false;
   } else if (s1 == "real") {
    if (s2 == "preference") {
        real_time_command.preference[s4] = value;
        return true;
    } else if (s2 == "scenario") {
        //AINFO << "XXXXXXXXXXXXXXXXXXX" << real_time_command.scenario[s3][s4];
        real_time_command.scenario[s3][s4] = value;
        return true;
    } else if (s2 == "obstacle") {
        real_time_command.obstacle[s4] = value;
        return true;
    } else if (s2 == "trj") {
        real_time_command.trj[s4] = value;
        return true;
    } else if (s2 == "speed") {
        real_time_command.speed[s4] = value;
        return true;
    } else if (s2 == "status") {
        real_time_command.status[s4] = value;
        return true;
    } else if (s2 == "constraint") {
        real_time_command.constraint[s4] = value;
    }
    return false;
   } else {
    //AINFO << "XXXXXXXXXXXXXXXXXXX" << s1 << s2 << s3 << s4;
    return false;
   }
}

bool UserIntercation::Set_value(std::string s1, std::string s2, std::string s3) {
    if (s2 == "park_id") {
        real_time_command.park_id = s3;
        return true;
    }
    return false;
}

bool UserIntercation::Set_yueshu(std::string s1, double s2){
    real_yueshu[s1] = s2;
    return true;
}

bool UserIntercation::Set_weather(std::string s1, double s2){
    weather[s1] = s2;
    return true;
}

const double UserIntercation::Get_value(std::string s1, std::string s2, std::string s3) {
    double value;
    if (s1 == "preference"){
        value = (real_time_command.preference[s3] > 0.0)? 
                            real_time_command.preference[s3] : 
                            ((pattern.preference[s3] > 0.0)? pattern.preference[s3] : 0.0); 
    } else if (s1 == "obstacle"){
        value = (real_time_command.obstacle[s3] > 0.0)? 
                            real_time_command.obstacle[s3] : 
                            ((pattern.obstacle[s3] > 0.0)? pattern.obstacle[s3] : 0.0);
    } else if (s1 == "trj"){
        value = real_time_command.trj[s3];
    //} else if (s1 == "status"){
    //    value = real_time_command.status[s3];
    //} else if (s1 == "park_id"){
        //value = real_time_command.park_id;
    } else if (s1 == "scenario"){
        value = real_time_command.scenario[s2][s3];
        if (value == 0.0) {
            try {
                value = pattern.scenario[s2][s3];
            } catch (std::exception& e) {}
        }
        if (s3 == "Expected_speed") {
            value = (real_time_command.speed["Speed"] > 0.0)? real_time_command.speed[s3] : value;
        }
        if (s3 == "Whether_stop" && s2 == "intersection"){
            value = (real_time_command.trj["whether_stop_before_intersection"] > 0.0)? real_time_command.trj["whether_stop_before_intersection"] : value;
        }
    } else if (s1 == "speed"){
        value = real_time_command.speed[s3];
    } else if (s1 == "constraint"){
        value = (real_time_command.constraint[s3] > 0.0)? 
                real_time_command.constraint[s3] :
                ((pattern.constraint[s3] > 0.0)? pattern.constraint[s3] : 0.0);
    }
    return value;
}

const double UserIntercation::Get_value(std::string s1, std::string s2, std::string s3, std::string s4){
    double value;
    if (s3 == "status") {
        value = (real_time_command.status[s4] > 0.0)? real_time_command.status[s4] :
            ((real_time_command.scenario[s2][s4] > 0.0) ? real_time_command.scenario[s2][s4] : 
            (pattern.scenario[s2][s4] > 0.0)? pattern.scenario[s2][s4] : 0.0);
    }
    return value;
}

const std::string UserIntercation::Get_ParkId(){
    const std::string park_id= real_time_command.park_id;
    return park_id;
}

void UserIntercation::record(){
    AINFO << "wk: record";
    for (const auto& port : real_yueshu){
        AINFO << port.first << port.second;
    }
}

const double UserIntercation::Get_weather(std::string s1){
    return weather[s1];
}

void UserIntercation::record_ys(){
    AINFO << "wk: record ys";
    for (const auto& rule_p : rule){
        std::map<std::string, double> yueshu = rule_p.second.yueshu;
        std::vector<std::string> xingwei = rule_p.second.xingwei;
        for (const auto& pair : yueshu){
            AINFO << pair.first <<" " << pair.second;
        }
        for (const auto& pair : xingwei){
            AINFO << pair;
        }
        AINFO << "NEXT";

    }
}

}
}