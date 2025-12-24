/******************************************************************************
 * Copyright 2019 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file
 **/

#include "modules/planning/scenarios/traffic_light_unprotected_left_turn/stage_approach.h"

#include "modules/common_msgs/perception_msgs/perception_obstacle.pb.h"
#include "modules/common_msgs/perception_msgs/traffic_light_detection.pb.h"
#include "cyber/common/log.h"
#include "cyber/time/clock.h"
#include "modules/common/vehicle_state/vehicle_state_provider.h"
#include "modules/planning/planning_base/common/frame.h"
#include "modules/planning/planning_base/common/planning_context.h"
#include "modules/planning/planning_base/common/speed_profile_generator.h"
#include "modules/planning/scenarios/traffic_light_unprotected_left_turn/context.h"

namespace apollo {
namespace planning {

using apollo::common::TrajectoryPoint;
using apollo::cyber::Clock;
using apollo::hdmap::PathOverlap;
using apollo::perception::TrafficLight;

StageResult TrafficLightUnprotectedLeftTurnStageApproach::Process(
    const TrajectoryPoint& planning_init_point, Frame* frame) {
  ADEBUG << "stage: Approach";
  CHECK_NOTNULL(frame);
  CHECK_NOTNULL(context_);

  auto context = GetContextAs<TrafficLightUnprotectedLeftTurnContext>();
  const ScenarioTrafficLightUnprotectedLeftTurnConfig& scenario_config =
      context->scenario_config;
  if (!pipeline_config_.enabled()) {
    //wk
    {
      std::lock_guard<std::mutex> lock(user_intercation_mutex_);
      frame->GetUserintercation()->Set_value("real", "trj", "", "vehicle_launch", 0.0);
      frame->GetUserintercation()->Set_value("real", "trj", "", "whether_stop_before_intersection", 0.0);
    }
    return FinishStage(frame);
  }

  // set cruise_speed to slow down
  //wk
  const double approach_cruise_speed = (frame->GetUserintercation()->Get_value("scenario", "intersection", "Expected_speed") > 0.0)?
                                      frame->GetUserintercation()->Get_value("scenario", "intersection", "Expected_speed") :
                                      scenario_config.approach_cruise_speed();
  frame->mutable_reference_line_info()->front().LimitCruiseSpeed(approach_cruise_speed);
      //scenario_config.approach_cruise_speed());

  StageResult result = ExecuteTaskOnReferenceLine(planning_init_point, frame);
  if (result.HasError()) {
    AERROR << "TrafficLightUnprotectedLeftTurnStageApproach planning error";
  }

  if (context->current_traffic_light_overlap_ids.empty()) {
    //wk
    {
      std::lock_guard<std::mutex> lock(user_intercation_mutex_);
      frame->GetUserintercation()->Set_value("real", "trj", "", "vehicle_launch", 0.0);
      frame->GetUserintercation()->Set_value("real", "trj", "", "whether_stop_before_intersection", 0.0);
    }
    return FinishScenario();
  }

  const auto& reference_line_info = frame->reference_line_info().back();

  const double adc_front_edge_s = reference_line_info.AdcSlBoundary().end_s();

  PathOverlap* traffic_light = nullptr;
  bool traffic_light_all_done = true;
  for (const auto& traffic_light_overlap_id :
       context->current_traffic_light_overlap_ids) {
    // get overlap along reference line
    PathOverlap* current_traffic_light_overlap =
        reference_line_info.GetOverlapOnReferenceLine(
            traffic_light_overlap_id, ReferenceLineInfo::SIGNAL);
    if (!current_traffic_light_overlap) {
      continue;
    }

    traffic_light = current_traffic_light_overlap;

    // set right_of_way_status
    reference_line_info.SetJunctionRightOfWay(
        current_traffic_light_overlap->start_s, false);

    const double distance_adc_to_stop_line =
        current_traffic_light_overlap->start_s - adc_front_edge_s;
    auto signal_color = frame->GetSignal(traffic_light_overlap_id).color();
    ADEBUG << "traffic_light_overlap_id[" << traffic_light_overlap_id
           << "] start_s[" << current_traffic_light_overlap->start_s
           << "] distance_adc_to_stop_line[" << distance_adc_to_stop_line
           << "] color[" << signal_color << "]";

    //wk
    if (distance_adc_to_stop_line < 0 && FLAGS_wait != 1.0) { 
          {
            std::lock_guard<std::mutex> lock(user_intercation_mutex_);
            frame->GetUserintercation()->Set_value("real", "trj", "", "vehicle_launch", 0.0);
            frame->GetUserintercation()->Set_value("real", "trj", "", "whether_stop_before_intersection", 0.0);
          }
          return FinishStage(frame);
        }
    // check on traffic light color and distance to stop line

    //wk
    const double stop_distance = (frame->GetUserintercation()->Get_value("scenario", "intersection", "Stopping_distance") > 0.0)?
                                frame->GetUserintercation()->Get_value("scenario", "intersection", "Stopping_distance") :
                                scenario_config.max_valid_stop_distance();

    if (distance_adc_to_stop_line >= stop_distance){
      traffic_light_all_done = false;
      break;
    }

    if (signal_color != TrafficLight::GREEN && 
        frame->GetUserintercation()->Get_value("trj", "", "whether_stop_before_intersection") != 2.0 &&
        frame->GetUserintercation()->Get_value("trj", "", "vehicle_launch") != 1.0){
      traffic_light_all_done = false;
      break;
    }
  }

  if (traffic_light == nullptr) {
    //wk
    {
      std::lock_guard<std::mutex> lock(user_intercation_mutex_);
      frame->GetUserintercation()->Set_value("real", "trj", "", "vehicle_launch", 0.0);
      frame->GetUserintercation()->Set_value("real", "trj", "", "whether_stop_before_intersection", 0.0);
    }
    return FinishScenario();
  }

  if (traffic_light_all_done && FLAGS_wait != 1.0) {
    //wk
    {
      std::lock_guard<std::mutex> lock(user_intercation_mutex_);
      frame->GetUserintercation()->Set_value("real", "trj", "", "vehicle_launch", 0.0);
      frame->GetUserintercation()->Set_value("real", "trj", "", "whether_stop_before_intersection", 0.0);
    }
    return FinishStage(frame);
  }

  return result.SetStageStatus(StageStatusType::RUNNING);
}

StageResult TrafficLightUnprotectedLeftTurnStageApproach::FinishStage(
    Frame* frame) {
  auto context = GetContextAs<TrafficLightUnprotectedLeftTurnContext>();
  const ScenarioTrafficLightUnprotectedLeftTurnConfig& scenario_config =
      context->scenario_config;
  // check speed at stop_stage
  const double adc_speed = injector_->vehicle_state()->linear_velocity();
  //wk
  if ((adc_speed > scenario_config.max_adc_speed_before_creep() && 
      frame->GetUserintercation()->Get_value("scenario", "intersection", "Whether_creep") == 0.0) || 
      frame->GetUserintercation()->Get_value("scenario", "intersection", "Whether_creep") == 2.0) {
    // skip creep
    next_stage_ = "TRAFFIC_LIGHT_UNPROTECTED_LEFT_TURN_INTERSECTION_CRUISE";
  } else {
    // creep
    // update PlanningContext
    injector_->planning_context()
        ->mutable_planning_status()
        ->mutable_traffic_light()
        ->mutable_done_traffic_light_overlap_id()
        ->Clear();
    for (const auto& traffic_light_overlap_id :
         context->current_traffic_light_overlap_ids) {
      injector_->planning_context()
          ->mutable_planning_status()
          ->mutable_traffic_light()
          ->add_done_traffic_light_overlap_id(traffic_light_overlap_id);
    }

    context->creep_start_time = Clock::NowInSeconds();
    next_stage_ = "TRAFFIC_LIGHT_UNPROTECTED_LEFT_TURN_CREEP";
  }

  // reset cruise_speed
  auto& reference_line_info = frame->mutable_reference_line_info()->front();
  reference_line_info.LimitCruiseSpeed(FLAGS_default_cruise_speed);

  return StageResult(StageStatusType::FINISHED);
}

}  // namespace planning
}  // namespace apollo
