

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <strings.h>

using std::string;
// ----------------------- Symbol class definition -----------------------

namespace CustomTypes
{
	class Symbol
	{

	public:
		std::string Name;
		string Type;
		string Scope;
		Symbol();
		Symbol(string, string, string);
		Symbol(const char *, const char*, const char *);
		string ToString() { return Name + "\t" + Type + "\t" + Scope; }
	};

	Symbol::Symbol()
	{
		Name = "";
		Type = "";
		Scope = "";
	}

	Symbol::Symbol(string name_, string type_, string scope_)
	{
		Name = name_;
		Type = type_;
		Scope = scope_;
	}

	Symbol::Symbol(const char* name_, const char* type_, const char* scope_)
	{
		Name = string(name_);
		Type = string(type_);
		Scope = string(scope_);
	}

	bool CompareSymbols(const Symbol &first, const Symbol &second)
	{
		if (first.Name < second.Name)
			return true;
		else
			return false;
	}

}



#endif /* STRUCTURES_H_ */
