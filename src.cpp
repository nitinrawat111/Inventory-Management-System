#include<fstream>
#include<iostream>
#include<conio.h>
#include<string.h>
using namespace std;

void upper(char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

int choice_prompt(int lower_bound, int upper_bound)     //Both bounds are inclusive
{
    int choice;
    do
    {
        cout<<"Enter choice(Between "<< lower_bound << " and " << upper_bound << ") : ";
        cin>>choice;
    } while (choice < lower_bound || choice > upper_bound);
    return choice;
}

class item
{
    char itemId[7];
    char itemType[10];
    float price;
    int available_stock;
    public:
    float get_price()
    {
        return price;
    }
    int get_stock()
    {
        return available_stock;
    }
    char* get_Id()
    {
        return itemId;
    }
    char* get_type()
    {
        return itemType;
    }
    void sold(int sold_quantity)
    {
        available_stock -= sold_quantity;
    }
    void restock(int bought_quantity)
    {
        available_stock += bought_quantity;
    }
    void update_price(float new_price)
    {
        price = new_price;
    }
    void get_data();
    void display_data();
};

void item :: get_data()
{
    cout<<"Enter Item Type : ";
    cin>>itemType;
    cout<<"Enter ItemId : ";
    cin>>itemId;
    cout<<"Enter Price : ";
    cin>>price;
    do
    {
        cout<<"Enter available stock(should be >= 0) : ";
        cin>>available_stock;
    } while (available_stock < 0);
    upper(itemType);
    upper(itemId);
}

void item::display_data()
{
    cout<<"Type = "<<itemType<<endl;
    cout<<"Id = "<<itemId<<endl;
    cout<<"Price = "<<price<<endl;
    cout<<"Available Stock = "<<available_stock<<endl;
}

void add()
{
    item newItem;
    newItem.get_data();
    ofstream file(newItem.get_type(), ios_base::binary | ios_base::app);
    file.write((char*)&newItem, sizeof(newItem));
    cout<<"Object added successfully. Press any Key to return to menu....";
    getch();
    file.close();      
}

void update()
{
    int choice;
    cout<<"1. Update Stock\n";
    cout<<"2. Update Price\n";
    choice = choice_prompt(1, 2);
    
    system("cls");
    char type[10], id[7];
    cout<<"Enter Type of item which you want to update : ";
    cin>>type;
    cout<<"Enter Id of the item which you want to update : ";
    cin>>id;
    upper(type);
    upper(id);

    item item_to_update;
    bool found = false;
    fstream file(type, ios_base::binary | ios_base::in | ios_base::out);
    if(!file.is_open())
    {
        cout<<"No data for such a type exixts. Press any key to return to menu";
        getch();
        return;
    }
    while(!file.eof())
    {
        file.read((char*)&item_to_update, sizeof(item_to_update));
        if(strcmp( item_to_update.get_Id(), id) == 0 )
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        cout<<"Item not found. Press any key to return to menu...";
        getch();
        return;
    }
    file.seekp(-1 * sizeof(item_to_update), ios_base::cur);

    switch(choice)
    {
        case 1:
                cout<<"Select an option\n";
                cout<<"1. Item(s) has been sold \n";
                cout<<"2. Item(s) has been bought \n";
                choice = choice_prompt(1, 2);
                switch(choice)
                {
                    case 1:
                            int sold_quantity;
                            do
                            {
                                cout<<"Enter number of pieces sold(should be >= 0) : ";
                                cin>>sold_quantity;
                            }while(sold_quantity < 0);
                            item_to_update.sold(sold_quantity);
                            break;
                    case 2: 
                            int bought_quantity;
                            do
                            {
                                cout<<"Enter number of pieces bought(should be >= 0) : ";
                                cin>>sold_quantity;
                            }while(bought_quantity < 0);
                            item_to_update.restock(bought_quantity);
                            break;
                }
        case 2:
                float new_price;
                cout<<"Enter new Price : ";
                cin>>new_price;
                item_to_update.update_price(new_price);
    }
    file.write((char*)&item_to_update, sizeof(item_to_update));
    cout<<"Item updated. Press any key to return to menu...";
    getch();
    file.close();
}

void analyse()
{
    char type[10];
    int choice;
    do 
    {
        cout<<"Enter Type of item(s) whose stock you want to see : ";
        cin>>type;
        upper(type);

        item item_to_display;
        ifstream file(type, ios_base::binary | ios_base::in);
        if(!file.is_open())
        {
            cout<<"No data for such a type exixts. Press any key to return to menu";
            getch();
            return;
        }

        system("cls");
        while(file.read((char*)&item_to_display, sizeof(item_to_display)))
        {
            item_to_display.display_data();
            cout<<"--------------------------------------------------------------------\n";
        }

        cout<<"Press 1 to see stock for another type otherwise press any other number to go to main menu : ";
        cin>>choice;

        system("cls");
    }while(choice == 1);
}

int main()
{
    int choice;
    do
    {
        system("cls");
        cout<<"1. Analyse Stock\n";
        cout<<"2. Add an item\n";
        cout<<"3. Update Inventory\n";
        cout<<"4. Exit\n";
        choice = choice_prompt(1, 4);
        system("cls");
        switch(choice)
        {
            case 1:
                    analyse();
                    break;
            case 2:
                    add();
                    break;
            case 3:
                    update();
                    break;
            case 4:
                    exit(0);
                    break;
        }
    } while (1);
}