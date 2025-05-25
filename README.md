
# Custom CS:GO like Case Opener game

A Case Opener game written in C++, with luck based skin rolling, user statistics and inventory, you can choose between 3 official cases or make even one yourself!


## Authors

- [Mikołaj Tryba](https://www.github.com/Meecoy)
- [Dominik Zając](https://www.github.com/dzajaczsme)
- [Adrian Wąż](https://github.com/AdrianW3945)


## Case Opening

![](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExdDB6c2hmYXNiaG9tdXhkcDNtaDN0Ynk5dzlzcHJ1dnd0ZGt0d280aSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fEkOLBjDYFEb1CnQ9x/giphy.gif)

The main feature of opening the case consists of the graphic part, which pick random skins from selected case and displays them in vertical boxes and the backend part, where skin that's actually the winning one is randomly rolled using the ```draw_skin()```. After the roll it is written to ```User/inventory.json``` file with the full skin data that isn't shown in the opening screen.

![](https://i.imgur.com/9Rfs6Ev.png)
## Behind The Scenes

Let's start with the core function, earlier mentioned ```draw_skin()```.
It takes 2 args, a case name which the skin will be rolled from and a luck factor, which will be crucial for adding difficulty in Case Fights with bots. The whole body of this function took the most time to make, and not for no reason.

To make this as "random" as possible, it's using Mersenne Twister random number generator to roll a number from range 0 - 1000. Based on the rolled number it determines the quality of the skin, which later gets all the skins with that quality from the case inserted to the vector and rolls again the index of the skin inside the vector.

The returned skin data is stored in a struct:
```
struct Returned_skin {
    std::string skin_title;
    std::string skin_description;
    std::string skin_ps;
    std::string skin_quality;
    std::string skin_weapon;
    std::string skin_path;
    std::string skin_float;
    bool skin_stat_track;
    unsigned int skin_price;
};
```
, where based on the rolled skin title the rest of the information except for float, stat track and price is assigned automatically.

```
// IMPORTANT NOTE! The if statements are used because the abc["xyz"].get(something) statement 
// is mandatory for getting the data from json file, and g++ doesn't like when returned value from 
// the function isn't used.
  if (selected_skin["title"].get(title) == simdjson::SUCCESS) {
    Skin.skin_title = std::string(title);
  }
  if (selected_skin["description"].get(description) == simdjson::SUCCESS) {
    Skin.skin_description = std::string(description);
  }
  if (selected_skin["ps"].get(ps) == simdjson::SUCCESS) {
    Skin.skin_ps = std::string(ps);
  }
  if (selected_skin["skin"].get(path) == simdjson::SUCCESS) {
    Skin.skin_path = std::string(path);
  }
  if (selected_skin["weapon"].get(weapon) == simdjson::SUCCESS) {
    Skin.skin_weapon = std::string(weapon);
  }

  if (i > 0) {
    Skin.skin_quality = qualities[i - 1].quality;
  }
```

The 3 skin informations that aren't mentioned here are slightly more complex to get. Starting with price, it's determined by skin rarity. Normal skins could get prices that doesn't exactly match their statistics, but the knife is surely the most priced one.

Next is float, which is determined byrandomly generating 12 numbers after 0. and adding them to one string.

The last one is the stat_track, which is determined in slightly different way, as it will only be true if the 7th number of the float is equal five, which is better than boring roll from 1-10 and actually makes it appear more rarely. It also boost up the skin price by quite a large sum!


## Case Fights

Although the rest of the functions aren't as complicated or interesting, our next stop are the Case Fights.

Case Fights use the same function as normal rolls, but you can change the number of rolls. After that the function for rolling skins is repeated for each player the same amount of times, the skins are stored in a vector, total price is compared and the winner gets it all (Or doesn't, because bots don't have saved eq)!

The biggest difficulty in fighting is ***Luck***. ***Luck*** adds up to the 0 - 1000 range given number, making it so the bot could get better cards, even making him get only the knifes!
## Making Your Own Case

The biggest feature of our Case Opener is that it's not typical simulator, that just takes 1:1 CS:GO skins, puts them in a case and that's all. Our game is more universal, as you can practically make your own cases, just following a few steps!

**1.** Add a directory to ```Cases/<yourcasename>``` folder.

**2.** Add another directory for your skins images and ```case.json``` file inside previously made dir. Optionally you could add the case thumbnail in case.png (It must be in 512x format).

**3.** Open the ```case.json`` in text editor and add the objects inside the array with skins data you want to roll. It hould look like this:
```
[
    {
        "title": "",
        "description": "",
        "ps": "",
        "quality": "",
        "weapon": "",
        "skin": "Cases/<path>/<to>/<yourskin.png>"
    },

    ...
]
```

***Important note:*** You must add atleast 1 skin for each quality, otherwise the program might return N/A in the place of your rolled skin!

**4.** Open your skins directory and insert images of skins you'd like to roll. Just remember they must be 512x format in order to work properly!

**5.** Open the program, choose the file and **voila**! You can now open your own skins from your own case!


## Building binaries

To compile this game you need to have latest version of Cmake or Make in order for this to work. Just use:

```
git clone https://www.github.com/Meecoy/Case-Opener-Cpp
cd Case-Opener-Cpp
git submodule init
git submodule update
make #Or whatever Cmake users have to use to use Makefile flags
./CaseOpener.exe
```
