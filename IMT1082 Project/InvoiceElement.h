#if !defined(__INVOICEELEMENT_H)
#define  __INVOICEELEMENT_H

#include <string>
#include "listtool2.h"
#include "InvoiceTemplate.h"
#include "Global.h"

using namespace std;

class InvoiceElement : public Element
{
private:
    string  description; // Description of the element
    int     unitPrice;   // Its cost

public:
    InvoiceElement();   // Default constructor
    InvoiceElement(ifstream& file);
    ~InvoiceElement();  // Deconstructor
    void display();     // Dsiplays invoice data
    void writeToFile(ofstream& file); // writes data to file
};

#endif