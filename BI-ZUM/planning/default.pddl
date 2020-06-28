(define (domain planning)
	(:requirements :strips :typing)
	(:types place item friends state ship)
	(:predicates
		(has ?item)
		(owns ?ship)
		(is_at ?place)
		(is ?state)
		(knows ?friends)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                THE FOREST                ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_forest
		:precondition (or
			(is_at river)
		)
		:effect (and
			(not (is_at river))
			(is_at forest)
		)
	)
	(:action pick_flowers
		:precondition (and
			(is_at forest)
		)
		:effect (and
			(has flowers)
		)
	)
	(:action fight_bear
		:precondition (and
			(is_at forest)
		)
		:effect (and
			(is tough)
			(has bear_fur)
			(is hunter_of_bears)
		)
	)
	(:action get_map_from_grandpa
		:precondition (and
			(is_at forest)
			(has alcohol)
		)
		:effect (and
			(not (has alcohol))
			(has map)
			(knows shady_people)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                THE RIVER                 ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_river
		:precondition (or
			(is_at forest)
			(is_at harbor)
		)
		:effect (and
			(not (is_at forest))
			(not (is_at harbor))
			(is_at river)
		)
	)
	(:action steal_boat
		:precondition (and
			(is_at river)
		)
		:effect (and
			(is felon)
			(owns boat)
		)
	)
	(:action pan_gold
		:precondition (and
			(is_at river)
		)
		:effect (and
			(has gold_nugget)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                THE HARBOR                ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_harbor
		:parameters (?ship - ship)
		:precondition (or
			(is_at river)
			(is_at city)
			(is_at pub)
			(is_at sea)
			(and
				(is_at lighthouse)
				(owns ?ship)
			)
		)
		:effect (and
			(not (is_at river))
			(not (is_at city))
			(not (is_at pub))
			(not (is_at sea))
			(not (is_at lighthouse))
			(is_at harbor)
		)
	)
	(:action work
		:precondition (and
			(is_at harbor)
		)
		:effect (and
			(has gold_nugget)
		)
	)
	(:action sell_bear_fur
		:precondition (and
 			(is_at harbor)
			(has bear_fur)
		)
		:effect (and
			(not (has bear_fur))
			(has gold_coin)
		)
	)
	(:action sell_coconuts
		:precondition (and
			(is_at harbor)
			(has coconuts)
		)
		:effect (and
			(not (has coconuts))
			(has gold_coin)
		)
	)
	(:action meet_smugglers
		:precondition (and
			(is_at harbor)
			(knows shady_people)
			(has gold_brick)
		)
		:effect (and
			(knows smugglers)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                THE PUB                   ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_pub
		:precondition (or
			(is_at harbor)
		)
		:effect (and
			(not (is_at harbor))
			(is_at pub)
		)
	)
	(:action buy_alcohol
		:precondition (and
			(is_at pub)
			(has gold_nugget)
		)
		:effect (and
			(has alcohol)
			(not (has gold_nugget))
		)
	)
	(:action buy_alcohol_for_others
		:precondition (and
			(is_at pub)
			(has gold_coin)
		)
		:effect (and
			(knows honest_people)
			(not (has gold_coin))
		)
	)
	(:action fight_in_pub
		:precondition (and
			(is_at pub)
			(is drunk)
		)
		:effect (and
			(is tough)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                THE CITY                  ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_city
		:precondition (or
			(is_at harbor)
			(is_at academia)
		)
		:effect (and
			(not (is_at harbor))
			(not (is_at academia))
			(is_at city)
		)
	)
	(:action make_savings
		:precondition (and
			(is_at city)
			(has gold_nugget)
		)
		:effect (and
			(not (has gold_nugget))
			(has gold_coin)
			(knows honest_people)
		)
	)
	(:action invest
		:precondition (and
			(is_at city)
			(has gold_coin)
		)
		:effect (and
			(not (has gold_coin))
			(has gold_brick)
			(knows honest_people)
		)
	)
	(:action become_thief
		:precondition (and
			(is_at city)
		)
		:effect (and
			(is felon)
			(has gold_coin)
		)
	)
	(:action buy_indulgences
		:precondition (and
			(is_at city)
			(has gold_nugget)
		)
		:effect (and
			(not (is felon))
			(not (has gold_nugget))
		)
	)
	(:action do_community_service
		:precondition (and
			(is_at city)
		)
		:effect (and
			(not (is felon))
			(is drunk)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;              THE ACADEMIA                ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_academia
		:precondition (or
			(is_at city)
		)
		:effect (and
			(not (is_at city))
			(is_at academia)
		)
	)
	(:action become_captain
		:precondition (and
			(is_at academia)
			(not (is felon))
			(has gold_coin)
		)
		:effect (and
			(not (has gold_coin))
			(is captain)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;                 THE SEA                  ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_sea
		:parameters (?ship - ship)
		:precondition (and
			(owns ?ship)
			(or 
				(is_at harbor)
				(is_at lighthouse)
				(is_at island)
			)
			(or
				(is pirate)
				(is beaten_by_pirates)
				(is killer_of_pirates)
			)
		)
		:effect (and
			(not (is_at harbor))
			(not (is_at lighthouse))
			(not (is_at island))
			(is_at sea)
		)
	)
	(:action go_to_sea_and_get_beaten_by_pirates
		:parameters (?ship - ship)
		:precondition (and
			(owns ?ship)
			(not (is tough))
			(not (is pirate))
			(not (is killer_of_pirates))
			(or
				(is_at sea)
				(is_at harbor)
				(is_at island)
				(is_at lighthouse)
			)
		)
		:effect (and
			(is tough)
			(not (owns boat))
			(not (owns frigate))
			(not (owns caravel))
			(not (owns gold_nugget))
			(not (owns gold_coin))
			(not (owns gold_brick))
			(is_at sea)
			(not (is_at harbor))
			(not (is_at island))
			(not (is_at lighthouse))
		)
	)
	(:action go_to_sea_and_join_pirates
		:parameters (?ship - ship)
		:precondition (and
			(knows shady_people)
			(not (is killer_of_pirates))
			(not (is pirate))
			(or
				(is_at sea)
				(and
					(owns ?ship)
					(or
						(is_at harbor)
						(is_at island)
						(is_at lighthouse)
					)
				)
			)
		)
		:effect (and
			(is drunk)
			(is pirate)
			(is_at sea)
			(not (is_at harbor))
			(not (is_at island))
			(not (is_at lighthouse))
		)
	)
	(:action go_to_sea_and_beat_pirates
		:parameters (?ship - ship)
		:precondition (and
			(not (is killer_of_pirates))
			(not (is pirate))
			(is tough)
			(owns caravel)
			(or
				(is_at sea)
				(and
					(owns ?ship)
					(or
						(is_at harbor)
						(is_at island)
						(is_at lighthouse)
					)
				)
			)
		)
		:effect (and
			(is killer_of_pirates)
			(owns boat)
			(owns frigate)
			(owns caravel)
			(has gold_nugget)
			(has gold_coin)
			(has gold_brick)
			(is_at sea)
			(not (is_at harbor))
			(not (is_at island))
			(not (is_at lighthouse))
		)
	)
	(:action dive_for_pearl
		:precondition (and
			(is_at sea)
		)
		:effect (and
			(has pearl)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;             THE LIGHTHOUSE               ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_lighthouse
		:parameters (?ship - ship)
		:precondition (or
			(is_at sea)
			(and
				(owns ?ship)
				(is_at harbor)
			)
		)
		:effect (and
			(not (is_at harbor))
			(not (is_at sea))
			(is_at lighthouse)
		)
	)
	(:action impress_girl
		:precondition (and
			(is_at lighthouse)
			(or
				(is captain)
				(is killer_of_pirates)
				(is hunter_of_bears)
			)
		)
		:effect (and
			(is engaged)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;               THE ISLAND                 ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action go_to_island
		:precondition (or
			(is_at sea)
		)
		:effect (and
			(not (is_at sea))
			(is_at island)
		)
	)
	(:action pick_up_coconuts
		:precondition (and
			(is_at island)
		)
		:effect (and
			(has coconuts)
		)
	)
	(:action loot_cocain_lair
		:precondition (and
			(is_at island)
			(has map)
		)
		:effect (and
			(has cocain)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;            OTHERS / COMBINED             ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action take_cold_shower
		:precondition (or
			(is_at river)
			(is_at sea)
		)
		:effect (and
			(not (is drunk))
		)
	)
	(:action chop_trees
		:precondition (or
			(is_at forest)
			(is_at island)
		)
		:effect (and
			(has wood)
		)
	)
	(:action make_ring
		:precondition (and
			(has gold_brick)
			(has pearl)
		)
		:effect (and
			(not (has gold_brick))
			(not (has pearl))
			(has ring)
		)
	)
	(:action build_boat
		:precondition (and
			(has wood)
		)
		:effect (and
			(not (has wood))
			(owns boat)
		)
	)
	(:action build_frigate
		:precondition (and
			(has wood)
			(has gold_nugget)
			(owns boat)
		)
		:effect (and
			(not (has wood))
			(not (has gold_nugget))
			(not (owns boat))
			(owns frigate)
		)
	)
	(:action build_caravel
		:precondition (and
			(has wood)
			(has gold_coin)
			(owns boat)
		)
		:effect (and
			(not (has wood))
			(not (has gold_coin))
			(not (owns boat))
			(owns caravel)
		)
	)
	(:action get_drunk
		:precondition (and
			(has alcohol)
		)
		:effect (and
			(not (has alcohol))
			(is drunk)
		)
	)
	(:action become_an_alcoholic
		:precondition (and
			(has alcohol)
			(is drunk)
		)
		:effect (and
			(not (has alcohol))
			(is alcoholic)
		)
	)


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;               	 ENDINGS                  ;;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action wedding
		:precondition (and
			(has ring)
			(has flowers)
			(knows honest_people)
			(is engaged)
			(not (is felon))
			(is_at island)
			(not (is drunk))
			(not (is alcoholic))
		)
		:effect (and
			(is married)
		)
	)	
	(:action become_admiral
		:precondition (and
			(is captain)
			(is killer_of_pirates)
			(is_at academia)
			(not (is drunk))
		)
		:effect (and
			(is admiral)
		)
	)	
	(:action become_cocain_addict
		:precondition (and
			(has cocain)
			(is alcoholic)
			(owns frigate)
			(knows smugglers)
			(has gold_brick)
		)
		:effect (and
			(is cocain_addict)
		)
	)

	(:action become_happy
		:precondition (or
			(is married)
			(is admiral)
			(is cocain_addict)
		)
		:effect (and
			(is happy)
		)
	)
)