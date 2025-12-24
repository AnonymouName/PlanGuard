# From Rules to Road: Executable Safety Policies within the Autonomous Vehicle Planner


## LLM-Assisted Rule Generation
- Function call.txt: The function call representation for the PlanGuard language.
- Grammer.txt: The EBNF grammar of the PlanGuard.
- Mannul Rules.txt: Driving rules manually crafted based on traffic laws.
- TalkWithLLM.py: Responsible for invoking LLM to generate driving rules.
- Post_dealwith_semantic.py: Responsible for using LLM to perform semantic correction on generated driving rules.
- Post_dealwith_syntax.py: Responsible for using LLM to perform grammatical correction on generated driving rules.
- utils.py: Some auxiliary functions, including extracting driving rules from LLM response content and converting driving rules represented by Function calls back into $\mu$Drive language.
- LawBreak: Code for detecting traffic condition violations in scenarios, sourced from LawBreak.

## Related Scenarios and Results
- Scenario: All guideline driving scenarios employed in the experiment, comprising 10 traffic violation scenarios from $\mu$Drive and official scenarios provided by Apollo.
- Specs: Including the traffic laws and driving manuals applied in the experiment.
- Result：
  - Analysis.xlsx: Including documented records of the reasonableness of the generated rules.
  - CompleteGuide、EssentialManual、SafeDriving、Traffic Law: Including the generated driving rules.
  - Results from secenarios involving traffic law violations: Includes execution records and videos for the 10 safety violation scenarios provided by PlanGuard.
  - **Noted**: The execution logs of the official scenarios are too large to include, but can be obtained by contacting the authors.
 
## Performance evaluation on ten traffic violation scenarios
- S1:The AV entered the intersection during a green light but failed to yield to the straight-moving vehicles, resulting in an accident.
  - Apollo result：https://github.com/wkyml/UDriver/assets/82079546/d285c255-43c7-4556-b950-5323349a18fc
  - PlanGuard result：https://github.com/wkyml/UDriver/assets/82079546/71aa9885-1e71-4c0b-983d-8663cd47bd36 
- 
