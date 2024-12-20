#include "Util.h"
#pragma once
Block fillBuffer(Disk d);
void creatFile (FILE *ms, Disk *D);
int fileExists(FILE *ms, Disk D, char fName[20] );
void insertStudent(FILE *ms, Disk *D, Student newStudent, Meta *meta);
void deleteFile(FILE *ms, Disk *D, char fName[20]) ;
int binarySearch(Student *students, int numRecords, int ID) ;
void insertStudent(FILE *ms, Disk D, Student newStudent, Meta *meta);

