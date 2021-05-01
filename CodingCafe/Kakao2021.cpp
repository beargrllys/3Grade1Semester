#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string new_id)
{
    int index = 0;
    string answer = "";
    string save_letter = new_id;
    //step1
    for (int i = 0; i < save_letter.length(); i++)
    {
        if (save_letter[i] >= 65 && save_letter[i] <= 90)
        {
            save_letter[i] += 32;
        }
    }

    /*(save_letter[i] >= 65 &&  save_letter[i] <= 90)

    (save_letter[i] >= 97 &&  save_letter[i] <= 122)

    (save_letter[i] >= 48 &&  save_letter[i] <= 57)*/

    for (int i = 0; i < save_letter.length(); i++)
    {
        while (!(save_letter[i] == '-' || save_letter[i] == '_' || save_letter[i] == '.' || (save_letter[i] >= 65 && save_letter[i] <= 90) || (save_letter[i] >= 97 && save_letter[i] <= 122) || (save_letter[i] >= 48 && save_letter[i] <= 57)))
        {
            i++;
        }
        save_letter[index] = save_letter[i];
        index++;
    }
    while ((index = save_letter.find("..")) != string::npos)
    {
        save_letter.erase(index, 1);
    }

    if (save_letter[0] == '.')
    {
        save_letter.erase(0, 1);
    }

    if (save_letter[save_letter.length() - 1] == '.')
    {
        save_letter.erase(save_letter.length() - 1, 1);
    }

    while ((index = save_letter.find(" ")) != string::npos)
    {
        save_letter.erase(index, 1);
    }

    if (save_letter.length() > 15)
    {
        save_letter = save_letter.substr(0, 15);
    }

    while (save_letter.length() < 3)
    {
        save_letter.append(1, save_letter[save_letter.length() - 1]);
    }

    return save_letter;
}

int main()
{
    cout << solution("abcdefghijklmn.p") << endl;
}