/*
 *
 *  Copyright (c) 2004 Peter Michalek All Rights Reserved.
 *
 *

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Attn: ipr@michalek.org
 *
 *
 */

#if !defined(_ATTRGROUPS_H_)
#define _ATTRGROUPS_H_
#if _MSC_VER > 1000
#pragma once
#pragma  warning( disable : 4786 )
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include <string>
#include "../cups/ipp.h"
using namespace std;


typedef map<string, int> IntMap;

/**
 * Encapsulates a set of groups for given group type.
 * This is used for formating/output purposes.
 */
class AttrGroups  
{
public:
	enum EOutputStyle { OutVertical = 1, OutHorizontal = 2, /*Narrow, OutWide*/ };

private:
	/// used to hold all values for all pertinent attributes
	vector<string>* values;
	/// holds all names in the order they should be displayed and indices 
	/// for easy lookup
	IntMap nameMap;
	/// total groups of given type
	unsigned int groupCount;
	/// max attribute count for a group
	unsigned int attrCount;

public:
	AttrGroups(ipp_t* message, ipp_tag_t attrGroup);
	virtual ~AttrGroups();
	void print(char* printerUri, char* title, EOutputStyle style);
	unsigned int outputIpq(ipp_t* response, char* printerUri, char* title, ipp_tag_t attrGroup, EOutputStyle style);


private:
	void ippCountAttributesInGroup(ipp_t* message, ipp_tag_t group, IntMap &nameMap,
			unsigned int* maxAttrCount, unsigned int* groupCount);
	ipp_attribute_t	*	ippFindNextAttributeInGroup(ipp_t *ipp,	
			ipp_tag_t  group);
	void ippResetIterator(ipp_t* message);
	static char *GetDateTime(time_t t);
	static void ippValueToString(ipp_attribute_t* attr, string& bufferOut);
	void populateValues(ipp_t* response, ipp_tag_t attrGroup);
	// find index of name within names array
	int indexOfName(const char*name);
	/// find name for given index
	/// @return value is temp and must be copied
	const char* /*AttrGroups::*/nameForIndex(int i);

};

#endif // !defined(_ATTRGROUPS_H_)
