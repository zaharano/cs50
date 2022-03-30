# Calc Dat
[Live](https://zaharano.github.io/calculator/)

It's a calculator. It does what it says on the tin.

I'm proud of how I (eventually) figured out that it's best to just abstract the display away from the real values going on under the hood - this way you can accurately calculate numbers with lots of digits while not trying to figure out how to show various amounts of exponentiation and decimals. Storing the values as strings for the purpose of mutation, but casting them to numbers for operating and rounding seems a good system. 

When I say accurately calculate numbers with lots of digits I should qualify... the floating point issues of JS cause weird behavior, but that's quite beyond the scope here I think. It's maybe an argument for NOT attempting to accurately retain high digit counts and just go ahead and round everything in storage. But I'm moving on for now!

*Part of The Odin Project curriculum*

### Todos:
* [x] Add +/- key : * -1 I think?
* [x] Add backspace : slice length - 1
* [x] Add keypress event listeners : simple enough
* [x] err handling - display error output but don't keep error output
* [x] fix up the digit fix for display (adapt to e, -, .)

### Bugs
* [ ] Calc vals *are* being rounded when I don't think they should be, even beyond the issues in JS floating point I think. A good chance to try a testing setup!
* [ ] unhandled annoying floating point in JS issues. ex. 3.3 * 3 = 9.899999.. 
* [x] need to handle actual out of bound situations? (outside js int handling) *nah 'Infinity' display is fine*

### Nice to adds
* [ ] copy to clipboard button on currVal when hovered
* [ ] repeated = presses chain the last performed op so 3 + 5 = 8 pressing = again results 13, 18 etc
    * maybe extra opVal cache, use existing chainFlag
    * maybe good to refactor so that that equals puts the result in opVal, doesn't clear opSign. I think this is how most calculators actually work. Results are displayed in the sub display to indicate they can be further operated on. 
* [x] some responsiveness (could use better implementation)
