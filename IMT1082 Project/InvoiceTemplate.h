#if !defined(__INVOICETEMPLATE_H)
#define  __INVOICETEMPLATE_H

#include <string>
#include "Global.h"
using namespace std;

class InvoiceTemplate
{
private:
	int     templateCount; // Amount of templates
	string* templates;     // The templates

public:
	InvoiceTemplate();     // Default constructor
	~InvoiceTemplate();    // Deconstructor
	void display();
	string getTemplate();
};
#endif