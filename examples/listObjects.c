/* 
 * Copyright 2014-2016 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000, there is a non-exclusive license for use of this work 
 * by or on behalf of the U.S. Government. Export of this program may require
 * a license from the United States Government.
 *
 * This file is part of the Power API Prototype software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <pwr.h> 

char* myctime(const time_t *timep);
void list_childs_recursively(PWR_Obj parentObj, int indentationLevel);
char* str_repeat(char* base_str, int repetitions);
char* printObjDetails(PWR_Obj curObj, int indentationLevel);

int main( int argc, char* argv[] )
{
    PWR_Grp     grp;
    PWR_Obj     self;
    PWR_Cntxt   cntxt;
    time_t      time;
    int         rc;
    double       value;
    PWR_Time ts;
	PWR_Status  status;

    // Get a context
    rc = PWR_CntxtInit( PWR_CNTXT_DEFAULT, PWR_ROLE_APP, "App", &cntxt );
    assert( PWR_RET_SUCCESS == rc );

    rc = PWR_CntxtGetEntryPoint( cntxt, &self );
    assert( PWR_RET_SUCCESS == rc );
    
	PWR_ObjType objType;
	PWR_ObjGetType( self, &objType );
    printf("I am a `%s`\n", PWR_ObjGetTypeString( objType ) ); 

    list_childs_recursively(self, 1);

	//PWR_CntxtDestroy( cntxt );
    return 0;
}
char* myctime(const time_t *timep)
{
    char* tmp = ctime(timep);
    tmp[ strlen(tmp) - 1 ] = 0; 
    return tmp;
}
void list_childs_recursively(PWR_Obj parentObj, int indentationLevel)
{
  PWR_Grp childObjects;
  int returnCode = PWR_ObjGetChildren( parentObj, &childObjects);
  if( returnCode >= PWR_RET_SUCCESS )
  {
    for(int i = 0; i <PWR_GrpGetNumObjs(childObjects); ++i)
    {
      char childName[100];
      PWR_Obj curChildObj;
      PWR_GrpGetObjByIndx(childObjects, i, &curChildObj);
      PWR_ObjGetName( curChildObj, childName, 100);
      char* indentationStr = str_repeat("-", indentationLevel * 2 - 1);
      printf("%s>%s\n", indentationStr , childName);
      free(indentationStr);
      printObjDetails(curChildObj, indentationLevel);
      list_childs_recursively(curChildObj, indentationLevel + 1);
    }
  } else
  {
    printf("Failure. Could not get a listing of Childs.");
  }
}
char* printObjDetails(PWR_Obj curObj, int indentationLevel)
{
  int rc;
  double value;
  PWR_Time timestamp;
  char* indentationStr = str_repeat(" ", indentationLevel * 2);
  if(PWR_RET_SUCCESS == PWR_ObjAttrIsValid( curObj, PWR_ATTR_VOLTAGE))
  {
    rc = PWR_ObjAttrGetValue( curObj, PWR_ATTR_VOLTAGE, &value, &timestamp);
    if(PWR_RET_SUCCESS == rc) printf("%sVoltage:%f\n", indentationStr, value);
    else printf("%sCould not read Voltage\n", indentationStr);
  } 

  if(PWR_RET_SUCCESS == PWR_ObjAttrIsValid( curObj, PWR_ATTR_ENERGY))
  {
    rc = PWR_ObjAttrGetValue( curObj, PWR_ATTR_ENERGY, &value, &timestamp);
    if(PWR_RET_SUCCESS == rc) printf("%sEnergy:%f\n", indentationStr, value);
    else printf("%sCould not read Energy\n", indentationStr);
  }
}
char* str_repeat(char* base_str, int repetitions)
{
  int oneStrLength = strlen(base_str);
  int wholeStrLength = oneStrLength * repetitions;
  if(1 > wholeStrLength) return "";
  char* outputStr = malloc(wholeStrLength + 1);
  for(int i = 0; i < repetitions;)
  {
    for(int j = 0; j < oneStrLength; ++j)
    {
      outputStr[i] = base_str[j];
      ++i;
    }
  }
  outputStr[wholeStrLength] = 0;

  return outputStr;
}
