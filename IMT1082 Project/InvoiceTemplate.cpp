#include "InvoiceTemplate.h"
#include "Global.h"
#include <fstream>

InvoiceTemplate::InvoiceTemplate() // Default constructor, init empty template
    : templateCount(0),
      templates(new string[20])
{
    ifstream file ("REG_POST.DTA");

    if (file)
    {
        
        readLine(file, templates[templateCount]);
        while (!file.eof())
            readLine(file, templates[++templateCount]);
    }
}

InvoiceTemplate::~InvoiceTemplate() // Deconstructor
{
    delete[] templates;
}

void InvoiceTemplate::display()
{
    for (int i = 0; i < templateCount; i++)
        cout << templates[i] << " - " << i << endl;
}

string InvoiceTemplate::getTemplate()
{
    string buffer;
    int number = 0;       
    display();                              // displays elements in REG_POST
    buffer = readLine("Number or new description"); // get number or description
    if (isNumber(buffer))                           // checks if number
    {
        number = stoi(buffer);                      // number = buffer
        if (number <= templateCount || number >= 0) // if number is within range
            return templates[number];               // of displayed elements
    }
    return buffer;                                  // return string
}

