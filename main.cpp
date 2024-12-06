
// #include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

// max 1000 caracters from the spec sheet
#define MAX_LENGTH_COMMAND_STRING 1000
#define MAX_ARGUMENTS 100

int main(void)
{

    cout << "Basic Posix Shell\n";

    while (1)
    {
        string inputString;
        getline(cin, inputString);
        cout << "raw input : " << "'" << inputString << "'\n";

        // first processing to remove outer spaces

        if (inputString.size() > 1000)
        {
            cerr << "Warning : input size is over 1000 chars - Input will be dropped\n";
            continue;
        }

        // todo extract method
        // left trim
        int posStart = 0;
        while (1)
        {
            if (inputString[posStart] == ' ')
            {
                posStart++;
            }
            else
                break;
        }

        // right trim
        int posEnd = 1;
        while (1)
        {
            if (inputString[inputString.size() - posEnd] == ' ')
            {
                posEnd++;
            }
            else
                break;
        }
        posEnd--;

        // cout << inputString.size() << endl;
        // cout << "striped start " << posStart << endl;
        // cout << "striped end " << posEnd << endl;

        int nbChar = inputString.size() - (posEnd + posStart);
        inputString = inputString.substr(posStart, nbChar);

        cout << "before word spliting : " << "'" << inputString << "'\n";

        // word splitting
        string commandLists[100];
        int pos = 0;
        int lastPos = 0;
        int commandsPos = 0;
        while (pos <= inputString.size())
        {
            if (inputString[pos] == ' ' or inputString[pos] == '\0')
            {
                cout << pos << endl;

                commandLists[commandsPos] = inputString.substr(lastPos, (pos - lastPos));
                lastPos = pos;
                commandsPos++;
                while (inputString[pos++] == ' ')
                {
                    pos++;
                }
            }
                pos++;
        }

        for (int i = 0; i < 100; i++)
        {
            if (!commandLists[i].empty())
                cout << "-" << commandLists[i] << "-" << endl;
        }

        // check to exit the shell
        if (inputString.compare("exit") == 0)
        {
            break;
        }

        cout << "'" << inputString << "'\n";
    }
    cout << "correctly exited the shell." << endl;
    return 0;
}
