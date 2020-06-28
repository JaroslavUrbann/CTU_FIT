(define (problem planning)
	(:domain planning)
	(:requirements :strips :typing)
	(:objects
		wood flowers map gold_nugget gold_coin gold_brick alcohol coconuts bear_fur pearl cocain ring - item
		island sea lighthouse harbor river forest pub city academia - place
		boat frigate caravel - ship
		honest_people shady_people smugglers - friends
		drunk tough felon captain pirate beaten_by_pirates killer_of_pirates engaged hunter_of_bears alcoholic happy married admiral cocain_addict - state
	)
	(:init
		(is_at harbor)
	)
	(:goal
		(and
			(is happy)
		)
	)
)