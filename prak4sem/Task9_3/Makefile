all: game

game: Weapon.o Potion.o Armor.o MagicItem.o MagicWeapon.o Inventory.o main.o
	g++ -o game Weapon.o Potion.o Armor.o MagicItem.o MagicWeapon.o Inventory.o main.o

Weapon.o: Weapon.cpp Weapon.h Item.h
	g++ -c Weapon.cpp

Potion.o: Potion.cpp Potion.h Item.h
	g++ -c Potion.cpp

Armor.o: Armor.cpp Armor.h Item.h
	g++ -c Armor.cpp

MagicItem.o: MagicItem.cpp MagicItem.h
	g++ -c MagicItem.cpp

MagicWeapon.o: MagicWeapon.cpp MagicWeapon.h Weapon.h MagicItem.h
	g++ -c MagicWeapon.cpp

Inventory.o: Inventory.cpp Inventory.h Item.h
	g++ -c Inventory.cpp

main.o: main.cpp Weapon.h Potion.h Armor.h MagicWeapon.h Inventory.h
	g++ -c main.cpp

clean:
	rm -f *.o game
