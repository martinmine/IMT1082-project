#include "InvoiceElement.h"
#include <fstream>


InvoiceElement::InvoiceElement() // Default constructor, init empty
{
	InvoiceTemplate* Invoice;               // new invoicetemplate object ptr
	Invoice = new InvoiceTemplate;
	description = Invoice->getTemplate();	// gets the description of the bill
											// gets the cost of the bill
	unitPrice = readValue<int>("Unit price: ", 1, 1000);  
}

InvoiceElement::InvoiceElement(ifstream& file) // cctor that reads from file
{
	readLine(file, description);
	unitPrice = readValue<int>(file);
	file.ignore();
}

InvoiceElement::~InvoiceElement() // Deconstructor
{

}

void InvoiceElement::display() // Displays all data
{
	cout << "Description: " << description << endl
		 << "Unit price: "  << unitPrice   << endl;
}

void InvoiceElement::writeToFile(ofstream& file) // Writes all data to file
{
	file << description << endl
		 << unitPrice   << endl;
}