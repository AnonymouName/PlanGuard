# From Rules to Road: Executable Safety Policies within the Autonomous Vehicle Planner


## PlanGuard
- Before using this project, please securely set up and build the Apollo 9.0 environment according to [the official Apollo system instructions](https://github.com/ApolloAuto/apollo).
- Other Prerequisites：
  - ANTLR4：Make sure installation of version [antlr-4.8](https://www.antlr.org/download/antlr-4.8-complete.jar) (the latest version is not supported).
  - RTAMT：Please refer to [the github page](https://github.com/nickovic/rtamt) for installation of RTAMT.
  - Python3
- Replace the original Apollo planning module with the planning directory provided in this repository, and recompile the system. The provided planning module implements a bridge between PlanGuard and Apollo.
- Language：The PlanGuard language implemented based on ANTLR4

## LLM-Assisted Rule Generation
- Function call.txt: The function call representation for the PlanGuard language.
- Grammer.txt: The EBNF grammar of the PlanGuard.
- Mannul Rules.txt: Driving rules manually crafted based on traffic laws.
- TalkWithLLM.py: Responsible for invoking LLM to generate driving rules.
- Post_dealwith_semantic.py: Responsible for using LLM to perform semantic correction on generated driving rules.
- Post_dealwith_syntax.py: Responsible for using LLM to perform grammatical correction on generated driving rules.
- utils.py: Some auxiliary functions, including extracting driving rules from LLM response content and converting driving rules represented by Function calls back into $\mu$Drive language.

## LawBreaker
- LawBreak: The traffic-condition violation detection code is adapted from [LawBreaker](https://github.com/lawbreaker2022/LawBreaker-SourceCode) and refactored to support Apollo 9.0. As FixDrive is built upon the same LawBreaker framework, relevant details can also be found in the [FixDrive project](https://github.com/lawbreaker2022/FixDrive2025).

## Related Scenarios and Results
- Scenario: All guideline driving scenarios employed in the experiment, comprising 10 traffic violation scenarios from LawBreaker and FixDrive and official scenarios provided by Apollo.
- Specs: Including the traffic laws and driving manuals applied in the experiment.
- Result：
  - Analysis.xlsx: Including documented records of the reasonableness of the generated rules.
  - CompleteGuide、EssentialManual、SafeDriving、Traffic Law: Including the generated driving rules.
  - Results from secenarios involving traffic law violations: Includes execution records and videos for the 10 safety violation scenarios provided by PlanGuard.
  - **Noted**: The execution logs of the official scenarios are too large to include, but can be obtained by contacting the authors.
 
## Performance evaluation on ten traffic violation scenarios
- S1:The AV entered the intersection during a green light but failed to yield to the straight-moving vehicles, resulting in an accident.
  Apollo result：

https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc

  PlanGuard result：
    
https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36

- S2:The AV started and entered the intersection when the traffic light was yellow.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/f6671ff9-178f-43e0-a915-77e7471c5961)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/24b6d5dc-e61f-46f5-832d-27da17b9f671)

- S3:The AV entered the intersection on a yellow light.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/36fafb58-349b-408f-b7eb-2cd760293b43)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36
](https://github.com/wkyml/UDriver/assets/82079546/30f711f2-9fea-4029-afff-6323e84b72c1)
- S4:The AV continued to accelerate and rushed into the intersection on a yellow light, subsequently passed through the intersection on a red light.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/76635633-de1c-41f7-ab06-610177462997)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/d930c06e-33ca-4096-af22-c833d88050a9)

- S5:The AV entered the intersection on a red light.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/30926569-468c-4621-a7a0-47ea03cf7988)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/45161d3b-4730-4653-a5d2-7ce762e3f506)

- S6:The AV is traveling in the fast lane but is not maintaining the required speed limit for the fast lane.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/9b2dbd40-9c27-4fe7-8929-a189ab543f04)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/0f999b84-16bf-4901-a746-f9bc6e4103fc)

- S7:The AV is traveling in the fast lane and come to a stop due to an static obstacle (failure to change lanes to an available lane on the right), ultimately failing to reach its destination.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc
](https://github.com/wkyml/UDriver/assets/82079546/3a4717a1-81ca-4e79-b20f-f911bf03110e)
  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/1f8f8be3-da99-4569-8dd2-56d887a4fcf7)

- S8:The AV continues to travel at speeds exceeding 30 kilometers per hour despite fog, rain, snow, dust storms, and hail.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/61bd2e01-7f13-4827-88d8-2b75ec19833c)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36
](https://github.com/wkyml/UDriver/assets/82079546/c616fc88-f470-460e-9f5f-63682aa06943)
- S9:The AV fail to yield to the oncoming straight-through traffic at the stop sign and proceed to make a left turn at the intersection, resulting in an accident.
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/a6e51f3c-1882-4b94-bb46-1e7cb120b48a)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36](https://github.com/wkyml/UDriver/assets/82079546/ae3e85dd-3555-481a-9be5-6f6983e46952)

- S10:The AV chooses to enter the intersection on a green light despite congestion (6 vehicles present at the intersection).
  Apollo result：

[https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc](https://github.com/wkyml/UDriver/assets/82079546/a1c733cd-0140-4881-b3ba-e916db072f5d)

  PlanGuard result：
    
[https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36
](https://github.com/wkyml/UDriver/assets/82079546/3c541675-5139-4885-9c70-9be937f85317)
