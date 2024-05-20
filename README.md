<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h1 align="center">Blasphemous</h1>

  <p align="center">
    Blasphemous is a difficult metroidvania set in a weirdly twisted religous setting. The game has many unique bosses and an intricate map with plenty of areas to explore for completionists.
    <br />
    <strong>Original game : </strong>
    <a href="https://store.steampowered.com/app/774361/Blasphemous/"><strong>General info »</strong></a>
    ·
    <a href="https://youtu.be/XbWL90l_0Ow"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

TODO: add screenshot 

Here's why:
TODO: describe why you chose this game 
* I wanted a game that I enjoyed myself to keep motivation to work on the project.
* I wanted a fairly recent game instead of an nes game or similar games from that era

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* The movement
* The enemies shown in the video
* The areas shown in the video
* The pickups shown in the video

### What I will probably make as well:
* The map
* The prayer abilities
* The activatable shrines
* The grief mechanic (punishment for dying)

### What I plan to create if I have enough time left:
* The execute mechanic
* More areas
* More enemies
* A boss from a zone
* The inventory and permanent upgrades

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
Detailed instructions on how to run your game project are in this section.

### Prerequisites

* Visual Studio 2022

### How to run the project

Build and run the latest version with a compiler that supports at least C++17

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play

These are the currently implemented controls.

### Controls
* A to move left
* D to move right
* S to crouch
* W to climb a ladder or S while standing above a ladder
* Space to jump
* Shift to dodge
* Left click to attack
* Right click to parry

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
## Class structure 

### Object composition 
The player class knows about the texture manager, enemy manager and the level manager. 
The level manager knows about the texture manager and enemy manager. 
The enemy manager knows about the player and level manager. 

The game class has all the manager classes.

The level manager owns multiple vectors of level objects such as the collision, the platforms, the ledges, ...
The enemy manager owns a vector of enemies.

### Inheritance 
Different enemy types all inherit from the enemy baseclass.

### ..

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [X] week 01 topics applied
    - [X] const keyword applied proactively (variables, functions,..)
    - [X] static keyword applied proactively (class variables, static functions,..)
    - [X] object composition (optional)
- [X] week 02 topics applied
- [X] week 03 topics applied
- [X] week 04 topics applied
- [ ] week 05 topics applied
- [ ] week 06 topics applied
- [X] week 07 topics applied
- [X] week 08 topics applied
- [X] week 09 topics applied (optional)
- [ ] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Tirez Arthur - arthur.tirez@student.howest.be

Project Link: [https://github.com/HowestDAE/dae16-TirezArthur](https://github.com/HowestDAE/dae16-TirezArthur)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. 

* [Many pages on cpp reference](https://en.cppreference.com/w/)
* [Jsoncpp an open source json parser for C++](https://github.com/open-source-parsers/jsoncpp)
* [Forum user miner600 provided all the sprites from the original game](https://mugenguild.com/forum/topics/blasphemous-spritesupdated-191072.0.html)
* [Reddit user RAMAR713 provided all the sounds from the original game](https://www.reddit.com/r/Blasphemous/comments/xnl62p/blashpemous_sound_files/)
* [codeandweb.com allowed me to make the spritesheets](https://www.codeandweb.com/free-sprite-sheet-packer)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

