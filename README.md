# CS 3503 Assignment 3 - BitBoard Game
## Author
Kai Glaza
## Description
My implementation of Owl Tech’s BitBoard Checkers game
## Build Instructions
```
bash

gcc checkers.c -o checkers

./checkers

```
## Game Instructions
The move format: from_row from_column to_row to_column, specifying with coordinates how you want to move your piece

If you want to quit before actually finishing the game, enter -1 at any time

It follows standard checkers rules

## Notes
Since I've had some decent practice now the `bitops.c` file was not too hard to code at all
`checkers.c` was by far the hardest to code, as it just felt like it was taking forever and hurt me the most
with stuff like scanners which I've had practice with in Java a lot but it was weird in C nonetheless

I did like the freedom to kind of code whatever I want to make it work

## Biggest Struggles
The pointers like using `->` I quickly got the hang of

Checking for Kings was the hardest conceptually with the 0x000000 bit stuff but YT helped a lot

## Favorite things learned
I think I got a lot better with coding more condensed like putting returns on the same line

I ended up commenting so much that I just felt I needed to and I eventually got to the point of doing it
whilst coding but sometimes like with nested statements I would add uneccessary brackets just because it got
visually hard to look at

## SORRY FOR:
The absurd commenting I put a lot in the collapsable blocks so they don't get super annoying

The actual menu interaction – it took me a couple of tries to really get the flow of it for testing

