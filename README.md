rollForName is a tool to randomly generate stuff. The whole thing is very simple : write random tables for anything, then tell the software from which tables to choose, and in which order, all using simple txt files. rollForName uses a very simple syntax for writing files. Its easy to understand, easy to use, and yet gives you enough freedom to generate just about anything.

How does it work?
rollForName works with 2 different files : table files, and generation files.

Table files contain a bunch of tables, like "age", "occupation", "origin country", etc. Tables can contain reference to other tables. For example, you can have a table "Race" that has 4 entries : "human", "elf" "dwarf", and "other". That "other" entry would refer to another table, where you can draw again from a list of less common races. You can use the tables provided with the software, or write your own. 
To write your own table files, see the corresponding section below.

Generation files are basically a blueprint that tells the software what to generate. For example, a basic human fantasy character would have a gender (male or female), and an age (child, teen, adult, elder). There goes our first blueprint! A more complicated one would draw the same things, but would probably add an occupation, skin/hair/eye color, spoken dialects, etc. Bottom line is : you just tell the software which tables to draw from, and he does that for you.
To write your own generators, see the corresponding section below.

	Writing generators
First, let's start by writing a few generator files to get a hang of the whole thing. Let's look at an example.

GenericFantasyCharacterExample
REQUIRED GENERATORS : general/languages/common
REQUIRED TABLES : fantasy/jobs fantasy/races/

t: gender
t: age
g: "common language"
t: "common races"
t: "fantasy job"


I think its pretty self explanatory, but lets dive into it line by line anyway.
All generator files start with the same header for coherence purposes. The first line is the name. It can't contain spaces by default. You can rebel against this oppressive system if you want, but the software will just remove the spaces and gloat in an evil fashion anyway. Or you can also write the name between quotes ("like this"). That way you can put all the spaces you want. You rebel.

On the next line is the list of required generators. It HAS to start with the sentence "required generators :". UPPERCASE, lowercase, however you want, doesn't matter. Then, the names of the other generators you will use
Because, of course, generators can call on other generators. That means you can write a complex generator once, and reuse it anywhere! In that example, we say that this generator will use the "common" generator, located in the res/generators/general/languages/ folder. All generators must be placed in the res/generators/ folder. Similarly, all tables must be place in the res/tables/ folder.

Next up, the required tables. Same deal as above for the "required tables :" part.
You've guessed it : these are the table files this generator needs. As you can see, you can put several fies by separating them with spaces. I know rebels, calm down, we got something for you too : you can use spaces in the path by putting the whole thing between quotes. But you really shouldn't do that. If you know a programmer he will resent you for putting spaces in folder names. (And if he doesn't, he's a bad programmer and he should know it. Bad!)
If you learned under Sherlock Holmes, master of observation, you might have noticed that the two required tables have a slightly different syntax : "fantasy/jobs" and "fantasy/races/". 
SPOILER : There's a slash '/' at the end of "fantasy/races".
Might not seem like much, but its actually very important. This tells the program that "races" isn't a file : it's a folder. Meaning that instead of looking for a file named "races" in the folder "res/tables/fantasy/", the programm will instead load EVERY TABLE FILE in the "res/tables/fantasy/races/" folder. Pretty significant difference, and a neat little trick. (And that also works for generator files!)
You might be worried about overloading the program with a bunch of unnecessary files. Fear not : unless your table files contain as many tables as a thesaurus has words, you should be fine.

And then, the heart of the matter! The actual generation! As you can see, its pretty simple : just write all the tables and generators you want to use, one line after the other. Every line is prefixed with either "t:" (short for "table:") or "g:" (short for "generator:"). As you might have guessed, these refer to either tables, or generators.
If you don't like suffixes, you can actually not put them at all. The software will then search through all generators, THEN all tables, for something with that name. If no correspondance is found, it will print a nice big error message for you.
/!\ Note : As of now, loading different tables or generators with the same name is NOT supported. Meaning if you use the same name for different tables or generators, when loading the files, the program will just insult you with various degrees of politeness, then terminate. If you absolutely really really really neeeeeeeed this feature, either code it yourself, or email me some nice words with pictures of kittens and I'll add it.

About the whole generation process, quick FAQ : 
 - Every table/generator is called in order, top to bottom.
 - You can call the same table/generator several times if you want.
 - If you end up with an infinite loop (generator A calls generator B, that itself calls generator A, and so on...), the program will terminate with another moderately polite insult.


	Writing table files
Here we'll cover the syntax on how to write table files.