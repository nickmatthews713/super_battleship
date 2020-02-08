# Super Battleship

I made a mini battleship game. the rules are similar to regular battleship, except  
that you have 3 special abilities. you can pick up and use(bomb, torpedo, and flurry).  
The text will walk you through the game and how the ui works. please resize your  
terminal window to 50 x 24 before playing to fit the screen. The player who destroys the  
other teams ships first is the winner.

## Usage

Once you have the project files downloaded, run the following commands inside the project  
to start the game
```
make
```
This will generate the excecutable named 'battle.exe' which you can then excecute with
```
./battle
```

## Gameplay

What the game turn looks like

1) PLACE YOUR SHIPS: you'll be prompted to place ships by pressing 'a'
to randomize a set of ships and 'any key' to use that set

2) CHOOSE ATTACK: choose between your unlimited normal attack by pressing
'a', or choose from your limited special abilities(bomb, topedo, flurry)
by pressing 'q', 'w', or 'e' respectively

3) TARGET A LOCATION: after choosing an attack, there will be a cursor
appearing in the left corner of the NPC'S field. navigate the cursor using
the arrow keys and then press 'f' to fire on that location.

4) ENEMY'S TURN: the NPC will fire on you using certain hueristics

## Author

  * **Nick Matthews** - (https://github.com/nickmatthews713)
