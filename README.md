	rollForName is a tool to randomly generate stuff. The whole thing is very simple : write random tables for anything, then tell the software from which tables to choose, and in which order, all using simple txt files. rollForName uses a very simple syntax for writing files. Its easy to understand, easy to use, and yet gives you enough freedom to generate just about anything.

How does it work?
rollForName works with 2 different files : table files, and generation files.

Table files contain a bunch of tables, like "age", "occupation", "origin country", etc. Tables can contain reference to other tables. For example, you can have a table "Race" that has 4 entries : "human", "elf" "dwarf", and "other". That "other" entry would refer to another table, where you can draw again from a list of less common races. You can use the tables provided with the software, or write your own. 
To write your own table files, see the corresponding section below.

Generation files are basically a blueprint that tells the software what to generate. For example, a basic human fantasy character would have a gender (male or female), and an age (child, teen, adult, elder). There goes our first blueprint! A more complicated one would draw the same things, but would probably add an occupation, skin/hair/eye color, spoken dialects, etc. Bottom line is : you just tell the software which tables to draw from, and he does that for you.
To write your own generators, see the corresponding section below.

	Writing tables 
Let's start by writing tables. All tables must be contained within a .rfn file, put somewhere in the res/tables/ folder. let's learn with a small example : 

"hair color"
{
	* "Blond" : "Some would say golden."
	* "Dark" : "Raven-like. Gothics will like that one."
	* "Gray" : "Well past your youth."
}

As you can see, a table starts with a name between quotes. That name is an identifier : it must be unique. You can't have two tables with the same name.
Next, there are a few entries between curly brackets {}. All entries have the same layout : 
- A character that signals the start of the entry. Here, it's '*', but you can also use '!' or '-' if you prefer those.
- Then an entry name, within quotes.
- Then a colon ':'.
- Then an entry description, within quotes.
When the program draws from the table "hair color", it will an entry at random and return its name, and description. So, when running the software, you will get something like : "[hair color] : blond : Some would say golden."

Pretty simple, right? Let's get to the interesting stuff. What if you want an entry to be selected more often than others? You could just write the same line several times, but that would be stupid. Instead, you can use a dice roll, and ranges. Look at the following example.

"social status"
[1d6]
{
	! <0> "Unknown" : "Sorry, who? Never head of him."
	! <1> "Newcomer" : "Arrived last week, or really sucks at making friends."
	! <2> "New friend" : "Haven't been here long, but everyone feels you have potential."
	! <3> "Old friend" : "Been here a while, you know a reasonable amount of people."
	! <4> "Familiy friend" : "Not part of the family, but you know, you're part of it anyway."
	! <5_6> "Part of the landscape" : "Whenever tourists picture the place, you're right next the historical monument."
}

Two new things have appeared ! First, a roll : [1d5]. All rolls are written between brackets []. The syntax is pretty easy : [1d5] reads "throw 1 dice that has 5 faces". Since this is a computer program, and we don't care about stuff like physics in here, you can use the number of faces you want. Of course, you can do complicated rolls by adding modifiers, like [1d5+2]. That will throw a d5, then add 2 to the result. 
You can also do several rolls, like those : 
[2d5] : throw 2 dices with 5 faces.
[1d20+1d6] : Throw 1 dice with 20 faces, and 1 dice with 6 faces.
And, of course, you mix up the two methods : 
[1d6+2+1d20-42]

That's all very funny, but how do we use the result of that roll? We use ranges. Ranges are written between chevrons <>. They're easy to understand : <5_6> means "any number between 5 and 6 included". For single numbers, you can just write it directly, meaning that <4_4> and <4> mean the same thing : the number four.

In our example, you can see that we throw a 6 sided dice, and just go to the corresponding line in the table.
Quick FAQ : 
- <2_4> and <4_2> are the same. Internally, the software will just sort out which is the biggest and lowest number.
- "What if ranges intersect? Like a table with two lines, one with range <1_3> and the other with range <2_4> ?" : That's an invalid table, and you should feel bad about yourself.
- "What if the result of the roll doesn't correspond to any table" : nothing. The software will display a blank line, and just go to the next table to be drawn.


Lets do something even funnier : variables! Look at those two tables : 

"charisma"
{
	! "boring" : "($cha=-1)"
	! "normal" : "($cha=0)"
	! "charismatic" : "($cha=1)"
}

"social status"
[1d3]+($cha)
{
	! <0> "Unknown" : "Sorry, who? Never head of him."
	! <1> "Newcomer" : "Arrived last week, or really sucks at making friends."
	! <2> "New friend" : "Haven't been here long, but everyone feels you have potential."
	! <3> "Old friend" : "Been here a while, you know a reasonable amount of people."
	! <4> "Familiy friend" : "Not part of the family, but you know, you're part of it anyway."
	! <5> "Part of the landscape" : "Whenever tourists picture the place, you're right next the historical monument."
}

You might see where this is going. The first table draws you charisma, and affects a value to a variable. The second table uses that variable to draw out your social status. Let's look at that ugly syntax.
All variables have a name, preceeded by a dollar $, and the whole thing is surrounded by parentheses. So if you write ($test), you're refering to the variable named "test". You can't put spaces in a variable name, and its a good idea to keep them short and meaningful.
You can assign a value to a variable like this : ($test=2). Variables can only contain whole numbers (no decimals, or fractions). Assigning a value can only be done in an empty description, so you can't assign a value in the middle of a description.
As you can see in the second table's roll, you can just add the variable to the roll as shown.

Quick FAQ : 
 - "Can I use variables and rolls in table entries?" : Yes you can. Just remember to put the roll between brackets, and the variable between parenthesis and preceeded by a dollar. That would look something like "You gain a magical skeleton servant for [1d10]+($luck) days", for example.
 
Last thing about tables : you can write tables that refer to other tables! Consider the following example.

"common races"
{
	- "Orc" : "Prone to fits of WHAAAAAAAG!"
	- "Elf" : "Proud salad lover."
	- "Dwarf" : "Proud salad hater."
	- "Human" : "Remarkably average in every way."
	- "Other" : "-> other races"
}

"other races"
{
	- "Goblin" : "Sly by nature."
}



	Writing generators
First, let's start by writing a few generator files to get a hang of the whole thing. Let's look at an example.

"Fantasy character example"
REQUIRED GENERATORS : "common languages"
REQUIRED TABLES : "fantasy/jobs", "fantasy/races/"
{
	t: "gender"
	t: "age"
	g: "common languages"
	t: "common races"
	t: "fantasy job"
}

Let's go through that line by line. First, there's the name between quotes (""). The name is a unique identifier : you can't have two generators (or tables) with the same name. But you can have a generator and a table with an identical name, because generators and tables are handled differently.

Next, the required generators. This is a list of all the "child" generators our "Fantasy character example" needs to function properly. This must be a relative path to a generator file containing the generator you want.