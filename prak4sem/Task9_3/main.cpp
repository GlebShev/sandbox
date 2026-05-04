#include "Item.h"
#include "Weapon.h"
#include "Potion.h"
#include "Armor.h"
#include "MagicWeapon.h"
#include "Inventory.h"
#include <iostream>
#include <string>

using namespace std;

void showMenu()
{
    cout << "\nМеню:" << endl;
    cout << "1. Добавить оружие" << endl;
    cout << "2. Добавить зелье" << endl;
    cout << "3. Добавить броню" << endl;
    cout << "4. Добавить магическое оружие" << endl;
    cout << "5. Показать инвентарь" << endl;
    cout << "6. Использовать предмет" << endl;
    cout << "7. Удалить предмет" << endl;
    cout << "8. Проверить наличие предмета по имени" << endl;
    cout << "9. Выход" << endl;
    cout << "Выберите действие: ";
}

int main()
{
    Inventory inventory;
    int choice;
    while (true)
    {
        showMenu();
        cin >> choice;
        if (choice == 9)
            break;
        switch (choice)
        {
        case 1:
        {
            string name;
            double weight;
            int damage;
            cout << "Введите название оружия: ";
            cin >> name;
            cout << "Введите вес: ";
            cin >> weight;
            cout << "Введите урон: ";
            cin >> damage;
            inventory.addItem(new Weapon(name, weight, damage));
            break;
        }
        case 2:
        {
            string name;
            double weight;
            int healing;
            cout << "Введите название зелья: ";
            cin >> name;
            cout << "Введите вес: ";
            cin >> weight;
            cout << "Введите количество восстановления HP: ";
            cin >> healing;
            inventory.addItem(new Potion(name, weight, healing));
            break;
        }
        case 3:
        {
            string name;
            double weight;
            int defense;
            cout << "Введите название брони: ";
            cin >> name;
            cout << "Введите вес: ";
            cin >> weight;
            cout << "Введите уровень защиты: ";
            cin >> defense;
            inventory.addItem(new Armor(name, weight, defense));
            break;
        }
        case 4:
        {
            string name;
            double weight;
            int damage;
            string magicEffect;
            cout << "Введите название магического оружия: ";
            cin >> name;
            cout << "Введите вес: ";
            cin >> weight;
            cout << "Введите урон: ";
            cin >> damage;
            cout << "Введите магический эффект: ";
            cin >> magicEffect;
            inventory.addItem(new MagicWeapon(name, weight, damage, magicEffect));
            break;
        }
        case 5:
        {
            cout << "Содержимое инвентаря:" << endl;
            int index = 0;
            for (auto item : inventory)
            {
                cout << index << ": " << item->getName() << endl;
                ++index;
            }
            break;
        }
        case 6:
        {
            int index;
            cout << "Введите индекс предмета для использования: ";
            cin >> index;
            Item *item = inventory.getItem(index);
            if (item)
            {
                item->use();
            }
            else
            {
                cout << "Предмет не найден!" << endl;
            }
            break;
        }
        case 7:
        {
            int index;
            cout << "Введите индекс предмета для удаления: ";
            cin >> index;
            inventory.removeItem(index);
            break;
        }
        case 8:
        {
            string name;
            cout << "Введите имя предмета для поиска: ";
            cin >> name;
            if (inventory.contains(name))
            {
                cout << "Предмет найден в инвентаре." << endl;
            }
            else
            {
                cout << "Предмет не найден." << endl;
            }
            break;
        }
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
    return 0;
}
