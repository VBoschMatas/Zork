# Zork

## Author: Vicenç Bosch Matas

Small game created from a base code obtained at https://github.com/d0n3val/zork.

Main, globals and general object structure are similar to the base code. 

This version of Zork has 11 rooms, various creatures and items, consumables, new ways of blocking paths and quests.

## How to play
Through keyboard input type the actions the main character has to perform in order to navigate and complete the quests.
List of commands:
>look
>
>stats
>inventory
>look [entity]
>[direction]
>go [direction]
>take [item]
>take [item] from [container]
>drop [item]
>drop [item] at [container]
>equip [item]
>unequip [item]
>talk (to) [creature]
>attack [creature]
>loot [creature]
>examine [creature]
>break [direction]
>unlock [direction] with [item]
>lock [direction] with [item]
>use [item]

Added a "help" comman in game to list all commands there.

## Objective
The main goal of this game is to complete the quest comissioned by the dispatcher.
To read the details in game type ">talk to dispatcher".
The quest is about finding a troll thats inside one of the rooms, killing it, and returning its head back to the dispatcher.
Once this is done the game will successfully be finished.
If the main character dies at some point the game will end in a defeat.

There are various secondary objectives, like completing side quests or collecting better gear. Not all connections will be unlocked or unblocked at the beggining, the player must have to figure out how to get through the obtacles.
