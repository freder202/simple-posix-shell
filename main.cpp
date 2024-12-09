// Frederic Bilodeau - Dec. 8, 2024

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

// max 1000 caracters from the spec sheet
#define MAX_LENGTH_COMMAND_STRING 1000
// max 100 arguments from the spec sheet
#define MAX_ARGUMENTS 100

int main(void)
{
    string inputString;
    while (1)
    {
        cerr << "$ ";
        getline(cin, inputString);

        // from the spec sheet I think I need to do it before trimming 
        if (inputString.size() > MAX_LENGTH_COMMAND_STRING)
        {
            cerr << "Warning : input size is over 1000 chars - Input will be dropped\n";
            continue;
        }

        // left trim
        int posStart = 0;
        while (inputString[posStart] == ' ')
        {
            posStart++;
        }

        // right trim
        int posEnd = 0;
        while (inputString[inputString.size() - (posEnd + 1)] == ' ')
        {
            posEnd++;
        }

        // splicing the input from the trims found
        int nbChar = inputString.size() - (posEnd + posStart);
        inputString = inputString.substr(posStart, nbChar);

        // check to exit the shell
        if (inputString.compare("exit") == 0)
        {
            break;
        }

        // argument counter
        int pos = 0;
        int argCount = -1;
        while (pos <= inputString.size())
        {
            if ((inputString[pos] == ' ' || inputString[pos] == '\0') && inputString[pos - 1] != ' ')
            {
                argCount++;
            }
            pos++;
        }

        if (argCount > MAX_ARGUMENTS)
        {
            cerr << "Warning : argument size is over 100 chars - Input will be dropped\n";
            continue;
        }

        // this is one of the commands that need more processing than only the execvp
        // https://stackoverflow.com/questions/18686114/cd-command-not-working-with-execvp
        if (inputString.substr(0, 3) == "cd ")
        {
            int cdStart = 2;
            while (inputString[++cdStart] == ' ')
                ;

            bool trimMore = inputString[cdStart] == '\'';
            // the trimMore variable is mutliplying the amount to remove on each side
            chdir(inputString.substr(cdStart + trimMore, inputString.size() - (cdStart + (trimMore * 2))).c_str());
        }

        // sequence from lecture 6 of CS110 from Standford
        int newPid = fork();
        if (newPid == 0)
        {
            // everything is changed to a char * because namespace std is used, which makes string default (make the code clearer)
            char *arguments[] = {(char *)"/bin/sh", (char *)"-c", (char *)inputString.c_str(), NULL};

            execvp(arguments[0], arguments);

            exit(0);
        }
        int statusError;
        waitpid(newPid, &statusError, 0);

        if (WEXITSTATUS(statusError) != 0)
        {
            cerr << "error: command exited with code " << WEXITSTATUS(statusError) << endl;
        }
    }
    return 0;
}