#include "stdio.h"
#include "string.h"

enum sort_type
{
    date,
    sender
};

void read_mails(enum sort_type opening_type)
{
    FILE *mp;
    switch (opening_type)
    {
    case date:
        mp = popen("mail", "w");
    case sender:
        mp = popen("mail | sort -k 3", "w");
    }
    fputs("exit", mp);
    pclose(mp);
}

void write_mail(char *receiver, char *subject, char *msg)
{
    FILE *mp;
    char buffer[512];
    sprintf(buffer, "mail -s %s %s", subject, receiver);
    mp = popen(buffer, "w");
    fputs(msg, mp);
    pclose(mp);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        //        mail displaying sort type
        enum sort_type type;
        if (strcmp((argv[1]), "data") == 0)
        {
            type = date;
        }
        else if ((strcmp((argv[1]), "nadawca") == 0))
            type = sender;
        else
        {
            printf("wrong type of arg!");
            return 1;
        }
        read_mails(type);
    }
    // email title content
    else if (argc == 4)
    {
        write_mail(argv[1], argv[2], argv[3]);
    }
    else
    {
        printf("wrong number of args");
        return 1;
    }
    return 0;
}