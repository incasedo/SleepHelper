#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

//�ַ��滻����PHP�е�str_replace����һ��
string& str_replace(const string& old_value,const string& new_value,string& str)
{
	for(string::size_type pos(0); pos!=string::npos; pos+=new_value.length()) {
		if( (pos=str.find(old_value,pos))!=string::npos )
			str.replace(pos,old_value.length(),new_value);
		else break;
	}
	return str;
}