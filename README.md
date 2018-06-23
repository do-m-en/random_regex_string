# random_regex_string
Generates random strings that satisfy the provided regular expression - experimental and designed for property based testing.

Code is written in C++17 (was actually being developed when C++17 has not yet been released so the code is compiled with c++1z flag and contains a mixture of experimental and C++17 headers).

# Here be dragons
This repository contains my experimentation between the end of 2016 and beginning of 2017. After that point I've had different priorities so the library was never finished.

Repository contains many different branches that are in different stages of feature completeness.

They also differ in the way I've implemented the generator as I was experimenting with different ways of implementing it in C++.

The most recent content is on master branch (this is also the branch that I'll continue developing once I get back to it) but if I remember correctly spirit_x3_inspired_parser branch contains the most feature complete version.

I intended to add it to open source public domain once it'd be finished but haven't gotten around to do it. I usually hate putting unfinished things to open source - the world is already full of half baked projects that waste peoples time just by existing for others to see - but this time I'll make an exception since I need a place to store it and also believe that it is still a good reference for others that are interested in how (not) to implement a regex parser and string generator in C++.

I hope to get back to it one day but until then: Use it at your own risk :) You've been warned ;)

# How to build it
Standard CMake procedure:
```
mkdir build
cd build
cmake <path_to_the_source_code>
make
```
Code has been developed and tested on Linux (Ubuntu 18.04, GCC 7.3 - this version already reports warnings that weren't there in the latest GCC and clang that existed at the beginning of 2017).

# Missing features
This is just a short TODO list - there are certainly more missing features.
- Branches support different regex capabilities - depending on the branch - so master branch would have to support them all.
- There are some cases where I've made mistakes regarding capture groups so if I remember correctly they stil don't work correctly (if one capture group generates a pattern, that pattern has to be used in all subsequent references and not regenerated from allowed patterns) - it's possible that master branch already correctly supports this (haven't checked).
- Unicode support (UTF-8 and perhaps also UTF-18 and UTF-32).
- Intentionally generate a string that doesn't match the provided regex - since this library was intended for property based testing it should generate not just patterns that match the provided regex but also on demand generate those that don't match to test negative cases.
- Pay only for what you use - since regex can contain capture groups, references on those groups and a couple of other regex features that cause a performance penalty (either slows down parsing, generation or uses more memory) code should be written so that the user can toggle support of those features so that overhead is payed only when realy needed.
- Convert code so that it will support runtime as well as compile time regex parsing (bump to C++20 would probably be a desirable precondition for this).
- Remove all raw heap allocations - currently code contains memory leaks as the plan was always to remove all the heap allocations for compile time use so not much effort went into correct memory cleanup as that was only an intermediate version.

# License
Code is licensed under Boost 1.0 license.
