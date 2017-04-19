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

// AttrGroups.cpp: implementation of the AttrGroups class.
//
//////////////////////////////////////////////////////////////////////
#include "AttrGroups.h"
#include <cups/debug.h>
#include <cups/string.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AttrGroups::AttrGroups(ipp_t* message, ipp_tag_t attrGroup) {
	groupCount = 0;
	attrCount = 0;
	ippCountAttributesInGroup(message, attrGroup, nameMap, &attrCount, &groupCount);
//	names = new char*[attrCount];
	values = new vector<string>[groupCount+1]; 
	// prefill with empty strings
/*
	IntMap::iterator it = nameMap.begin();
	for (int j = 0; j < nameMap.size(); j++, it++) {
		assert(it != nameMap.end());
		const char*name=((*it).first).c_str();
		values[0].push_back(name);
	}
	*/
	for (unsigned int i = 0; i < groupCount+1; i++) {
		for (unsigned int j = 0; j < nameMap.size(); j++) {
			values[i].push_back("");
		}
	}
	//
	populateValues(message, attrGroup);
}


AttrGroups::~AttrGroups() {
	delete []values;
}


void AttrGroups::print(char* printerUri, char* title, EOutputStyle style) {
		if (!attrCount) {
			return;
		}
		unsigned int colCount = attrCount;
		int* lengths = new int[colCount];
		unsigned int i;
		for (i = 0; i < colCount; i++)
				lengths[i] = 0;
		for (i = 0; i < groupCount+1; i++) {
				for (unsigned int j = 0; j < colCount; j++) {
					//if (j < values[i].size()) {
					//	printf("values[%d][%d]=%s\n", i, j, values[i][j].c_str());
					//}
					int length = j < values[i].size() ? values[i][j].length() : 0;
					if (lengths[j] < length)
							lengths[j] = length;
				}
		}
		vector<string> slengths;
		for (i = 0; i < colCount; i++) {
				// create format strings
				char slength[128];
				sprintf(slength, "%%-%ds ", lengths[i]);
				//printf("lengths[%d]=%d, slength=%s\n", i, lengths[i], slength);
				slengths.push_back(slength);
		}
		// do the actual output of all arguments
		printf(title);
		printf("%s\n\n", printerUri);
		if (style == OutHorizontal) {
			for (i = 0; i < groupCount+1; i++) {
				for (unsigned int j = 0; j < colCount; j++) {
					if (j < values[i].size())
						printf(slengths[j].c_str(), values[i][j].c_str());
				}
					printf("\n");
			}
		}
		else {
			// vertical output (no columns). e.g.
			// printer-state: 3
			// printer-uri: http://edel/ipp.cgi
			for (i = 1; i < attrCount; i++) {
				for (unsigned int j = 0; j < groupCount; j++) {
					printf("%s: %s\n", values[0][i].c_str(), values[j+1][i].c_str());
				}
			} 	
			printf("\n");

		}

}


//******************** private functions
void AttrGroups::populateValues(ipp_t* response, ipp_tag_t attrGroup) {
		ipp_attribute_t* attr = NULL;
		// use map to allow throwing out of duplicate attributes
		IntMap lnameMap;

		ippResetIterator(response);
		// get first set to save names
		unsigned int i;
		for (i = 0; i < attrCount; i++) {
			const char* name = nameForIndex(i);
			assert(name);
			//DEBUG_printf(("name=%s\n", name));
			values[0][i] = name;
		}
		unsigned int groupIndex = 0;
		ippResetIterator(response);
		do {
			lnameMap.clear();
			for (i = 0; i < attrCount; i++) {
				do {
					attr = ippFindNextAttributeInGroup(response, attrGroup);
					if (!attr) {
						break;
					} else if (!attr->name) {
						if (i > 0) {
							break;
						} else {
							//fprintf(stderr, "name null at end of previous group\n");
							continue;
						}
					}
					IntMap::iterator it = lnameMap.find(attr->name);
					if (it == lnameMap.end()) {
						IntMap::value_type v(attr->name, lnameMap.size());
						lnameMap.insert(v);
						break;
					} else {
						// throw out if it already exists
						fprintf(stderr, "Found duplicate in name=%s\n", attr->name);
					}
				} while(true);

				if (attr) {
//					char svalue[256];
					char* name = attr->name;
					if (groupIndex >= groupCount) {
						assert(groupIndex < groupCount);
					}
					if (name) {
						int iname = indexOfName(name);
						if (iname >= 0) {
							string svalue;
							ippValueToString(attr, svalue);
							//printf("%s: %s\n", name, svalue);
							if (groupIndex < groupCount) {
								values[groupIndex+1][iname] = svalue;
							}
						} else {
							assert(iname >= 0);
						}
					} else {
						break;
					}
				} else {
					break;
				}
			}
			groupIndex++;
		} while(attr);
}


void AttrGroups::ippValueToString(ipp_attribute_t* attr, 
		string& bufferOut) {
	const char*s = attr->name;
  for (int i = 0; i < attr->num_values; i ++) {
		if (i)
			bufferOut += ',';
		char buffer[IPP_MAX_NAME];
		buffer[0] = 0;
		switch (attr->value_tag)
		{
			case IPP_TAG_INTEGER :
			case IPP_TAG_ENUM :
				if (strncmp(s, "time-at-", 8) == 0)
					sprintf(buffer, GetDateTime(attr->values[i].integer));
				else
					sprintf(buffer, "%d", attr->values[i].integer);
				break;

			case IPP_TAG_BOOLEAN :
				sprintf(buffer, "%d", attr->values[i].boolean);
				break;

			case IPP_TAG_NOVALUE :
				sprintf(buffer, "novalue");
				break;

			case IPP_TAG_RANGE :
				sprintf(buffer, "%d-%d", attr->values[i].range.lower,
					attr->values[i].range.upper);
				break;

      case IPP_TAG_DATE :
				{
					time_t	vtime;		/* Date/Time value */
					struct tm	*vdate;		/* Date info */
					vtime = ippDateToTime(attr->values[i].date);
					vdate = localtime(&vtime);
					strftime(buffer, sizeof(buffer), "%c", vdate);
				}
				break;

			case IPP_TAG_RESOLUTION :
				sprintf(buffer, "%dx%d%s", attr->values[i].resolution.xres,
					attr->values[i].resolution.yres,
					attr->values[i].resolution.units == IPP_RES_PER_INCH ?
				"dpi" : "dpc");
				break;

      case IPP_TAG_STRING :
      case IPP_TAG_TEXTLANG :
      case IPP_TAG_NAMELANG :
      case IPP_TAG_TEXT :
      case IPP_TAG_NAME :
      case IPP_TAG_KEYWORD :
      case IPP_TAG_URI :
      case IPP_TAG_URISCHEME :
      case IPP_TAG_CHARSET :
      case IPP_TAG_LANGUAGE :
      case IPP_TAG_MIMETYPE :
				sprintf(buffer, attr->values[i].string.text);
				break;

/*
      case IPP_TAG_BEGIN_COLLECTION :
          bufferOut += "\n";

          for (i = 0, val = attr->values; i < attr->num_values; i ++, val ++)
						ippValueToString(val->collection, bufferOut);
          break;
*/
			default :
				fprintf(stderr, "Unsupported type: value_tag=%d\n", attr->value_tag);
				break; /* anti-compiler-warning-code */
		}
		bufferOut += buffer;
	}

}

/*
 * taken from scheduler
 * 'GetDateTime()' - Returns a pointer to a date/time string.
*/
char *				/* O - Date/time string */
AttrGroups::GetDateTime(time_t t)		/* I - Time value */
{
  struct tm	*date;		/* Date/time value */
  static time_t	last_time = -1;	/* Last time value */
  static char	s[1024];	/* Date/time string */
  static const char * const months[12] =
		{		/* Months */
		  "Jan",
		  "Feb",
		  "Mar",
		  "Apr",
		  "May",
		  "Jun",
		  "Jul",
		  "Aug",
		  "Sep",
		  "Oct",
		  "Nov",
		  "Dec"
		};


  if (t != last_time)
  {
    last_time = t;

   /*
    * Get the date and time from the UNIX time value, and then format it
    * into a string.  Note that we *can't* use the strftime() function since
    * it is localized and will seriously confuse automatic programs if the
    * month names are in the wrong language!
    *
    * Also, we use the "timezone" variable that contains the current timezone
    * offset from GMT in seconds so that we are reporting local time in the
    * log files.  If you want GMT, set the TZ environment variable accordingly
    * before starting the scheduler.
    *
    * (*BSD and Darwin store the timezone offset in the tm structure)
    */

    date = localtime(&t);

    snprintf(s, sizeof(s), "[%02d/%s/%04d:%02d:%02d:%02d %+03ld%02ld]",
	     date->tm_mday, months[date->tm_mon], 1900 + date->tm_year,
	     date->tm_hour, date->tm_min, date->tm_sec,
#ifdef HAVE_TM_GMTOFF
             date->tm_gmtoff / 3600, (date->tm_gmtoff / 60) % 60);
#else
             timezone / 3600, (timezone / 60) % 60);
#endif /* HAVE_TM_GMTOFF */
  }

  return (s);
}


/**
 * Resets the iterator for ippFindNext* functions
*/
void AttrGroups::ippResetIterator(ipp_t* message) {
	message->current = NULL;
}



/**
 * Returns count of attributes in the given group.
 * Counts only first set of attributes in the first group of this kind.
 * This function can be used for example to calculate number of job attribute groups
 * and number of attributes in each group
 * in a response to get-jobs query.
 * Note: this implementaiton assumes that the first encoutered attr
 *     in the first group of given type will exist in all groups
 *     of given type. Although this is true for cups, it may not be
      true for other servers.
			CUPS needs to provide an API where end of group can be detected
			to handle this properly.

 * @param attrCount: max number of attrs in a group
 * @param nameMap output parameter that will contain a map of all names
 *			with indices in the order in which they were found
 * @param groupCount: total number of groups
 * @return 1 if success, 0 if inconsistent counts accross groups
 */
void AttrGroups::ippCountAttributesInGroup(ipp_t* message, ipp_tag_t group, 
		IntMap &lnameMap, unsigned int* maxAttrCount, unsigned int* groupCount) {
  ipp_attribute_t	*attr;			/* Current atttribute */
	ipp_tag_t		group_tag;		/* Group tag */
	unsigned int aCount = 0;
	unsigned int masterAttrCount = 0;
	*maxAttrCount = 0;
	*groupCount = 0;
	lnameMap.clear();

	if (message != NULL) {
		attr = message->attrs;
		const char* firstName = "";
		for (; attr != NULL; attr = attr->next)
		{
			//printf("name=%s\n", attr->name);
			group_tag = (ipp_tag_t)(attr->group_tag & IPP_TAG_MASK);
			// maxAttrCount only first set of attributes in the first group of this kind
			if ( attr != message->attrs && attr->name && !strcmp(attr->name, firstName)) { 
				// end of group
				if (masterAttrCount == 0) {
					// check for consistent attrCount accross all groups
					masterAttrCount = aCount;
				} else if (masterAttrCount < aCount){
					masterAttrCount = aCount;
				}
				(*groupCount)++;
				aCount = 0;
			}
			
			if (attr->name != NULL	&&
					(group_tag == group || group == IPP_TAG_ZERO))
			{
				// found first attr in given group type
				if (!firstName || firstName[0] == 0) {
					firstName = attr->name;
					(*groupCount)++;
				}
				aCount++;
				IntMap::iterator it = lnameMap.find(attr->name);
				if (it == lnameMap.end()) {
					IntMap::value_type v(attr->name, lnameMap.size());
					lnameMap.insert(v);
				}

			}
		}
	}
	if (!masterAttrCount && aCount > 0) {
		masterAttrCount = aCount;
	}
	if (masterAttrCount > lnameMap.size()) {
		DEBUG_printf(("There are probably multiple values of some single-valued attribute within the IPP message.\n"));
		DEBUG_printf(("The extra values will be ignored\n"));
		masterAttrCount = lnameMap.size();
	}
	*maxAttrCount = masterAttrCount;
}


/**
 * 
	Find next attribute belonging to given group.
	If group 0, find next attribute belonging to any group.
	Valid group value:
  IPP_TAG_ZERO = 0x00,
  IPP_TAG_OPERATION,
  IPP_TAG_JOB,
  IPP_TAG_END,
  IPP_TAG_PRINTER,
  IPP_TAG_UNSUPPORTED_GROUP,
  IPP_TAG_SUBSCRIPTION,
  IPP_TAG_EVENT_NOTIFICATION,

*/
ipp_attribute_t	*				/* O - Matching attribute */
	AttrGroups::ippFindNextAttributeInGroup(ipp_t      *ipp,		/* I - IPP request */
		     ipp_tag_t  group)		/* I - Group of attribute */
{
  ipp_attribute_t	*attr;			/* Current atttribute */
	ipp_tag_t		group_tag;		/* Group tag */


  DEBUG_printf(("ippFindNextAttributeInGroup(%p, group=%d\n", ipp, group));

  if (ipp == NULL)
    return (NULL);

  if (ipp->current)
    attr = ipp->current->next;
  else
    attr = ipp->attrs;

  for (; attr != NULL; attr = attr->next)
  {
    DEBUG_printf(("ippFindNextAttributeInGroup: attr = %p, name = \'%s\'\n", attr,
                  attr->name));

    group_tag = (ipp_tag_t)(attr->group_tag & IPP_TAG_MASK);

    if (attr->name != NULL  &&
        (group_tag == group || group == IPP_TAG_ZERO))
    {
      ipp->current = attr;
      return (attr);
    } else if (!attr->name) {
			DEBUG_printf(("ippFindNextAttributeInGroup: name is null - end of group"));
      ipp->current = attr;
			return attr;
		}
  }

  ipp->current = NULL;
  return (NULL);
}

int AttrGroups::indexOfName(const char*name) {
	int rc = -1;
	IntMap::iterator it = nameMap.find(name);
	if (it != nameMap.end()) {
		rc = (*it).second;
	}
	return rc;
}

const char* AttrGroups::nameForIndex(int i) {
	const char* name = NULL;
	IntMap::iterator it;
	for (it = nameMap.begin(); it != nameMap.end(); it++) {
		int currentIndex = (*it).second;
		if (i == currentIndex) {
			name = ((*it).first).c_str();
			break;
		}
	}
	return name;
}
