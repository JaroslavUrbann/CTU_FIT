### Planning
In this project, I implemented a planning task using the Planning Domain Definition Language.

- __problem.pddl__ is the problem definition
- __default.pddl__ is the default domain
- __default_plan.sas__ is the plan for the default domain
- __all_goals.pddl__ is the domain file where all goals need to be completed to make the sailor happy
- __all_goals_plan.sas__ is the plan for that particular domain

Pddl files can be run using the [Fast Downward](http://www.fast-downward.org/) planner:
1. First run `./build.py` after downloading
2. Then you can run either of the domains using: \
	`./fast-downward.py default.pddl problem.pddl --search "astar(lmcut())"` \
	or \
	`./fast-downward.py all_goals.pddl problem.pddl --search "astar(lmcut())"`